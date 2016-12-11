#ifndef __COLOR_CONSTRAINTS_H__
#define __COLOR_CONSTRAINTS_H__

#include "Common.h"

namespace cvg
{

#define HUE	0
#define SAT	1
#define VAL	2

#define R	0
#define G	1
#define B	2

#define Y	0
#define CR	1
#define CB	2

	template<typename T, int cn>
	struct ColorConstraint_
	{
		T _min[cn];
		T _max[cn];
		inline T min(int idx)const{ return _min[idx]; }
		inline T max(int idx)const{ return _max[idx]; }
	};

	template<typename T, int cn>
	struct ColorSample_
	{
		T data[cn];
		
		inline T& operator [] (int idx)
		{
			return data[idx];
		}
		
		inline ColorSample_<T, cn> operator + (ColorSample_<T, cn>& aColorSample)
		{
			ColorSample_<T, cn> r;
			for (size_t i = 0; i < cn; i++)
			{
				r.data[i] = data[i] + aColorSample.data[i];
			}
			return r;
		}
		inline ColorSample_<T, cn> operator - (ColorSample_<T, cn>& aColorSample)
		{
			ColorSample_<T, cn> r;
			for (size_t i = 0; i < cn; i++)
			{
				r.data[i] = data[i] - aColorSample.data[i];
			}
			return r;
		}
		inline ColorSample_<T, cn> operator / (T factor)
		{
			ColorSample_<T, cn> r;
			for (size_t i = 0; i < cn; i++)
			{
				r.data[i] = data[i] / factor;
			}
			return r;
		}	
		inline ColorSample_<T, cn> operator * (T factor)
		{
			ColorSample_<T, cn> r;
			for (size_t i = 0; i < cn; i++)
			{
				r.data[i] = data[i] * factor;
			}
			return r;
		}
	};

	typedef ColorConstraint_<int, 3> ColorConstraint3i;
	typedef ColorConstraint3i HSVColorConstraint;

	typedef ColorSample_<int, 3> ColorSample3i;
	typedef ColorSample3i HSVColorSample;

	void InitColorConstraintHSV(HSVColorConstraint&);
}
#endif	// __COLOR_CONSTRAINTS_H__