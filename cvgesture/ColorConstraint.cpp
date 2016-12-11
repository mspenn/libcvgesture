#include "ColorConstraint.h"

namespace cvg
{
	void InitColorConstraintHSV(HSVColorConstraint& hsvColorConstraint)
	{
		hsvColorConstraint._min[HUE] = 12;
		hsvColorConstraint._max[HUE] = 7;
		hsvColorConstraint._min[SAT] = 30;
		hsvColorConstraint._max[SAT] = 30;
		hsvColorConstraint._min[VAL] = 50;
		hsvColorConstraint._max[VAL] = 50;
	}
}