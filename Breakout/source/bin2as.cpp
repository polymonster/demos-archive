// simple ELF code segment ripper

#include <stdio.h>
#include <assert.h> // used for runtime asserts !


typedef unsigned char byte;

int main(int argc, char* argv[])
{
	assert(argc == 3);
	const char* szTag = argv[1];
	const char* szInFile = argv[2];

	FILE* pf = fopen(szInFile, "rb");
	assert(pf != NULL);

	printf("\t.text\n");
	printf("\t.align 4 # 16 bytes\n");
	printf("\t.global VU_%s_start\n", szTag);
	printf("VU_%s_start:\n", szTag);

	while (1)
	{
		unsigned int data[4]; // 128 bits
		assert(sizeof(data) == 128/8);
		int cb = fread(data, 1, 16, pf);
		if (cb == 0)
			break;
		assert(cb == 16);
		printf("\t.word 0x%08X, 0x%08X, 0x%08X, 0x%08x\n",
			data[0], data[1], data[2], data[3]);
	}

	printf("\t.global VU_%s_end\n", szTag);
	printf("VU_%s_end:\n", szTag);

	fclose(pf);

	return 0;
}

