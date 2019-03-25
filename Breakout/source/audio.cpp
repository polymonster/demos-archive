#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/soundcard.h>
#include <malloc.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "audio.h" 

#define AUDIO_DSP0 "/dev/dsp"
#define AUDIO_DSP1 "/dev/dsp1"
#define AUDIO_DIR "audio/"

AudioDevice::AudioDevice(const int Which)
{
	m_Current = NULL;
	m_AudioFD = 0;
	m_FragmentSize = 0;
	m_Silence = NULL;
	
	// open the audio device
	Open(Which);
}


AudioDevice::~AudioDevice()
{
	if(m_AudioFD)
	{
		close(m_AudioFD);
		m_AudioFD = 0;
	}
	
	if(m_Silence)
	{
		free(m_Silence);
		m_Silence = 0;
	}
}



int AudioDevice::GetFreeFragments(void) const
{
	audio_buf_info info;

	if(ioctl(m_AudioFD, SNDCTL_DSP_GETOSPACE, &info) == -1) return 0;
	return info.fragments;
}

void AudioDevice::Close(void) 
{
	if(m_AudioFD)
	{
		close(m_AudioFD);
		m_AudioFD = 0;
	}
	
	if(m_Silence)
	{
		free(m_Silence);
		m_Silence = 0;
	}
}

int AudioDevice::Open(const int Which) 
{
	int format = AFMT_U8;
	int stereo = 0;
	int speed = 11025;
	
	if(!Which) 
	{
		m_AudioFD = open(AUDIO_DSP0,O_WRONLY,0);
		if(m_AudioFD == -1) 
		{
			perror(AUDIO_DSP0);
			exit(1);
		}	
	}
	else 
	{
		//fprintf(stderr,"Using alternate device\n");
		m_AudioFD = open(AUDIO_DSP1,O_WRONLY,0);
		if(m_AudioFD == -1) 
		{
			perror(AUDIO_DSP1);
			exit(1);
		}
	}
/*
	int arg = 0xMMMMSSSS
	
	Argument of this call is an integer encoded as 0xMMMMSSSS (in hex). 
	The 16 least significant bits determine the fragment size. The size is 2SSSS. 
	For example SSSS=0008 gives fragment size of 256 bytes (28). 
	The minimum is 16 bytes (SSSS=4) and the maximum is total_buffer_size/2. 
	Some devices or processor architectures may require larger fragments in this 
	case the requested fragment size is automatically increased.  
	The 16 most significant bits (MMMM) determine maximum number of fragments. 
	By default the deriver computes this based on available buffer space. 
	The minimum value is 2 and the maximum depends on the situation. 
	Set MMMM=0x7fff if you don't want to limit the number of fragments.  

	int arg = 0xfFFF0008;
	if(ioctl(audio_fd, SNDCTL_DSP_SETFRAGMENT, &arg)==-1)
	{
		 perror("SNDCTL_DSP_SETFRAGMENT");
		 exit(20);
	}
*/	
	if(ioctl(m_AudioFD,SNDCTL_DSP_SETFMT,&format)==-1) 
	{
		perror("SNDCTL_DSP_SETFMT");
		exit(2);
	} 
	
	if(ioctl(m_AudioFD,SNDCTL_DSP_STEREO,&stereo)==-1) 
	{
		perror("SNDCTL_DSP_STEREO");
		exit(3);
	}
	
	if(ioctl(m_AudioFD,SNDCTL_DSP_SPEED,&speed)==-1) 
	{
		perror("SNDCTL_DSP_SPEED");
		exit(4);
	}
	
	if(ioctl(m_AudioFD,SNDCTL_DSP_GETBLKSIZE,&m_FragmentSize)==-1) 
	{
		perror("SNDCTL_DSP_GETBLKSIZE");
		exit(5);
	}
	
	//printf("Fragment size: %d\n", m_FragmentSize);
	
	m_Silence = (unsigned char *)malloc(m_FragmentSize);
	for(int i=0;i<m_FragmentSize;i++) m_Silence[i]=0x80;
	

	return m_AudioFD;
}

void AudioDevice::SetSoundSample( SoundSample * const Sample)
{
	m_Current = Sample;
}


