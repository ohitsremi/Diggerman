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
	size_t m_sonar;
public:
	DiggerMan() : Actor(IMID_PLAYER, 30, 60, right, 1.0, 0), m_health(10), m_water(5), m_gold(0), m_sonar(true) {}	//Define sonar behavior
	size_t getHealth() { return m_health; }
	size_t getWater() { return m_water; }
	size_t getGold() { return m_gold; }
	size_t getSonar() { return m_sonar; }
	bool isAlive() { return m_health != 0; }
	void dropGold() { m_gold -= 1; }
	void decHealth() { m_health -= 2; }
	void killDiggerMan() { m_health = 0; }
	void increaseSonar() { m_sonar += 2; }
	void increaseGold() { m_gold++; }
	void increaseWater() { m_water += 5; }
	void doSomething(StudentWorld *) override;
	virtual ~DiggerMan() {}
};

class Protester : public Actor
{
	int ticks = 0;
	int nonRestingTicks = 0;
	int stunTicks = 0;
	int nonShoutingTicks = 0;
	int recentPerpTicks = 0;
	bool recentPerpTurn = false;
public:
	Protester(int x, int y, int ID = IMID_PROTESTER) : Actor(ID, x, y, left, 1.0, 0), m_health(5) { status = rest; }
	size_t getHealth() { return m_health; }
	enum protesterState { rest, active, chase, leave, stunned };
	virtual void doSomething(StudentWorld *) override;
	void setLeave() { status = leave; }
	void exitField();
	void decHealth() { m_health -= 2; }
	void setStun() { status = stunned; }
	bool move(StudentWorld *world);

	virtual bool isAlive() { return isVisible(); };
	virtual ~Protester() {}
private:
	size_t m_health;
	protesterState status;
	int numSquaresToMoveInCurrentDirection = 0;
};

class HardcoreProtester : public Protester
{
	size_t m_health;
public:
	HardcoreProtester(int x, int y) : Protester(x, y, IMID_HARD_CORE_PROTESTER), m_health(20) {}
	bool isAlive() { return m_health != 0; }
	virtual void doSomething(StudentWorld *) override {}
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
	enum SonarState { temporary };
	Sonar(int x, int y) : Goodie(IMID_SONAR, x, y), status(temporary) { setVisible(true); }
	virtual bool isAlive() { return alive; }
	virtual void doSomething(StudentWorld *) override;
	virtual ~Sonar() {}
private:
	SonarState status;
	bool alive = true;
	int ticks = 0;
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
	int ticks = 0;
};

class Water : public Goodie
{
public:
	enum WaterState { temporary };
	int ticks = 0;
	Water(int x, int y) : Goodie(IMID_WATER_POOL, x, y)
	{
		status = temporary;
		setVisible(true);
	}
	virtual void doSomething(StudentWorld *) override;
	virtual bool isAlive() override { return isVisible(); }
	virtual ~Water() {}
private:
	WaterState status;
	bool alive = true;

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
public:
	enum BoulderState { stable, waiting, falling, dead };
	int ticks = 0;
	Boulder(int x, int y) : Actor(IMID_BOULDER, x, y, down, 1.0, 1)
	{
		status = stable;
	}
	virtual void doSomething(StudentWorld *) override;
	virtual bool isAlive() override { return alive; }
	virtual ~Boulder() {}
private:
	BoulderState status;
	bool alive = true;
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
