#pragma once
#include "olcPixelGameEngine.h"
#include "SnakeSegment.h"
#include <vector>


class Snake
{
public:
	Snake();
	~Snake();
	void ResetSnake();
	void DrawSnakeSegments(olc::PixelGameEngine* gameInstance,const olc::vi2d& vBlockSize);
	bool DetectContact(const olc::vi2d& point1, const olc::vi2d& point2, const olc::vi2d& vBlockSize);
	void DetectSelfContact(bool& isGameOver,const olc::vi2d& vBlockSize);
	void AddSnakeSegment(const olc::vi2d& vBlockSize);
	void MoveSnakeSegments();
	void ChangeDirection(const olc::vi2d& dir);
	void SetPossibleDirection();
	SnakeSegment& GetSnakeHead();
	int GetCurrentSnakeSize();
	SnakeSegment& GetCurrentSnakeTail();
	olc::vi2d GetRightDir();
	olc::vi2d GetLeftDir();


private:
	std::vector<SnakeSegment>* SnakeBody;
	olc::vi2d vSnakeHeadDir;
	olc::vi2d RightDir;
	olc::vi2d LeftDir;
};


