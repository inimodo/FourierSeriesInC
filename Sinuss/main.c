#include <math.h>
#include "CCD.h"

#define _P_SIZE 80
#define _P_WIDTH (16*_P_SIZE)
#define _P_HEIGHT (9*_P_SIZE) 

#define _P_YMIDD (float)((float)_P_HEIGHT/(float)2)
#define _P_XMIDD (float)((float)_P_WIDTH/(float)2)

#define _P_GRAPHHEIGHT (float)((float)_P_HEIGHT/(float)10)

#define _P_DEGTORAD  (float)(3.14159265359f/180.f)

#define _P_YSCAN 0.5f
#define _P_XSCAN 0.5f

#define _P_QUULUNIT 1

#define _P_OUTCOLOR (_CCD_ALIASCOLOR){255,255,255}
#define _P_CIRCLECOLOR (_CCD_ALIASCOLOR){255,0,255}
#define _P_LINECOLOR (_CCD_ALIASCOLOR){255,255,0}

#define _P_SCROLLSPEED 5
#define _P_CIRCLEQUALITY 30

#define _P_MIDDLEPOS (_CCD_ALIASVECTOR){_P_WIDTH/2, _P_HEIGHT/2}

_CCD_ALIAS c_canvas;
float * f_amp, * f_frequenz;
int i_itemlist = 10,i_angle = 0;


float getsinglemath(float f_angle, float f_amp_array, float f_frequenz_array) {
 return sin(_P_DEGTORAD*((f_angle ) *f_frequenz_array))*f_amp_array;
}

float domath(float f_angle,float * f_amp_array, float * f_frequenz_array,int i_items) {
	float f_value = 0;
	for (int i = 0; i < i_items; i++){
		f_value += getsinglemath(f_angle , f_amp_array[i], f_frequenz_array[i]);
	}
	return f_value;
}

_CCD_ALIASVECTOR getanglepos(_CCD_DATA_DEF i_angle, _CCD_DATA_DEF i_distance, _CCD_ALIASVECTOR v_anchor) {
	_CCD_ALIASVECTOR v_pos;
	v_pos.X = cos(_P_DEGTORAD*i_angle)*i_distance + v_anchor.X;
	v_pos.Y = sin(_P_DEGTORAD*i_angle)*i_distance + v_anchor.Y;
	return v_pos;
}

void drawcircle(_CCD_ALIAS * c_render, _CCD_DATA_DEF i_distance, _CCD_ALIASVECTOR v_anchor, _CCD_ALIASCOLOR c_color) {
	_CCD_ALIASVECTOR v_last, v_first, v_current;
	for (_CCD_DATA_DEF i = 0; i < 360; i += 360 / _P_CIRCLEQUALITY)
	{
		v_current = getanglepos(i, i_distance, v_anchor);
		if (i == 0) {
			v_last = v_current;
			v_first = v_current;
		}
		_CCD_DRAWLINE(c_render, v_current, v_last, c_color);
		v_last = v_current;
	}
	_CCD_DRAWLINE(c_render, v_current, v_first, c_color);

}

void dorendering(_CCD_ALIAS * c_render, float * f_amp_array, float * f_frequenz_array, int i_items) {
	_CCD_ALIASVECTOR v_vec, v_first,v_last;
	_CCD_DATA_FLOAT f_current;

	for (int f_unit = 0; f_unit < _P_XMIDD / _P_XSCAN; f_unit += _P_QUULUNIT)
	{
		v_last = _P_MIDDLEPOS;

		for (int i = 0; i < i_items; i++) {
			v_vec = getanglepos((f_unit-i_angle) * f_frequenz_array[i], f_amp_array[i] * _P_GRAPHHEIGHT*_P_YSCAN, v_last);
			v_last = v_vec;
		}
		v_vec.X = f_unit * _P_XSCAN + _P_XMIDD;
		_CCD_SETPIXEL(c_render, v_vec, _P_OUTCOLOR);
	}


	v_last = (_CCD_ALIASVECTOR) { _P_XMIDD/2, _P_YMIDD};


	for (int i = 0; i < i_items; i++) {

		v_vec = getanglepos(0-i_angle* f_frequenz_array[i], f_amp_array[i]* _P_GRAPHHEIGHT*_P_YSCAN, v_last);

		_CCD_DRAWLINE(c_render, v_last, v_vec, _P_LINECOLOR);
		drawcircle(c_render, f_amp_array[i] * _P_GRAPHHEIGHT*_P_YSCAN, v_last, _P_CIRCLECOLOR);

		v_last = v_vec;
	}

	_CCD_DRAWLINE(c_render, v_vec, (_CCD_ALIASVECTOR) { _P_XMIDD, v_vec.Y }, _P_LINECOLOR);

}


_CCD_MAIN{
	c_canvas = _CCD_CREATE(_P_WIDTH,_P_HEIGHT);

	f_amp = malloc(sizeof(float)*i_itemlist);
	f_frequenz = malloc(sizeof(float)*i_itemlist);


	for (int i = 0; i < i_itemlist; i++)
	{
		f_frequenz[i] = i+1;
		f_amp[i] = (i_itemlist-i)/ (float)i_itemlist;


	}
	//f_frequenz[0] = 1.5f;
	//f_frequenz[1] = 2;
	//f_frequenz[2] = 1;
	//f_frequenz[3] = 2.5f;
	//f_frequenz[4] = 1;


	//f_amp[0] = 1;
	//f_amp[1] = 1;
	//f_amp[2] = 1;
	//f_amp[3] = 1;
	//f_amp[4] = 1;

	return 0;
}
_CCD_UPDATE{
	_CCD_CLEAR(&c_canvas);

    i_angle += _P_SCROLLSPEED;
	printf("%d\n",i_angle);

	dorendering(&c_canvas, f_amp, f_frequenz, i_itemlist);

	_CCD_DRAWBUFFER(&c_canvas);
	return 0;
}
_CCD_END{}


/*
//for (float f_unit = 0; f_unit < _P_XMIDD / _P_XSCAN; f_unit += _P_QUULUNIT)
//{
//	f_current = domath(f_unit-i_angle, f_amp_array, f_frequenz_array, i_items);
//	v_vec = (_CCD_ALIASVECTOR){
//		f_unit*_P_XSCAN+ _P_XMIDD,
//		(_P_GRAPHHEIGHT* f_current*_P_YSCAN) + _P_YMIDD
//	};
//	if (f_unit == 0)v_first = v_vec;
//	_CCD_SETPIXEL(c_render, v_vec, _P_LINECOLOR);
//}
//_CCD_DRAWLINE(c_render, v_first, (_CCD_ALIASVECTOR) { 0, v_first.Y }, _P_LINECOLOR);
*/