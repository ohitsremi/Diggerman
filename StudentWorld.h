#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <array>
#include <string>
#include <memory>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}
	StudentWorld() :GameWorld("") {}
	virtual int init()
	{
		//Populate game with Dirt
		digger = std::make_shared<DiggerMan>();
		for (size_t x = 0; x < dirt_field.size(); x++)
			for (size_t y = 0; y < dirt_field[x].size(); y++)
				if (y < 8 || (x < 30 || x > 33) && y < 60) // modified to match his sample
					dirt_field[x][y] = std::make_unique<Dirt>(x, y);
		//Populate game with boulders and clear Dirt objects in the way
		for (int i = 0; i < std::min((int)getLevel() / 2 + 2, 7); i++)
		{
			int x = std::rand() % 60;
			int y = std::rand() % 36 + 20;	//Between 20 and 56
			if (isSafeDistanceAway(x, y))
			{
				roster.emplace_back(std::make_shared<Boulder>(x, y));
				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++)
						dirt_field[x + i][y + j].reset();
			}
			else
				i--;
		}
		//Populate game with Gold which Diggerman can pick up
		for (int i = 0; i < std::max(5 - ((int)getLevel() / 2), 2); i++)
		{
			int x = std::rand() % 60;
			int y = std::rand() % 56;
			if (isSafeDistanceAway(x, y))
				roster.emplace_back(std::make_shared<Gold>(x, y, Gold::pickup));
			else
				i--;
		}
		//Populate game with Oil Barrels
		for (int i = 0; i < std::min(2 + (int)getLevel(), 18); i++)
		{
			int x = std::rand() % 60;
			int y = std::rand() % 56;
			if (isSafeDistanceAway(x, y))
				roster.emplace_back(std::make_shared<Oil>(x, y));
			else
				i--;
		}
		//Get initial count of boulders as calculated above
		oilCount = count_if(roster.begin(), roster.end(), [](std::shared_ptr<Actor> a)
		{ return std::dynamic_pointer_cast<Oil>(a) != nullptr; }
		);

		setGameStatText(getStats(oilCount));
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		//Format and display game text
		std::ostringstream display;
		display << std::setiosflags(std::ios::right);
		display << "Lvl: " << std::setw(2) << getLevel() << "  Lives: " << std::setw(1) << getLives()
			<< "  Hlth: " << std::setw(3) << digger->getHealth() * 10 << "%  Wtr: " << std::setw(2) << digger->getWater()
			<< "  Gld: " << std::setw(2) << digger->getGold() << "  Sonar: " << std::setw(2) << digger->getSonar()
			<< "  Oil Left :" << std::setw(2) << oilCount << "  Scr: " << std::setw(6) << std::setfill('0') << getScore();
		setGameStatText(display.str());
		//Tell actors to do something if they are alive
		for (std::shared_ptr<Actor> a : roster)
		{
			if (a->isAlive())
				a->doSomething(this);
			if (!digger->isAlive())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
		}
		digger->doSomething(this);
		dig(digger->getX(), digger->getY());
		//T is ticks passed since protester was added. P is maximum number of protesters allowed
		int T = std::max(25, 200 - (int)getLevel());
		int P = std::min(15, 2 + (int)(getLevel() * 1.5));
		int probabilityOfHardcore = std::min(90, (int)getLevel() * 10 + 30);
		//Adds a protester as long as there is enough room and time has passed between them spawning in
		if (pRoster.size() < P)
		{
			if (ticks > T || pRoster.size() == 0)
			{
				if (rand() % 100 < probabilityOfHardcore)
				{
					auto p = std::make_shared<HardcoreProtester>(60, 60);
					roster.emplace_back(p);
					pRoster.emplace_back(p);
					ticks = 0;
				}
				else
				{
					auto p = std::make_shared<Protester>(60, 60);
					roster.emplace_back(p);
					pRoster.emplace_back(p);
					ticks = 0;
				}
			}
			ticks++;
		}
		//Spawn in either a Water or Sonar
		int G = getLevel() * 25 + 300;
		int Gchance = std::rand() % G;
		if (rand() % G < 1)
		{
			if (rand() % 5 < 4)
			{
				while (true)
				{
					int x = std::rand() % 60;
					int y = std::rand() % 56;
					if (!doesCollide(x, y))
					{
						roster.emplace_back(std::make_shared<Water>(x, y));
						break;
					}
				}
			}
			else
				roster.emplace_back(std::make_shared<Sonar>(0, 60));
		}

		removeDeadGameObjects();

		if (oilCount == 0)
			return GWSTATUS_FINISHED_LEVEL;
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		roster.clear();
		pRoster.clear();
		for (size_t x = 0; x < dirt_field.size(); x++)
			for (size_t y = 0; y < dirt_field[x].size(); y++)
				dirt_field[x][y].reset();
		digger.reset();
	}

	void dig(size_t minX, size_t minY)
	{
		for (size_t d_x = minX; d_x < minX + 4; d_x++)
			for (size_t d_y = minY; d_y < minY + 4; d_y++)
				if (d_y < dirt_field[d_x].size() && dirt_field[d_x][d_y])
				{
					dirt_field[d_x][d_y].reset();
					playSound(SOUND_DIG);
				}
	}

	void decreaseOil()
	{
		oilCount--;
	}

	bool isSafeDistanceAway(int x, int y)
	{
		for (std::shared_ptr<Actor> a : roster)
			if (pow(abs(a->getX() - x), 2) + pow(abs(a->getY() - y), 2) <= 36 || !(y < 8 || (x < 26 || x > 33)))
				return false;
		return true;
	}

	bool isValid(int x, int y)
	{
		return x >= 0 && x < 64 && y >= 0 && y < 64;
	}

	//Opposite of isSafeDistanceAway, takes in a custom radius for now
	bool isWithinDistance(Actor * a, int rad)
	{
		return pow(abs(a->getX() - digger->getX()), 2) + pow(abs(a->getY() - digger->getY()), 2) <= pow(rad, 2);
	}

	bool isWithinDistanceOfProtester(Actor * a, int rad)
	{
		int v = 0;
		for (std::shared_ptr<Protester> p : pRoster)
		{
			if (pow(abs(a->getX() - p->getX()), 2) + pow(abs(a->getY() - p->getY()), 2) <= pow(rad, 2))
			{
				v++;
			}
		}
		if (v > 0)
			return true;
		else
			return false;
	}

	bool isShootingProtester(Actor * a, int rad)
	{
		int v = 0;
		for (std::shared_ptr<Protester> p : pRoster)
		{
			if (pow(abs(a->getX() - p->getX()), 2) + pow(abs(a->getY() - p->getY()), 2) <= pow(rad, 2))
			{
				decPHealth(p.get());
				v++;
			}
		}
		if (v > 0)
			return true;
		else
			return false;
	}

	bool isFallingOnProtester(Actor * b)
	{
		int v = 0;
		for (std::shared_ptr<Protester> p : pRoster)
		{
			if (pow(abs(b->getX() - p->getX()), 2) + pow(abs(b->getY() - p->getY()), 2) <= 4)
			{
				protesterSetLeave(p.get());
				v++;
			}
		}
		if (v > 0)
			return true;
		else
			return false;
	}

	bool diggermanAhead(Actor * a, int x, int y)
	{
		int dx = digger->getX(), dy = digger->getY();
		if (x < dx && y == dy && !doesCollide(x + 1, y))
		{
			a->setDirection(GraphObject::right);
			return true;
		}
		if (x > dx && y == dy && !doesCollide(x - 1, y))
		{
			a->setDirection(GraphObject::left);
			return true;
		}
		if (x == dx && y > dy && !doesCollide(x, y - 1))
		{
			a->setDirection(GraphObject::down);
			return true;
		}
		if (x == dx && y < dy && !doesCollide(x, y + 1))
		{
			a->setDirection(GraphObject::up);
			return true;
		}
		return false;
	}

	void diggerAction(char choice)
	{
		switch (choice)
		{
		case 'g':	digger->increaseGold();		break;
		case 'w':	digger->increaseWater();	break;
		case 'd':	digger->decHealth();		break;
		case 's':	digger->increaseSonar();	break;
		case 'k':	digger->killDiggerMan();	break;
		default:; break;
		}
	}

	void decPHealth(Protester * p)
	{
		if ((int)p->getHealth() > 0)
		{
			playSound(SOUND_PROTESTER_ANNOYED);
			p->decHealth();
			p->setStun();
		}
		else
		{
			playSound(SOUND_PROTESTER_GIVE_UP);
			p->setLeave();
		}
	}

	void bribeProtester(Gold * g) {
		for (std::shared_ptr<Protester> p : pRoster)
		{
			if (isWithinDistanceOfProtester(g, 3)) {
				p->setLeave();
				return;
			}

		}
	}
	void protesterSetLeave(Protester * p)
	{
		p->setLeave();
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
	void dropGold(int x, int y)
	{
		roster.emplace_back(std::make_shared<Gold>(x, y, Gold::GoldState::dropped));
	}
	bool doesCollide(int x, int y)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (!isValid(x + i, y + j) || dirt_field[x + i][y + j])
					return true;
		return false;
	}

	bool exitPath(int x, int y)
	{
		return false;	//Needs work
	}

	void removeDeadGameObjects()
	{
		//It theoretically is possible for this to crash
		for (size_t i = 0; i < roster.size(); i++)
			if (!roster[i]->isAlive())
				roster.erase(roster.begin() + i);
		for (size_t i = 0; i < pRoster.size(); i++) {
			if (!pRoster[i]->isAlive())
				pRoster.erase(pRoster.begin() + i);
		}
	}

	int rangeRandomNumGenerator(int min, int max)
	{
		int n = max - min + 1;
		int remainder = RAND_MAX % n;
		int x;
		do { x = rand(); } while (x >= RAND_MAX - remainder);
		return min + x % n;
	}

	void randomDirection(Actor * a, int x, int y)
	{
		int d;
		for (;;)
		{
			d = rangeRandomNumGenerator(0, 4);
			switch (d)
			{
			case 0: //right
				if (!doesCollide(x + 1, y)) {
					a->setDirection(GraphObject::right);
					return;
				}
			case 1: // left
				if (!doesCollide(x - 1, y)) {
					a->setDirection(GraphObject::left);
					return;
				}
			case 2: // up
				if (!doesCollide(x, y + 1)) {
					a->setDirection(GraphObject::up);
					return;
				}
			case 3: // down
				if (!doesCollide(x, y - 1)) {
					a->setDirection(GraphObject::down);
					return;
				}
			}
		}
	}

	bool atIntersection(GraphObject::Direction d, int x, int y)
	{
		if (d == GraphObject::left || d == GraphObject::right)
		{
			if (!doesCollide(x, y + 1) || !doesCollide(x, y - 1))
				return true;
		}
		if (d == GraphObject::up || d == GraphObject::down)
		{
			if (!doesCollide(x + 1, y) || !doesCollide(x + 1, y))
				return true;
		}
		return false;
	}
private:
	std::vector<std::shared_ptr<Actor>> roster;	//maybe also have a 2d array of actor pointers
	std::vector<std::shared_ptr<Protester>> pRoster;
	std::shared_ptr<DiggerMan> digger;
	std::array<std::array<std::unique_ptr<Dirt>, 64>, 64> dirt_field;  // modified to match his sample
	int oilCount;
	int ticks = 0;
};

#endif // STUDENTWORLD_H_
