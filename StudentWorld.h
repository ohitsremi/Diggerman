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
		//Populate with Dirt, Boulder, Oil,Gold
		for (size_t r = 0; r < dirt_field.size(); r++){
			for (size_t c = 0; c < dirt_field[r].size(); c++)
				if (c < 30 || c > 33)
					//dirt_field[r][c] = std::unique_ptr<Dirt>(new Dirt(r, c));
					dirt_field[r][c] = std::make_unique<Dirt>(r, c);
		}
		//How to enter in an Actor
		roster.emplace_back(std::make_shared<Oil>(std::rand() % 64, std::rand() % 64));
		//Shows total oil barrels on field
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
		digger->doSomething();
		dig(digger->getX(), digger->getY());
		for (std::shared_ptr<Actor> a : roster)
			a->doSomething();
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		//decLives();
		//return GWSTATUS_PLAYER_DIED;
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		roster.clear();
		//Free all actors
	}

	void dig(int x, int y)
	{
		int maxX = x + 3, maxY = y + 3;
		if (maxX > 64) maxX = 64;
		if (maxY > 64) maxY = 64;
		do do dirt_field[x][y].reset(); while (y < maxY); while (x < maxX);
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
	std::vector<std::shared_ptr<Actor>> roster;
	std::shared_ptr<DiggerMan> digger;
	std::array<std::array<std::unique_ptr<Dirt>, 60>, 64> dirt_field;
};

#endif // STUDENTWORLD_H_
