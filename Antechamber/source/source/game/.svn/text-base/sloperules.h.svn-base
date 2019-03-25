#ifndef _SLOPERULES_H
#define _SLOPERULES_H

#define NUMBER_OF_SLOPES 24

typedef enum
{
	NULL_COLLISION = 0,
	WALL_AND_FLOOR,
	SLOPE,

}block_types_e;

typedef struct
{
	float m, c_ratio;

}slope_formula_s;

extern slope_formula_s slope_formula[NUMBER_OF_SLOPES];

void set_slope_formulas();


#endif