void AudioDevice::HandleAudio(void) 
{
	unsigned char * audiobuf;
	int ff,i;
	

	ff = GetFreeFragments();
	
	if(!ff) return;
	
	if(m_Current == NULL)
	{
		audiobuf = m_Silence;
		for(i=0;i<ff;i++) 
		{
			write(m_AudioFD, audiobuf, m_FragmentSize);
		}
		return;
	}
	
	for(i=0;i<ff;i++) 
	{
    	if(m_Current)
    	{
    		audiobuf = m_Current->GetNextFragment();
    		if(!audiobuf) 
    		{
    			m_Current = NULL;
    			audiobuf = m_Silence;
    		}
    	}
    	else 
    	{
    		audiobuf = m_Silence;
    	}
    	
    	write(m_AudioFD, audiobuf, m_FragmentSize);
	 }
}

int AudioDevice::GetFragmentSize(void) const
{
	return m_FragmentSize;
}


SoundSample::SoundSample(char * Filename, AudioDevice * const pAD)
{
	m_Location = 0;
	m_Stream = NULL;
	m_FragSize = 0;
	m_NumFragments = 0;
	m_pAD = pAD;

	Load(Filename);
}


SoundSample::~SoundSample() 
{
	if(m_Stream) 
	{
		free(m_Stream);
		m_Stream = NULL;
	}
}


void SoundSample::Reset() 
{
	m_Location = 0;
}

unsigned char * SoundSample::GetNextFragment() 
{
	if(m_Location >= m_NumFragments) return NULL;
	return &m_Stream[(m_Location++) * m_FragSize];
}

void SoundSample::Load(char * Filename)
{
	struct stat file_stat;
	FILE * fp;
	int i;
	long filesize;
	char buf[100];
	
	m_FragSize = m_pAD->GetFragmentSize();
	
	strncpy(m_SampleName, Filename, 32);
	sprintf(buf, "%s%s.raw", AUDIO_DIR, Filename);
	printf("Loading file %s\n",buf);
	
	if(stat(buf, &file_stat) <0 ) 
		{
		printf("Failed to stat file -> %s!\n", buf);
		exit(1);
		}
		
	//printf("Finished with the file stat.\n");
	filesize = file_stat.st_size;
	//printf("File size: %ld\n", filesize);
	
	// Sanity check at this point
	assert(m_FragSize > 0);
	
	m_NumFragments = filesize/m_FragSize;
	m_NumFragments++;
	//printf("Number of fragments: %ld\n\n", m_NumFragments);
	
	// Allocate memory for sound
	m_Stream = (unsigned char *)malloc(m_NumFragments*m_FragSize);
	
	// Open sound file
	fp=fopen(buf,"r");
	if(!fp) 
		{
		printf("Failed to open file -> %s!\n", buf);
		exit(1);
		}
	
	// Fill sound buffer	
	for(i=0;i<filesize;i++) 
		{
		m_Stream[i]=fgetc(fp);
		}
	
	// Fill buffer to end with silence	
	for(i=filesize;i<m_NumFragments*m_FragSize;i++) m_Stream[i]=0x80;
	
	// Close the sound file
	fclose(fp);
}


void SoundSample::Play(void)
{
	m_Location = 0;
	m_pAD->SetSoundSample(this);
}


