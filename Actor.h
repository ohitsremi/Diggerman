#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y,
		Direction dir, double s = 1.0,
		unsigned int d = 0) : GraphObject(ID, x, y, dir, s, d) {
		setVisible(true);
	}	//Does an object below not need to be set to visible?
	virtual void doSomething(StudentWorld *) = 0;
	virtual bool isAlive() = 0;
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
	bool isAlive() override { return m_health != 0; }
	void increaseGold() { m_gold++; }
	void doSomething(StudentWorld *) override;
	virtual ~DiggerMan() {}
};

class Protester : public Actor
{
	size_t m_health;
public:
	Protester(int x, int y, int ID = IMID_PROTESTER) : Actor(ID, x, y, left, 1.0, 0), m_health(5) {}
	size_t getHealth() { return m_health; }
	virtual void doSomething(StudentWorld *) override;
	virtual bool isAlive() { return m_health != 0; };
	virtual ~Protester() {}
};

class HardcoreProtester : public Protester
{
	size_t m_health;
public:
	HardcoreProtester(int x, int y) : Protester(x, y, IMID_HARD_CORE_PROTESTER), m_health(20) {}
	virtual void doSomething(StudentWorld *) override;
	virtual bool isAlive() override;
	virtual ~HardcoreProtester() {}
};

class Goodie : public Actor
{
public:
	Goodie(int ID, int x, int y) : Actor(ID, x, y, right, 1.0, 2) {}
	virtual void doSomething(StudentWorld *) = 0;
	virtual bool isAlive() = 0;
	virtual ~Goodie() {}
};

class Sonar : public Goodie
{
public:
	Sonar(int x, int y) : Goodie(IMID_SONAR, x, y) { setVisible(true); }
	virtual void doSomething(StudentWorld *) override {};
	virtual bool isAlive() { return isVisible(); };
	virtual ~Sonar() {}
};

class Gold : public Goodie
{
public:
	enum GoldState { none, pickup, dropped };
	Gold(int x, int y, GoldState g) : Goodie(IMID_GOLD, x, y), status(g)
	{
		status == pickup ? setVisible(false) : setVisible(true);
	}
	virtual void doSomething(StudentWorld *) override;
	virtual bool isAlive() override { return alive; }
	virtual ~Gold() {}
private:
	GoldState status;
	bool alive = true;
};

class Water : public Goodie
{
public:
	Water(int x, int y) : Goodie(IMID_WATER_POOL, x, y) { setVisible(true); }
	virtual void doSomething(StudentWorld *) override {};
	virtual bool isAlive() override { return isVisible(); };
	virtual ~Water() {}
};

class Oil : public Goodie
{
	bool alive = true;
public:
	Oil(int x, int y) : Goodie(IMID_BARREL, x, y) { setVisible(false); }
	virtual void doSomething(StudentWorld *) override;
	virtual bool isAlive() override { return alive; }
	virtual ~Oil() {}
};

class Boulder : public Actor
{
	bool alive = true;
public:
	Boulder(int x, int y) : Actor(IMID_BOULDER, x, y, down, 1.0, 1) {}
	virtual void doSomething(StudentWorld *) override {}
	virtual bool isAlive() override { return alive; }
	virtual ~Boulder() {}
};

class Dirt : public Actor
{
public:
	Dirt(int x, int y) : Actor(IMID_DIRT, x, y, right, 0.25, 3) {}
	virtual void doSomething(StudentWorld *) override {}
	virtual bool isAlive() override { return false; }
	virtual ~Dirt() {}
};

class Projectile : public Actor
{
	int distance = 0;
	bool alive = true;
public:
	Projectile(int x, int y, Direction d) : Actor(IMID_WATER_SPURT, x, y, d, 1.0, 1) {}
	virtual void doSomething(StudentWorld *) override;
	virtual bool isAlive() override { return alive; }
	virtual ~Projectile() {}
};
#endif // ACTOR_H_
