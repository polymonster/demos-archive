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

#ifndef __PAD_H_INCLUDED__
#define __PAD_H_INCLUDED__


#ifdef __cplusplus
extern "C" {
#endif

#include <linux/ps2/pad.h>
	

//bitmasks for digital button presses	
#define PAD_SELECT	(1 << 0)
#define PAD_L3		(1 << 1)
#define PAD_R3		(1 << 2)
#define PAD_START	(1 << 3)
#define PAD_UP		(1 << 4)
#define PAD_RIGHT	(1 << 5)
#define PAD_DOWN	(1 << 6)
#define PAD_LEFT	(1 << 7)
#define PAD_L2		(1 << 8)
#define PAD_R2		(1 << 9)
#define PAD_L1		(1 << 10)
#define PAD_R1		(1 << 11)
#define PAD_TRI		(1 << 12)
#define PAD_CIRCLE	(1 << 13)
#define PAD_CROSS	(1 << 14)
#define PAD_SQUARE	(1 << 15)

//indices into axes	
#define PAD_AXIS_LX	0
#define PAD_AXIS_LY	1
#define PAD_AXIS_RX	2
#define PAD_AXIS_RY	3
	
//indices into pressures
#define PAD_PLEFT	0
#define PAD_PRIGHT	1
#define PAD_PUP		2
#define PAD_PDOWN	3
#define PAD_PTRI	4
#define PAD_PCIRCLE	5
#define PAD_PCROSS	6
#define PAD_PSQUARE	7
#define PAD_PL1		8
#define PAD_PR1		9
#define PAD_PL2		10
#define PAD_PR2		11
	
//use these with the pad_* functions
#define PAD_0		1
#define PAD_1		2

//pad init flags	
#define PAD_INIT_DIGITAL	0x00	//select digital mode (default)
#define PAD_INIT_ANALOGUE	0x01	//select analogue mode
#define PAD_INIT_UNLOCK		0x00	//do not lock the pad mode
#define PAD_INIT_LOCK		0x02	//lock the pad mode
#define PAD_INIT_PRESSURE	0x04	//enable button pressure sensing
	

//the structure used to describe the state of a pad	
typedef struct
{
	int initflags;			//flags used to initialise the pad state
	int actuator;			//=1 if actuator supported otherwise 0
	int buttons;			//a bitfield describing which buttons are
							//currently pressed
	
	float axes[4];			//these values are mapped from -1 -> +1	
	float pressures[12];	//these values are mapped from 0 -> 1

	//state change info
	int pressed;			//like buttons, but contains buttons 
							//pressed since the last update
	int released;			//like buttons, but contains buttons
							//released since the last update
} padinfo_t;


//pad info for both pads
extern padinfo_t pad[2];


//pads should be PAD_0 and/or PAD_1, or'd together
//pad_init returns zero on failure
//actuator_supported returns 1 if supported otherwise 0
int pad_init (int pads, int initflags);
void pad_update (int pads);
void pad_cleanup (int pads);
//int actuator_supported (int padnum);
void enable_actuator(int padnum, int enable_small, int enable_big);
void set_actuator(int padnum, unsigned char small_intensity, unsigned char big_intensity);


#ifdef __cplusplus
};
#endif

#endif

