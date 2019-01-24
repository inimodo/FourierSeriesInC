#include <math.h>
#include "CCD.h"

#define _P_SIZE 80
#define _P_WIDTH (16*_P_SIZE)
#define _P_HEIGHT (9*_P_SIZE) 
#define _P_YMIDD (float)((float)_P_HEIGHT/(float)2)
#define _P_GRAPHHEIGHT (float)((float)_P_HEIGHT/(float)10)
#define _P_DEGTORAD  (float)(3.14159265359f/360.f)
#define _P_YSCAN 0.5f
#define _P_QUAL 0.5f
#define _P_QUULUNIT 1
#define _P_LINECOLOR (_CCD_ALIASCOLOR){255,255,255}
_CCD_ALIAS c_canvas;
float * f_amp, * f_frequenz;
int i_itemlist = 4;
float domath(float f_angle,float * f_amp_array, float * f_frequenz_array,int i_items) {
	float f_value = 0;
	for (int i = 0; i < i_items; i++){
		f_value += sinf(_P_DEGTORAD*f_angle*f_frequenz_array[i])*f_amp_array[i];
	}
	return f_value;
}
void dorendering(_CCD_ALIAS * c_render, float * f_amp_array, float * f_frequenz_array, int i_items) {
	for (float f_unit = 0,f_temp =0,f_current = 0; f_unit < _P_WIDTH/ _P_QUAL; f_unit += _P_QUULUNIT)
	{
		f_current = domath(f_unit, f_amp_array, f_frequenz_array, i_items);
		_CCD_ALIASVECTOR v_vec = { 
			f_unit*_P_QUAL,
			(_P_GRAPHHEIGHT* f_current*_P_YSCAN) + _P_YMIDD
		};
		_CCD_SETPIXEL(c_render, v_vec, _P_LINECOLOR);
	}
}
_CCD_MAIN{
	c_canvas = _CCD_CREATE(_P_WIDTH,_P_HEIGHT);
	return 0;
}
_CCD_UPDATE{
	_CCD_CLEAR(&c_canvas);
	f_amp = malloc(sizeof(float)*i_itemlist);
	f_frequenz = malloc(sizeof(float)*i_itemlist);

	f_frequenz[0] = 5;
	f_frequenz[1] = 1;
	f_frequenz[2] = 0.51;
	f_frequenz[3] = 0;

	f_amp[0] = 0.5;
	f_amp[1] = 1;
	f_amp[2] = 2;
	f_amp[3] = 0;

	dorendering(&c_canvas, f_amp, f_frequenz, i_itemlist);
	_CCD_DRAWBUFFER(&c_canvas);
return 0;
}
_CCD_END{}