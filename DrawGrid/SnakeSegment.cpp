
#include "SnakeSegment.h"

SnakeSegment::SnakeSegment(const olc::vi2d& currpos)
{
	vCurrentPos = currpos;
}
SnakeSegment::SnakeSegment(const SnakeSegment& snakeSeg)
	:vLastPos(snakeSeg.vLastPos), vCurrentPos(snakeSeg.vCurrentPos)
{
}