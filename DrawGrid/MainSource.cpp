/*
THIS APPLICATION USES THE ONELONECODER PIXEL GAME ENGINE 
THIS GAME ENGINE WAS CREATED BY A YOUTUBE USER CALLED javidx9 WHOSE CHANNEL CAN BE FOUND IN THE FOLLOWING LINK BELOW:

https://www.youtube.com/c/javidx9
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include"SnakeSegment.h"
#include "Snake.h"
//#include <vector>


class GridGame: public olc::PixelGameEngine
{
	int* GridArray;
	olc::Sprite* sprTile;
	olc::vi2d vBlockSize = { 16,16 };
	Snake* playerSnakeObject;
	olc::vi2d vTextPos = { 400, 20 };
	olc::vi2d vFoodPos;
	olc::Pixel cFoodColour;
	int CurrentScore = 0;
	bool isGameover = false;
	float currentTime = 0.0f;

public:

	GridGame()
	{
		sAppName = "SnakeGame";
	}
	~GridGame()
	{
		delete[] GridArray;
		delete playerSnakeObject;
		delete sprTile;
		std::cout << "Game over" << std::endl;
	}


public:
	void ResetLevel()
	{

		for (int y = 0; y < 15; y++)
		{
			for (int x = 0; x < 24; x++)
			{
				if (x == 0 || y == 0 || x == 23 || y == 14)
				{
					GridArray[y * 24 + x] = 5;
				}
				else
				{
					GridArray[y * 24 + x] = 0;
				}

			}
		}

		if (playerSnakeObject == nullptr)
		{
			playerSnakeObject = new Snake();
		}
		else
		{
			playerSnakeObject->ResetSnake();
		}
		CurrentScore = 0;
		cFoodColour = olc::YELLOW;
		vFoodPos = { 18,8 };
	
	}

	void DrawLevel()
	{
		for (int y = 0; y < 15; y++)
		{
			for (int x = 0; x < 24; x++)
			{
				olc::vi2d currentGridCoord = { x,y };
				if (playerSnakeObject->DetectContact(currentGridCoord * vBlockSize, playerSnakeObject->GetSnakeHead().vCurrentPos * vBlockSize, vBlockSize))
				{
					GridArray[y * 24 + x] = 10;
				}
				if (playerSnakeObject->DetectContact(currentGridCoord * vBlockSize, playerSnakeObject->GetCurrentSnakeTail().vCurrentPos * vBlockSize, vBlockSize))
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
	}
	
	bool DetectWallHits()
	{
		bool isHittingAnyWall = ((playerSnakeObject->GetSnakeHead().vCurrentPos.x * vBlockSize.x) > (23 * 16) - 16) || (playerSnakeObject->GetSnakeHead().vCurrentPos.x) < 1
			|| ((playerSnakeObject->GetSnakeHead().vCurrentPos.y * vBlockSize.y) > (14 * 16) - 16) || (playerSnakeObject->GetSnakeHead().vCurrentPos.y) < 1;

		return isHittingAnyWall;
	}

	void DrawUI()
	{
		std::string ScoreUI = "Score: " + std::to_string(CurrentScore);
		std::string SegmentNum = "Number of Segments: " + std::to_string(playerSnakeObject->GetCurrentSnakeSize());
	
		DrawString(vTextPos, ScoreUI);
		DrawString(400, 40, SegmentNum);
	}

	void GameLoopController(const float& fElapsedTime)
	{
		if (!isGameover)
		{

			if (GetKey(olc::D).bPressed)
			{
				playerSnakeObject->ChangeDirection(playerSnakeObject->GetRightDir());
			}
			if (GetKey(olc::A).bPressed)
			{
				playerSnakeObject->ChangeDirection(playerSnakeObject->GetLeftDir());
			}


			if (currentTime <= 0.2f)
			{
				currentTime += fElapsedTime;
			}
			else
			{

				currentTime = 0.0f;
				playerSnakeObject->MoveSnakeSegments();
				if (playerSnakeObject->DetectContact(playerSnakeObject->GetSnakeHead().vCurrentPos * vBlockSize, vFoodPos * vBlockSize, vBlockSize))
				{
					AddToScore();
					playerSnakeObject->AddSnakeSegment(vBlockSize);
					RepositionFood();
				}
				playerSnakeObject->DetectSelfContact(isGameover, vBlockSize);
			}
		}
	}

	void AddToScore()
	{
		CurrentScore += 1;
	}

	void RepositionFood()
	{
		olc::vi2d vNewRandomPos = { rand() % 24, rand() % 15};

		while(GridArray[vNewRandomPos.y * 24 + vNewRandomPos.x] == 10 || GridArray[vNewRandomPos.y * 24 + vNewRandomPos.x] == 5)
		{
			vNewRandomPos = { rand() % 25, rand() % 15 };
		}

		vFoodPos = vNewRandomPos;
	}
	void DrawFood()
	{
		FillRect(vFoodPos * vBlockSize, vBlockSize, cFoodColour);
	}


	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		GridArray = new int[24 * 15];
		ResetLevel();
		sprTile = new olc::Sprite("tut_tile.png");
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		std::string GameOver = "Game Over \npress Space bar to Restart";

		Clear(olc::DARK_GREEN);
		DrawLevel();

		if (DetectWallHits())
		{
			isGameover = true;
		}
		GameLoopController(fElapsedTime);
		
		DrawFood();
		DrawUI();
		playerSnakeObject->DrawSnakeSegments(this,vBlockSize);
		if (isGameover)
		{
			DrawString(6 * vBlockSize.x, 15 * vBlockSize.y, GameOver, olc::BLACK, 2);
			if (GetKey(olc::SPACE).bPressed)
			{
				ResetLevel();
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