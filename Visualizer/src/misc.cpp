#include <misc.h>

bool PointWithinRect(const Rectangle& rect, int x, int y)
{
	return x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height;
}
