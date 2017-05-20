#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y,
		Direction dir, double s = 1.0,
		unsigned int d = 0) : GraphObject(ID, x, y, dir, s, d) {
		setVisible(true);
	}	//Does an object below not need to be set to visible?
	virtual void doSomething() = 0;
	
	//StudentWorld& getWorld();
	
	virtual ~Actor() {}
};

class DiggerMan : public Actor
{
	size_t m_health;
	size_t m_water;
	size_t m_gold;
	bool m_sonar;
public:
	DiggerMan() : Actor(IMID_PLAYER, 30, 60, right, 1.0, 0), m_health(10), m_water(5), m_gold(0), m_sonar(true) {}	//Define sonar behavior
	size_t getHealth() { return m_health; }
	size_t getWater() { return m_water; }
	size_t getGold() { return m_gold; }
	bool getSonar() { return m_sonar; }
	bool isAlive() { return m_health != 0; }
	void doSomething(/*const int & value*/)		
	{
		if (!isAlive())
			//move(StudentWorld());
			return;
	}	
	void move(StudentWorld*);
	virtual ~DiggerMan() {}
};

class Protester : public Actor
{
	size_t m_health;
public:
	Protester(int x, int y, int ID = IMID_PROTESTER) : Actor(ID, x, y, left, 1.0, 0), m_health(5) {}
	size_t getHealth() { return m_health; }
	virtual void doSomething() override {}
	virtual ~Protester() {}
};

class HardcoreProtester : public Protester
{
	size_t m_health;
public:
	HardcoreProtester(int x, int y) : Protester(x, y, IMID_HARD_CORE_PROTESTER), m_health(20) {}
	virtual void doSomething() override {}
	virtual ~HardcoreProtester() {}
};

class Goodie : public Actor
{
public:
	Goodie(int ID, int x, int y) : Actor(ID, x, y, right, 1.0, 2) {}
	virtual void doSomething() = 0;
	virtual ~Goodie() {}
};

class Sonar : public Goodie
{
public:
	Sonar(int x, int y) : Goodie(IMID_SONAR, x, y) { setVisible(true); }
	virtual void doSomething() override {}
	virtual ~Sonar() {}
};

class Gold : public Goodie
{
public:
	Gold(int x, int y) : Goodie(IMID_GOLD, x, y) {}
	virtual void doSomething() override {}
	virtual ~Gold() {}
};

class Water : public Goodie
{
public:
	Water(int x, int y) : Goodie(IMID_WATER_POOL, x, y) { setVisible(true); }
	virtual void doSomething() override {}
	virtual ~Water() {}
};

class Oil : public Goodie
{
public:
	Oil(int x, int y) : Goodie(IMID_BARREL, x, y) { setVisible(true); }
	virtual void doSomething() override {}
	virtual ~Oil() {}
};

class Boulder : public Actor
{
public:
	Boulder(int x, int y) : Actor(IMID_BOULDER, x, y, down, 1.0, 1) {}
	virtual void doSomething() override {}
	virtual ~Boulder() {}
};

class Dirt : public Actor
{
public:
	//Dirt() : Actor(IMID_DIRT, 0, 0, right, 0.25, 3) { setVisible(false); }
	Dirt(int x, int y) : Actor(IMID_DIRT, x, y, right, 0.25, 3) {}
	virtual void doSomething() override {}
	virtual ~Dirt() {}
};

class Projectile : public Actor
{
	int distance=0;
public:
	Projectile(int x, int y, Direction d) : Actor(IMID_WATER_SPURT, x, y, d, 1.0, 1) {}
	bool isAlive = true;
	void doSomething() override;
	virtual ~Projectile() {}
};
#endif // ACTOR_H_
