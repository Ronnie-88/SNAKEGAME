#include "Snake.h"

Snake::Snake()
{
	ResetSnake();
}

Snake::~Snake()
{
	delete SnakeBody;
}

void Snake::ResetSnake()
{
	SnakeBody->clear();
	SnakeBody->emplace_back(olc::vi2d{ 12,7 });//head
	SnakeBody->emplace_back(olc::vi2d{ 11,7 });//body
	SnakeBody->emplace_back(olc::vi2d{ 10,7 });//tail
	ChangeDirection({ 1,0 });
}

void Snake::DrawSnakeSegments(olc::PixelGameEngine* gameInstance, const olc::vi2d& vBlockSize)
{
	for (unsigned int i = 0; i < snakeBodyRef.size(); i++)
	{
		if (i == 0)//the head must drawn differently
		{
			gameInstance->FillRect(snakeBodyRef[i].vCurrentPos * vBlockSize, vBlockSize, olc::CYAN);
		}
		else
		{
			gameInstance->FillRect(snakeBodyRef[i].vCurrentPos * vBlockSize, vBlockSize, olc::DARK_RED);
		}
	}
	
}

bool Snake::DetectContact(const olc::vi2d& point1, const olc::vi2d& point2, const olc::vi2d& vBlockSize)
{
	return (point1.x >= point2.x && point1.y >= point2.y && point1.x < (point2.x + vBlockSize.x) && point1.y < (point2.y + vBlockSize.y));
}

void Snake::DetectSelfContact(bool& isGameOver, const olc::vi2d& vBlockSize)
{
	for (int i = 0; i < snakeBodyRef.size(); i++)
	{
		if (i==0)//the head should not detect itself
		{
			continue;
		}
		if (DetectContact(GetSnakeHead().vCurrentPos * vBlockSize, snakeBodyRef[i].vCurrentPos * vBlockSize, vBlockSize))
		{
			isGameOver = true;
		}
	}
}

void Snake::AddSnakeSegment(const olc::vi2d& vBlockSize)
{
	const SnakeSegment lastSegment = GetCurrentSnakeTail();
	const olc::vi2d vNextSegmentPos = (lastSegment.vLastPos * vBlockSize) - (vBlockSize * vSnakeHeadDir);
	SnakeBody->emplace_back(vNextSegmentPos);
}

void Snake::MoveSnakeSegments()
{
	for (int i = 0; i < snakeBodyRef.size(); i++)
	{
		if (i==0)//the head only needs to change it's lastpos
		{
			snakeBodyRef[i].vLastPos = snakeBodyRef[i].vCurrentPos;
		}
		else//the head has to move at least one step forward to move the rest of the body 
		{
			if (i == 1) 
			{
				snakeBodyRef[i].vLastPos = snakeBodyRef[i].vCurrentPos;
				snakeBodyRef[i].vCurrentPos = GetSnakeHead().vLastPos;
			}
			else
			{
				snakeBodyRef[i].vLastPos = snakeBodyRef[i].vCurrentPos;
				snakeBodyRef[i].vCurrentPos = snakeBodyRef[i - 1].vLastPos;
			}
		}
		
	}
	GetSnakeHead().vCurrentPos += vSnakeHeadDir;
}

void Snake::ChangeDirection(const olc::vi2d& dir)
{
	vSnakeHeadDir = dir;
	SetPossibleDirection();
}

void Snake::SetPossibleDirection()
{
	if (vSnakeHeadDir.x == 1 && vSnakeHeadDir.y == 0)//west
	{
		LeftDir = { 0, -1 };
		RightDir = { 0, 1 };
	}
	else if (vSnakeHeadDir.x == 0 && vSnakeHeadDir.y == 1)//south
	{
		LeftDir = { 1, 0 };
		RightDir = { -1, 0 };
	}
	else if (vSnakeHeadDir.x == -1 && vSnakeHeadDir.y == 0)//east
	{
		LeftDir = { 0, 1 };
		RightDir = { 0, -1 };
	}
	else if (vSnakeHeadDir.x == 0 && vSnakeHeadDir.y == -1)//north
	{
		LeftDir = { -1, 0 };
		RightDir = { 1, 0 };
	}
}

SnakeSegment& Snake::GetSnakeHead()
{
	return snakeBodyRef[0];
}

int Snake::GetCurrentSnakeSize()
{
	return SnakeBody->size();
}

SnakeSegment& Snake::GetCurrentSnakeTail()
{
	return snakeBodyRef[SnakeBody->size()-1];
}

olc::vi2d Snake::GetRightDir()
{
	return RightDir;
}

olc::vi2d Snake::GetLeftDir()
{
	return LeftDir;
}
