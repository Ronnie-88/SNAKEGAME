#pragma once
#include "olcPixelGameEngine.h"

struct SnakeSegment
{
	olc::vi2d vLastPos;
	olc::vi2d vCurrentPos;

	SnakeSegment(const olc::vi2d& currpos);
	SnakeSegment(const SnakeSegment& snakeSeg);
};
