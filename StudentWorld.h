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

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}
	StudentWorld() :GameWorld("") {}
	virtual int init()
	{
		digger = std::make_shared<DiggerMan>();
		for (size_t x = 0; x < dirt_field.size(); x++)
			for (size_t y = 0; y < dirt_field[x].size(); y++)
				if (y < 8 || (x < 30 || x > 33) && y < 60) // modified to match his sample
					dirt_field[x][y] = std::make_unique<Dirt>(x, y);
		for (int i = 0; i < std::min((int)getLevel() / 2 + 2, 7); i++)
		{
			int x = std::rand() % 60;
			int y = std::rand() % 36 + 20;	//Between 20 and 56
			if (isSafeDistanceAway(x, y)) {
				roster.emplace_back(std::make_shared<Boulder>(x, y));
				for(int i=0; i<4; i++)
					for(int j=0; j<4; j++)
					dirt_field[x+i][y+j].reset();
			}
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
		oilCount = count_if(roster.begin(), roster.end(), [](std::shared_ptr<Actor> a)
		{ return std::dynamic_pointer_cast<Oil>(a) != nullptr; }
		);

		setGameStatText(getStats(oilCount));
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		std::ostringstream display;
		display << "Lvl: " << getLevel() << " Lives: " << getLives() << " HP: " << digger->getHealth()
			<< " Wtr: " << digger->getWater() << " Gld: " << digger->getGold()
			<< " Sonar: " << digger->getSonar() << " Oil Left :" << oilCount << " Scr: " << getScore();

		setGameStatText(display.str());

		for (std::shared_ptr<Actor> a : roster) {
			a->doSomething(this);
		}
		digger->move(this);
		dig(digger->getX(), digger->getY());

		//decLives();
		//return GWSTATUS_PLAYER_DIED;
		if (oilCount == 0)
			return GWSTATUS_FINISHED_LEVEL;
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		roster.clear();
		for (size_t r = 0; r < dirt_field.size(); r++)
			for (size_t c = 0; c < dirt_field[r].size(); c++)
				dirt_field[r][c].reset();
		digger.reset();
	}

	void dig(int minX, int minY)
	{
		for (int d_x = minX; d_x < minX + 4; d_x++)
			for (int d_y = minY; d_y < minY + 4; d_y++)
				if (d_y < dirt_field[d_x].size())
					dirt_field[d_x][d_y].reset();
	}

	void decreaseOil()
	{
		oilCount--;
	}

	bool isSafeDistanceAway(int x, int y)
	{
		for (std::shared_ptr<Actor> a : roster)
			if (pow(abs(a->getX() - x), 2) + pow(abs(a->getY() - y), 2) <= 36 || !( y < 8 || (x < 26 || x > 33)))
				return false;
		return true;
	}

	//Opposite of isSafeDistanceAway, takes in a custom radius for now
	bool isWithinDistance(Actor * a, int rad)
	{
		return	pow(abs(a->getX() - digger->getX()), 2) + pow(abs(a->getY() - digger->getY()), 2) <= pow(rad, 2);
	}

	std::string getStats(const int & oil)
	{
		std::ostringstream display;
		display << "Lvl: " << getLevel() << " Lives: " << getLives() << " HP: " << digger->getHealth()
			<< " Wtr: " << digger->getWater() << " Gld: " << digger->getGold()
			<< " Sonar: " << digger->getSonar() << " Oil Left :" << oil << "Scr: " << getScore();
		return display.str();
	}

	void addProjectile(int x, int y, GraphObject::Direction dir)
	{
		if (dir == GraphObject::right)
			x += 4;
		else if (dir == GraphObject::left)
			x -= 4;
		else if (dir == GraphObject::up)
			y += 4;
		else if (dir == GraphObject::down)
			y -= 4;
		roster.emplace_back(std::make_shared<Projectile>(x, y, dir));
	}

	bool doesCollide(int x, int y, GraphObject::Direction d)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				if (d == GraphObject::right)
					if (dirt_field[x + i][y])
						return true;
				else if (d == GraphObject::left)
					if (dirt_field[x - i][y])
						return true;
				else if (d == GraphObject::up)
					if (dirt_field[x][y + j])
						return true;
				else if (d == GraphObject::down)
					if (dirt_field[x][y - j])
						return true;
			 }
		return false;
	}
private:
	std::vector<std::shared_ptr<Actor>> roster;	//maybe also have a 2d array of actor pointers
	std::shared_ptr<DiggerMan> digger;
	std::array<std::array<std::unique_ptr<Dirt>, 64>, 64> dirt_field;  // modified to match his sample
	int oilCount;
};

#endif // STUDENTWORLD_H_
