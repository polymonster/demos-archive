/*    
	This file is part of ps2padlib.

	Copyright 2003, Alex Mole

    ps2padlib is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ps2padlib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ps2padlib; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "pad.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <math.h>
#include <linux/ps2/dev.h>
#include <memory.h>

//#include <stdio.h>

//the analogue sticks don't centre to zero- only to a relatively small range.
//values smaller than DRIFT_TOLERANCE are treated as zero
#define DRIFT_TOLERANCE		30


padinfo_t pad[2];

//the pad devices
static const char *devices[2] = { PS2_DEV_PAD0, PS2_DEV_PAD1 };

static int fd[2] = {-1,-1};
static int flags[2] = { 0, 0 };


typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
#define PACKED	__attribute__((packed))

struct digital_data
{
	u8 	unknown0			PACKED;
	u8 	flags		: 4		PACKED;
	u8 	type		: 4		PACKED;
	u16	buttons				PACKED;
} __attribute__((aligned(2)));

struct dualshock2_data
{
	u8 	unknown0			PACKED;
	u8 	flags		: 4		PACKED;
	u8 	type		: 4		PACKED;
	u16	buttons				PACKED;
	u8	rx					PACKED;
	u8	ry					PACKED;
	u8	lx					PACKED;
	u8	ly					PACKED;
	u8	pright				PACKED;
	u8	pleft				PACKED;
	u8	pup					PACKED;
	u8	pdown				PACKED;
	u8	ptri				PACKED;
	u8	pcircle				PACKED;
	u8	pcross				PACKED;
	u8	psquare				PACKED;
	u8	pl1					PACKED;
	u8	pr1					PACKED;
	u8	pl2					PACKED;
	u8	pr2					PACKED;
} __attribute__((aligned(2)));

union pad_data 
{
	u8 buf[PS2PAD_DATASIZE];
	struct digital_data dig;
	struct dualshock2_data ds2;
};


static int getstat (int padnum)
{
	int res;
	ioctl (fd[padnum], PS2PAD_IOCGETSTAT, &res);
	return res;
}

static int wait_stat_not_busy (int padnum)
{
	int res;
	while ((res=getstat(padnum)) == PS2PAD_STAT_BUSY) {}
	return res;
}


static int actuator_supported (int padnum)
{
	struct ps2pad_actinfo act_info;

	act_info.actno = -1;
	act_info.term = 0;
	act_info.result = 0;
	ioctl(fd[padnum], PS2PAD_IOCACTINFO, &act_info);

	return (act_info.result == 2);
}


static void enable_actuator(int padnum, int enable_small, int enable_big)
{
	struct ps2pad_act act;
	
	act.len = 6;
	memset(act.data, -1, sizeof(act.data));

	if (enable_small && enable_big)
	{
		act.data[0] = 0;
		act.data[1] = 1;
	}
	else if (enable_small)
	{
		act.data[0] = 0;
	}
	else if (enable_big)
	{
		act.data[1] = 0;
	}

	ioctl(fd[padnum], PS2PAD_IOCSETACTALIGN, &act);
}

static void set_actuator(int padnum, unsigned char small_intensity, unsigned char big_intensity)
{
	struct ps2pad_act act;
	
	act.len = 6;
	memset(act.data, -1, sizeof(act.data));

	if (small_intensity > 1)
		small_intensity = 1;

	act.data[0] = small_intensity;	//Small shaker
	act.data[1] = big_intensity;	//Big shaker
	
	ioctl(fd[padnum], PS2PAD_IOCSETACT, &act);
}



static int setmode (int padnum, int mode, int lock)
{
	struct ps2pad_mode pmode;
	pmode.offs = mode;
	pmode.lock = lock?3:2;
	return (ioctl (fd[padnum], PS2PAD_IOCSETMODE, &pmode) == 0);
}


static int pressure_supported (int padnum)
{
	int pressmode;
	ioctl (fd[padnum], PS2PAD_IOCPRESSMODEINFO, &pressmode);
	return (pressmode == 1);
}

static int enterpressmode (int padnum)
{
	return (ioctl (fd[padnum], PS2PAD_IOCENTERPRESSMODE) == 0);
}

static int exitpressmode (int padnum)
{
	return (ioctl (fd[padnum], PS2PAD_IOCEXITPRESSMODE) == 0);
}
		

static int init (int padnum, int initflags)
{
	int mode, lock;
	int stat;	

	flags[padnum] = initflags;
	
	fd[padnum] = open (devices[padnum], O_RDONLY);
	if (fd[padnum] < 0) return 0;

	while ((stat = getstat(padnum)) == PS2PAD_STAT_BUSY) {}
	if (stat != PS2PAD_STAT_READY)
	{
		//something went wrong...
		close (fd[padnum]);
		fd[padnum] = -1;
		return 0;
	}
	
	mode = (initflags & PAD_INIT_ANALOGUE) ? 1 : 0;
	lock = (initflags & PAD_INIT_LOCK) ? 1 : 0;	
	if (!setmode (padnum, mode, lock))
	{
		close (fd[padnum]);
		fd[padnum] = -1;
		return 0;
	}
	wait_stat_not_busy (padnum);

	if ((initflags & PAD_INIT_ANALOGUE) &&
			(initflags & PAD_INIT_PRESSURE) &&
			 pressure_supported (padnum))
	{
		if (!enterpressmode (padnum))
		{
			setmode (padnum, 0, 0);
			close (fd[padnum]);
			fd[padnum] = -1;
			return 0;
		}
		
		wait_stat_not_busy (padnum);
	}
			
	//initialise the structure to zero
	memset (&pad[padnum], 0, sizeof(padinfo_t));
	
	//store initflags in pad data structure	
	pad[padnum].initflags = initflags;
	
	//set the actuator info in pad structure
	pad[padnum].actuator = actuator_supported (padnum);

	
	return 1;
}


int pad_init (int pads, int initflags)
{
	if ((pads & PAD_0) && !init (0, initflags))
		return 0;
	if ((pads & PAD_1) && !init (1, initflags))
		return 0;

	return 1;
}


static float axis_to_float (int axisval)
{
	if (abs(axisval-127) < DRIFT_TOLERANCE)
		return 0.0f;
	return (((float)axisval) - 127.5f) * (1.0f / 127.5f);
}

static float press_to_float (int pressureval)
{
	return ((float)pressureval) * (1.0f / 255.0f);
}


static void update (int padnum)
{
	union pad_data data;
	int n;
	int changed, last;
	int status;
	
	//ignore requests for unopened pads
	if (fd[padnum] < 0)
		return;

	//read the status
	do {
		status = getstat (padnum);
	} while (status == PS2PAD_STAT_BUSY);

	n = read (fd[padnum], data.buf, sizeof(data));
	if (n <= 0)
		return;
	
	//update the buttons
	last = pad[padnum].buttons;
	//buttons are inverted!
	pad[padnum].buttons = ~data.dig.buttons;
	changed = last ^ ~data.dig.buttons;
	pad[padnum].pressed = pad[padnum].buttons & changed;
	pad[padnum].released = last & changed;

	if (flags[padnum] & PAD_INIT_ANALOGUE)
	{
		//update the analogue sticks
		pad[padnum].axes[PAD_AXIS_LX] = axis_to_float (data.ds2.lx);
		pad[padnum].axes[PAD_AXIS_LY] = axis_to_float (data.ds2.ly);
		pad[padnum].axes[PAD_AXIS_RX] = axis_to_float (data.ds2.rx);
		pad[padnum].axes[PAD_AXIS_RY] = axis_to_float (data.ds2.ry);

		if (flags[padnum] & PAD_INIT_PRESSURE)
		{
			//update the button pressures
			pad[padnum].pressures[PAD_PCROSS] = press_to_float (data.ds2.pcross);
			pad[padnum].pressures[PAD_PTRI] = press_to_float (data.ds2.ptri);
			pad[padnum].pressures[PAD_PCIRCLE] = press_to_float (data.ds2.pcircle);
			pad[padnum].pressures[PAD_PSQUARE] = press_to_float (data.ds2.psquare);
			pad[padnum].pressures[PAD_PUP] = press_to_float (data.ds2.pup);
			pad[padnum].pressures[PAD_PDOWN] = press_to_float (data.ds2.pdown);
			pad[padnum].pressures[PAD_PLEFT] = press_to_float (data.ds2.pleft);
			pad[padnum].pressures[PAD_PRIGHT] = press_to_float (data.ds2.pright);
			pad[padnum].pressures[PAD_PR1] = press_to_float (data.ds2.pr1);
			pad[padnum].pressures[PAD_PR2] = press_to_float (data.ds2.pr2);
			pad[padnum].pressures[PAD_PL1] = press_to_float (data.ds2.pl1);
			pad[padnum].pressures[PAD_PL2] = press_to_float (data.ds2.pl2);
		}
	}
}

void pad_update (int pads)
{
	if (pads & PAD_0) update (0);
	if (pads & PAD_1) update (1);
}


static void cleanup (int padnum)
{
	if (flags[padnum] & PAD_INIT_PRESSURE)
		exitpressmode (padnum);
	setmode (padnum, 0, 0);
	wait_stat_not_busy (padnum);
	close (fd[padnum]);
	fd[padnum] = -1;
}

void pad_cleanup (int pads)
{
	if (pads & PAD_0) cleanup (0);
	if (pads & PAD_1) cleanup (1);
}


