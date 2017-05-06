#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y, 
		Direction dir = right, double s = 1.0, 
		unsigned int d = 0) : GraphObject(ID, x, y, dir, s, d) { setVisible(true); }	//Does an object below not need to be set to visible?
	virtual void doSomething() = 0;
	virtual ~Actor() {}
};

class DiggerMan : public Actor
{
	size_t m_health;
	size_t m_water;
	size_t m_gold;
	bool m_sonar;
public:
	DiggerMan() : Actor(IMID_PLAYER, 30, 60, right, 1.0, 0), m_health(10), m_water(5), m_gold(0) {}	//Define sonar behavior
	unsigned int getHealth()	{ return m_health; }
	unsigned int getWater()		{ return m_water; }
	unsigned int getGold()		{ return m_gold; }
	bool getSonar()				{ return m_sonar; }
	bool isAlive()				{ return m_health != 0; }
	virtual void doSomething() override {}
	void move(StudentWorld*);
	virtual ~DiggerMan() {}
};

class Protester : public Actor
{
public:
	Protester(int x, int y, int ID = IMID_PROTESTER) : Actor(ID, x, y) {}
	virtual void doSomething() override {}
	virtual ~Protester() {}
};

class HardcoreProtester : public Protester
{
public:
	HardcoreProtester(int x, int y) : Protester(x, y, IMID_HARD_CORE_PROTESTER) {}
	virtual void doSomething() override;
	virtual ~HardcoreProtester() {}
};

class Goodie : public Actor
{
public:
	Goodie(int ID, int x, int y) : Actor(ID, x, y) {}
	virtual void doSomething() = 0;
	virtual ~Goodie() {}
};

class Sonar : public Goodie
{
public:
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
	Water(int x, int y) : Goodie(IMID_WATER_POOL, x, y) {}
	virtual void doSomething() override {}
	virtual ~Water() {}
};

class Oil : public Goodie
{
public:
	Oil(int x, int y) : Goodie(IMID_BARREL, x, y) {}
	virtual void doSomething() override {}
	virtual ~Oil() {}
};

class Boulder : public Actor
{
public:
	Boulder(int x, int y) : Actor(IMID_BOULDER, x, y) {}
	virtual void doSomething() override {}
	virtual ~Boulder() {}
};

class Dirt : public Actor
{
public:
	Dirt(int x, int y) : Actor(IMID_DIRT, x, y) {}
	virtual void doSomething() override {}
	virtual ~Dirt() {}
};

class Projectile : public Actor
{
public:
	Projectile(int x, int y) : Actor(IMID_WATER_SPURT, x, y) {}
	virtual void doSomething() override {}
	virtual ~Projectile() {}
};
#endif // ACTOR_H_
