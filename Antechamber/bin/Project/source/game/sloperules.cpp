#include "sloperules.h"

slope_formula_s slope_formula[NUMBER_OF_SLOPES];

void set_slope_formulas()
{
	slope_formula[0].m = 1.0f;
	slope_formula[0].c_ratio = 1.0f;

	slope_formula[1].m = 0.75f;
	slope_formula[1].c_ratio = 0.75f;

	slope_formula[2].m = 0.5f;
	slope_formula[2].c_ratio = 0.5f;

	slope_formula[3].m = 0.5f;
	slope_formula[3].c_ratio = 1;

	slope_formula[4].m = 0.25f;
	slope_formula[4].c_ratio = 0.25f;

	slope_formula[5].m = 0.25f;
	slope_formula[5].c_ratio = 0.5f;

	slope_formula[6].m = 0.25f;
	slope_formula[6].c_ratio = 0.75f;

	slope_formula[7].m = 0.25f;
	slope_formula[7].c_ratio = 1.0f;

	slope_formula[8].m = -0.25f;
	slope_formula[8].c_ratio = 0.75f;

	slope_formula[9].m = -0.25f;
	slope_formula[9].c_ratio = 0.5f;

	slope_formula[10].m = -0.25f;
	slope_formula[10].c_ratio = 0.25f;

	slope_formula[11].m = -0.25f;
	slope_formula[11].c_ratio = 0;

	slope_formula[12].m = -0.5f;
	slope_formula[12].c_ratio = 0.5f;

	slope_formula[13].m = -0.5f;
	slope_formula[13].c_ratio = 0;

	slope_formula[14].m = -0.75f;
	slope_formula[14].c_ratio = 0;

	slope_formula[15].m = -1.0f;
	slope_formula[15].c_ratio = 0;

	//fractional blocks
	slope_formula[16].m = 0.0f;
	slope_formula[16].c_ratio = 0.75f;

	slope_formula[17].m = 0.0f;
	slope_formula[17].c_ratio = 0.5f;

	slope_formula[18].m = 0.0f;
	slope_formula[18].c_ratio = 0.25f;

	//upside down slopes
	slope_formula[19].m = 1.0f;
	slope_formula[19].c_ratio = 0.0f;

	slope_formula[20].m = -1.0f;
	slope_formula[20].c_ratio = -1.0f;

	slope_formula[21].m = 0.5f;
	slope_formula[21].c_ratio = -0.5f;

	slope_formula[22].m = 0.5f;
	slope_formula[22].c_ratio = 0.0f;

	slope_formula[23].m = -0.5f;
	slope_formula[23].c_ratio = -0.5f;

	slope_formula[24].m = -0.5f;
	slope_formula[24].c_ratio = -1.0f;

	slope_formula[25].m = 0.0f;
	slope_formula[25].c_ratio = -0.5f;
}