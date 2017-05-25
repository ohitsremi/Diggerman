#include "Actor.h"
#include "StudentWorld.h"

void DiggerMan::doSomething(StudentWorld * world)
{
	int value;
	world->getKey(value);
	if (!isAlive())
		return;
	int x = getX(), y = getY();
	Direction dir = getDirection();
	switch (value)
	{
	case KEY_PRESS_LEFT:

		if (dir != left)
		{
			setDirection(left);
			break;
		}
		if (x <= 0)	break;

		moveTo(x - 1, y);
		break;
	case KEY_PRESS_RIGHT:

		if (dir != right)
		{
			setDirection(right);
			break;
		}
		if (x >= 60) break;

		moveTo(x + 1, y);
		break;
	case KEY_PRESS_DOWN:

		if (dir != down)
		{
			setDirection(down);
			break;
		}
		if (y <= 0)	break;

		moveTo(x, y - 1);
		break;
	case KEY_PRESS_UP:

		if (dir != up)
		{
			setDirection(up);
			break;
		}
		if (y >= 60)	break;

		moveTo(x, y + 1);
		break;
	case KEY_PRESS_SPACE:
		if (m_water == 0)
			break;
		else
		{
			world->addProjectile(x, y, getDirection());
			world->playSound(SOUND_PLAYER_SQUIRT);	//This isn't working for some reason
			m_water--;
		}
		break;
	}
}

void Projectile::doSomething(StudentWorld * world)
{
	Direction d = getDirection();
	int x = getX(), y = getY();


	if (world->doesCollide(x, y))
	{
		setVisible(false);
		alive = false;
		return;
	}
	else if (distance == 4) //how far does the squirt travel
	{
		setVisible(false);
		alive = false;
		return;
	}
	else if (d == right)
	{
		distance++;
		moveTo(x + 1, y);
	}
	else if (d == left)
	{
		distance++;
		moveTo(x - 1, y);
	}
	else if (d == up)
	{
		distance++;
		moveTo(x, y + 1);
	}
	else if (d == down)
	{
		distance++;
		moveTo(x, y - 1);
	}
}

void Protester::doSomething(StudentWorld* world)
{

}
void Oil::doSomething(StudentWorld * world)
{
	if (!alive)
		return;
	if (!isVisible() && world->isWithinDistance(this, 4))
	{
		setVisible(true);
		return;
	}
	if (world->isWithinDistance(this, 3))
	{
		alive = false;
		world->playSound(SOUND_FOUND_OIL);
		world->increaseScore(1000);
		world->decreaseOil();
	}

}

void Gold::doSomething(StudentWorld * world)
{
	if (!alive)
		return;
	if (isVisible() && world->isWithinDistance(this, 4))
	{
		setVisible(true);
		return;
	}
	if (status == pickup && world->isWithinDistance(this, 3))
	{
		alive = false;
		world->playSound(SOUND_GOT_GOODIE);
		world->increaseScore(10);
		world->increaseGold();
	}
}

void Boulder::doSomething(StudentWorld * world)
{

	int x = getX(), y = getY();
	if (!alive)
		return;
	if (status == stable) {
		if (!world->doesCollide(x, y - 1))
		{
			status = waiting;	
		}
	}
	if (status == waiting) 
	{
		ticks++;
		if (ticks == 30)
		{
			status = falling;
			world->playSound(SOUND_FALLING_ROCK);
		}
	}
	if (status == falling)
	{
		if (world->doesCollide(x, y))
			status = dead;
		else
			moveTo(x, y - 1);
	}
	if (status == dead)
	{
		setVisible(false);
		alive = false;
	}
}
void Water::doSomething(StudentWorld * world)
{
	if (!alive)
		return;
	if (status == temporary)
	{
		ticks++;
		int m = std::max(100, 300 - 10 * (int)world->getLevel());
		if (ticks == m)
		{
			setVisible(false);
			alive = false;
		}
	}
	if (status == temporary && world->isWithinDistance(this, 3))
	{
		alive = false;
		setVisible(false);
		world->playSound(SOUND_GOT_GOODIE);
		world->increaseScore(100);
		world->increaseWater();
	}
}
