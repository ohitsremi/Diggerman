#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <array>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <algorithm>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}

	virtual int init()
	{
		digger = std::make_shared<DiggerMan>();

		for (size_t x = 0; x < dirt_field.size(); x++)
			for (size_t y = 0; y < dirt_field[x].size(); y++)
				if (y < 4 || (x < 30 || x > 33))
					dirt_field[x][y] = std::make_unique<Dirt>(x, y);

		for (int i = 0; i < std::min((int)getLevel() / 2 + 2, 7); i++)
		{
			int x = std::rand() % 60;
			int y = std::rand() % 36 + 20;	//Between 20 and 56
			if (isSafeDistanceAway(x, y))
				roster.emplace_back(std::make_shared<Boulder>(x, y));
			else
				i--;
		}
		for (int i = 0; i < std::max(5 - ((int)getLevel() / 2), 2); i++)
		{
			int x = std::rand() % 60;
			int y = std::rand() % 56;
			if (isSafeDistanceAway(x, y))
				roster.emplace_back(std::make_shared<Gold>(x, y));
			else
				i--;
		}
		for (int i = 0; i < std::min(2 + (int)getLevel(), 18); i++)
		{
			int x = std::rand() % 60;
			int y = std::rand() % 56;
			if (isSafeDistanceAway(x, y))
				roster.emplace_back(std::make_shared<Oil>(x, y));
			else
				i--;
		}
		int oilCount = count_if(roster.begin(), roster.end(), [](std::shared_ptr<Actor> a)
			{ return std::dynamic_pointer_cast<Oil>(a) != nullptr; }
		);

		setGameStatText(getStats(oilCount));
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		int oilCount = count_if(roster.begin(), roster.end(), [](std::shared_ptr<Actor> a)
		{ return std::dynamic_pointer_cast<Oil>(a) != nullptr; }
		);

		std::ostringstream display;
		display << "Lvl: " << getLevel() << " Lives: " << getLives() << " HP: " << digger->getHealth()
			<< " Wtr: " << digger->getWater() << " Gld: " << digger->getGold()
			<< " Sonar: " << digger->getSonar() << " Oil Left :" << oilCount << " Scr: " << getScore();

		setGameStatText(display.str());

		for (std::shared_ptr<Actor> a : roster)
			a->doSomething();
		//decLives();
		//return GWSTATUS_PLAYER_DIED;
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		roster.clear();
		for (size_t r = 0; r < dirt_field.size(); r++)
			for (size_t c = 0; c < dirt_field[r].size(); c++)
				dirt_field[r][c].reset();
		//Do something with DiggerMan
	}

	void dig(int x, int y)
	{
		int maxX = x + 3, maxY = y + 3;
		if (maxX > 64) maxX = 64;
		if (maxY > 64) maxY = 64;
		do do dirt_field[x][y].reset(); while (y < maxY); while (x < maxX);
	}

	bool isSafeDistanceAway(int x, int y)
	{
		for (std::shared_ptr<Actor> a : roster)
			if(pow(abs(a->getX() - x), 2) + pow(abs(a->getY() - y), 2) <= 36)
				return false;
		return true;
	}

	std::string getStats(const int & oil)
	{
		std::ostringstream display;
		display << "Lvl: " << getLevel() << " Lives: " << getLives() << " HP: " << digger->getHealth()
			<< " Wtr: " << digger->getWater() << " Gld: " << digger->getGold()
			<< " Sonar: " << digger->getSonar() << " Oil Left :" << oil << "Scr: " << getScore();
		return display.str();
	}

private:
	std::vector<std::shared_ptr<Actor>> roster;	//maybe also have a 2d array of actor pointers
	std::shared_ptr<DiggerMan> digger;
	std::array<std::array<std::unique_ptr<Dirt>, 64>, 60> dirt_field;
};

#endif // STUDENTWORLD_H_
