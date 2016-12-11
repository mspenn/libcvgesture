#include "ROI.h"

namespace cvg
{
	ROI::ROI(Point lower, Point upper)
	{
		_bot_corner = lower;
		_top_corner = upper;

		_width = _top_corner.x - _bot_corner.x;
		_height = _top_corner.y - _bot_corner.y;

		_region = Rect(_bot_corner.x, _bot_corner.y, _width, _height);
	}
}