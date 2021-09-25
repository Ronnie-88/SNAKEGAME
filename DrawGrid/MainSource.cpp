/*
THIS APPLICATION USES THE ONELONECODER PIXEL GAME ENGINE 
THIS GAME ENGINE WAS CREATED BY A YOUTUBE USER CALLED javidx9 WHOSE CHANNEL CAN BE FOUND IN THE FOLLOWING LINK BELOW:

https://www.youtube.com/c/javidx9
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>


class GridGame: public olc::PixelGameEngine
{
private:
	struct SnakeSegment
	{
		olc::vi2d vLastPos;
		olc::vi2d vCurrentPos;

		SnakeSegment(const olc::vi2d& currpos)
		{
			vCurrentPos = currpos;
		}
	};

	int* GridArray;
	std::vector<SnakeSegment> SnakeBody;
	SnakeSegment* snakeHead;
	olc::Sprite* sprTile;
	olc::vi2d vBlockSize = { 16,16 };
	olc::vi2d vBoxPos;
	olc::vi2d vSnakeHeadDir = { 0,0 };
	olc::vi2d LeftDir = { 0,0 };
	olc::vi2d RightDir = { 0,0 };
	olc::vi2d vTextPos = { 400, 20 };
	olc::vi2d vFoodPos;
	olc::Pixel cFoodColour;
	int CurrentScore = 0;
	bool isGameover = false;

	float currentTime = 0.0f;
	int snakeSize = 3;


public:
	GridGame()
	{
		sAppName = "GridGame";
	}
	~GridGame()
	{
		delete[] GridArray;
		delete sprTile;
		delete snakeHead;
		std::cout << "Game over" << std::endl;
	}


public:
	void SetPossibleDirection()
	{
		if (vSnakeHeadDir.x == 1 && vSnakeHeadDir.y == 0)//west
		{
			LeftDir = {	0, -1 };
			RightDir ={	0, 1 };
		}
		else if(vSnakeHeadDir.x == 0 && vSnakeHeadDir.y == 1)//south
		{
			LeftDir = { 1, 0 };
			RightDir = { -1, 0 };
		}
		else if(vSnakeHeadDir.x == -1 && vSnakeHeadDir.y == 0)//east
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

	void Reset()
	{

		for (int y = 0; y < 30; y++)
		{
			for (int x = 0; x < 24; x++)
			{
				if (x == 0 || y == 0 || x == 23 || y == 29)
				{
					GridArray[y * 24 + x] = 5;
				}
				else
				{
					GridArray[y * 24 + x] = 0;
				}

			}
		}

		if (snakeHead != nullptr)
		{
			delete snakeHead;
			SnakeBody.clear();
		}
		CurrentScore = 0;
		snakeHead = new SnakeSegment({ 12,15 });
		SnakeBody.emplace_back(SnakeSegment({ 11,15 }));
		SnakeBody.emplace_back(SnakeSegment({ 10,15 }));
		cFoodColour = olc::YELLOW;
		vFoodPos = { 18,8 };
		ChangeDirection({ 1,0 });
	}

	void DrawHead()
	{
		FillRect(snakeHead->vCurrentPos * vBlockSize, vBlockSize, olc::CYAN);
	}

	void AddToScore()
	{
		CurrentScore += 1;
	}

	bool DetectContact(const olc::vi2d& point1, const olc::vi2d& point2)
	{
		return (point1.x >= point2.x && point1.y >= point2.y && point1.x < (point2.x + vBlockSize.x) && point1.y < (point2.y + vBlockSize.y));
	}
	
	void RepositionFood()
	{
		olc::vi2d vNewRandomPos = { rand() % 25, rand() % 31};

		if (GridArray[vNewRandomPos.y * 24 + vNewRandomPos.x] == 10 || GridArray[vNewRandomPos.y * 24 + vNewRandomPos.x] == 5)
		{
			vNewRandomPos = { rand() % 25, rand() % 31 };
		}

		vFoodPos = vNewRandomPos;
	}
	void DrawFood()
	{
		FillRect(vFoodPos * vBlockSize, vBlockSize, cFoodColour);
	}

	void DrawSnakeSegments()
	{
		for (int i = 0; i < SnakeBody.size(); i++)
		{
			FillRect(SnakeBody[i].vCurrentPos * vBlockSize, vBlockSize, olc::DARK_RED);
		}
	}

	void AddSnakeSegment()
	{
		const SnakeSegment lastSegment = SnakeBody[SnakeBody.size() - 1];
		const olc::vi2d vNextSegmentPos = (lastSegment.vLastPos * vBlockSize) - (vBlockSize * vSnakeHeadDir);
		SnakeBody.emplace_back(SnakeSegment(vNextSegmentPos));
	}
	void MoveSnakeSegments()
	{
		for (int i = 0; i < SnakeBody.size(); i++)
		{
			if (i==0)
			{
				SnakeBody[i].vLastPos = SnakeBody[i].vCurrentPos;
				SnakeBody[i].vCurrentPos = snakeHead->vLastPos;
			}
			else
			{
				SnakeBody[i].vLastPos = SnakeBody[i].vCurrentPos;
				SnakeBody[i].vCurrentPos = SnakeBody[i - 1].vLastPos;
			}
		}
	}
	void ChangeDirection(const olc::vi2d& dir)
	{
		vSnakeHeadDir = dir;
		SetPossibleDirection();
	}
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		GridArray = new int[24 * 30];
		Reset();
		sprTile = new olc::Sprite("tut_tile.png");
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		std::string GameOver = "Game Over \npress Space bar to Restart";

		Clear(olc::DARK_GREEN);
		for (int y = 0; y < 30; y++)
		{
			for (int x = 0; x < 24; x++)
			{
				olc::vi2d currentGridCoord = { x,y };
				if (DetectContact(currentGridCoord * vBlockSize, snakeHead->vCurrentPos * vBlockSize))
				{
					GridArray[y * 24 + x] = 10;
				}
				if (DetectContact(currentGridCoord * vBlockSize, SnakeBody[SnakeBody.size() - 1].vCurrentPos * vBlockSize))
				{
					GridArray[y * 24 + x] = 0;
				}

				if (GridArray[y * 24 + x] == 0)
				{
					DrawSprite(olc::vi2d(x, y) * vBlockSize, sprTile);
				}
				if (GridArray[y * 24 + x] == 5)
				{
					FillRect(olc::vi2d(x, y) * vBlockSize, vBlockSize, olc::DARK_MAGENTA);
				}
				
			}
		}

		
		if (((snakeHead->vCurrentPos.x * vBlockSize.x) > (23 * 16) - 16) || (snakeHead->vCurrentPos.x) < 1
			|| ((snakeHead->vCurrentPos.y * vBlockSize.y) > (29 * 16) - 16) || (snakeHead->vCurrentPos.y) < 1)
		{
			isGameover = true;
		}

		

		if (!isGameover)
		{

			if (GetKey(olc::D).bPressed)
			{
				ChangeDirection(RightDir);
			}
			if (GetKey(olc::A).bPressed)
			{
				ChangeDirection(LeftDir);
			}


			if (currentTime <= 0.2f)
			{
				currentTime += fElapsedTime;
			}
			else
			{

				currentTime = 0.0f;
				snakeHead->vLastPos = snakeHead->vCurrentPos;
				MoveSnakeSegments();
				snakeHead->vCurrentPos += vSnakeHeadDir;
				if (DetectContact(snakeHead->vCurrentPos * vBlockSize, vFoodPos * vBlockSize))
				{
					AddToScore();
					AddSnakeSegment();
					RepositionFood();
				}

				for (int i = 0; i < SnakeBody.size(); i++)
				{
					if (DetectContact(snakeHead->vCurrentPos * vBlockSize, SnakeBody[i].vCurrentPos*vBlockSize))
					{
						isGameover = true;
					}
				}

			}
		}
		
		std::string ScoreUI = "Score: "+ std::to_string(CurrentScore);
		std::string SegmentNum = "Number of Segments: " + std::to_string(SnakeBody.size());
		
		
		DrawHead();
		DrawFood();
		DrawSnakeSegments();

		DrawString(vTextPos, ScoreUI);
		DrawString(400, 40, SegmentNum);

		if (isGameover)
		{
			DrawString(6 * vBlockSize.x, 15 * vBlockSize.y, GameOver, olc::BLACK, 2);
			if (GetKey(olc::SPACE).bPressed)
			{
				Reset();
				isGameover = false;
			}
		}
		return true;
	}

};

int main()
{
	GridGame demo;
	if (demo.Construct(612, 480, 2, 2))
		demo.Start();

	return 0;
}