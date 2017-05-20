#include "Actor.h"
#include "StudentWorld.h"
/*
StudentWorld& Actor::getWorld() {
StudentWorld sw;
return sw;
}
*/
void DiggerMan::move(StudentWorld * world)
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
		if (y >= 60)break;

		moveTo(x, y + 1);
		break;
	case KEY_PRESS_SPACE:
		world->addProjectile(x, y, getDirection());
		break;
	}
}

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Projectile::doSomething()
{
	StudentWorld SW;
	Direction d = getDirection();
	int x = getX(), y = getY();
/*
	if (SW.checkObstacle(x, y)) {
		return;
	}

	*/
	if (distance == 6) //how far does the squirt travel
	{
		setVisible(false);
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