/*
STAT(2)                    System calls                   STAT(2)

NAME
       stat, fstat, lstat - get file status

SYNOPSIS
       #include <sys/stat.h>
       #include <unistd.h>

       int stat(const char *file_name, struct stat *buf);
       int fstat(int filedes, struct stat *buf);
       int lstat(const char *file_name, struct stat *buf);

DESCRIPTION
       These  functions  return  information  about the specified
       file.  You do not need any access rights to  the  file  to
       get  this  information  but  you need search rights to all
       directories named in the path leading to the file.

       stat stats the file pointed to by file_name and  fills  in
       buf.

       lstat  is  identical  to  stat,  only  the  link itself is
       stated, not the file  that  is  obtained  by  tracing  the
       links.

       fstat  is identical to stat, only the open file pointed to
       by filedes (as returned by open(2)) is stated in place  of
       file_name.

       They  all return a stat structure, which contains the fol-
       lowing fields:

              struct stat
              {
                  dev_t         st_dev;      // device 
                  ino_t         st_ino;      // inode 
                  mode_t        st_mode;     // protection 
                  nlink_t       st_nlink;    // number of hard links 
                  uid_t         st_uid;      // user ID of owner 
                  gid_t         st_gid;      // group ID of owner 
                  dev_t         st_rdev;     // device type (if inode device) 
                  off_t         st_size;     // total size, in bytes 
                  unsigned long st_blksize;  // blocksize for filesystem I/O 
                  unsigned long st_blocks;   // number of blocks allocated 
                  time_t        st_atime;    // time of last access 
                  time_t        st_mtime;    // time of last modification 
                  time_t        st_ctime;    // time of last change 
              };

       The value st_blocks gives the size of the file in 512-byte
       blocks.  The value st_blksize gives the "preferred" block-
       size for efficient file system I/O.  (Writing to a file in
       smaller  chunks  may  cause  an  inefficient  read-modify-
       rewrite.)

       Not all of the Linux filesystems implement all of the time
       fields.   Traditionally,  st_atime is changed by mknod(2),
       utime(2), read(2), write(2), and truncate(2).

       Traditionally, st_mtime is changed by mknod(2),  utime(2),
       and  write(2).  The st_mtime is not changed for changes in
       owner, group, hard link count, or mode.

       Traditionally, st_ctime is changed by writing or  by  set-
       ting  inode  information  (i.e., owner, group, link count,
       mode, etc.).

       The following POSIX macros are defined to check  the  file
       type:

              S_ISLNK(m)  is it a symbolic link?

              S_ISREG(m)  regular file?

              S_ISDIR(m)  directory?

              S_ISCHR(m)  character device?

              S_ISBLK(m)  block device?

              S_ISFIFO(m) fifo?

              S_ISSOCK(m) socket?

       The following flags are defined for the st_mode field:

              S_IFMT   00170000  bitmask  for  the file type bit-
                       fields (not POSIX)

              S_IFSOCK 0140000 socket (not POSIX)

              S_IFLNK  0120000 symbolic link (not POSIX)

              S_IFREG  0100000 regular file (not POSIX)

              S_IFBLK  0060000 block device (not POSIX)

              S_IFDIR  0040000 directory (not POSIX)

              S_IFCHR  0020000 character device (not POSIX)

              S_IFIFO  0010000 fifo (not POSIX)

              S_ISUID  0004000 set UID bit

              S_ISGID  0002000 set GID bit

              S_ISVTX  0001000 sticky bit (not POSIX)

              S_IRWXU  00700 user (file owner)  has  read,  write
                       and execute permission

              S_IRUSR  00400  user  has  read permission (same as
                       S_IREAD, which is not POSIX)

              S_IWUSR  00200 user has write permission  (same  as
                       S_IWRITE, which is not POSIX)

              S_IXUSR  00100 user has execute permission (same as
                       S_IEXEC, which is not POSIX)

              S_IRWXG  00070 group has read,  write  and  execute
                       permission

              S_IRGRP  00040 group has read permission

              S_IWGRP  00020 group has write permission

              S_IXGRP  00010 group has execute permission

              S_IRWXO  00007  others have read, write and execute
                       permission

              S_IROTH  00004 others have read permission

              S_IWOTH  00002 others have write permisson

              S_IXOTH  00001 others have execute permission

RETURN VALUE
       On success, zero is returned.  On error, -1  is  returned,
       and errno is set appropriately.

ERRORS
       EBADF  filedes is bad.

       ENOENT A  component  of the path file_name does not exist,
              or the path is an empty string.

       ENOTDIR
              A component of the path is not a directory.

       ELOOP  Too many symbolic links encountered while  travers-
              ing the path.

       EFAULT Bad address.

       EACCES Permission denied.

       ENOMEM Out of memory (i.e. kernel memory).

       ENAMETOOLONG
              File name too long.

CONFORMING TO
       The  stat  and  fstat  calls conform to SVr4, SVID, POSIX,
       X/OPEN, BSD 4.3.  The lstat call conforms  to  4.3BSD  and
       SVr4.   SVr4  documents  additional fstat error conditions
       EINTR, ENOLINK, and EOVERFLOW.  SVr4 documents  additional
       stat  and lstat error conditions EACCES, EINTR, EMULTIHOP,
       ENOLINK, and EOVERFLOW.  Use of the st_blocks and  st_blk-
       size fields may be less portable. (They were introduced in
       BSD.  Are not specified by POSIX. The interpretation  dif-
       fers between systems, and possibly on a single system when
       NFS mounts are involved.)

SEE ALSO
       chmod(2), chown(2), readlink(2), utime (2)

Linux                      May 13, 1998                         1


*/