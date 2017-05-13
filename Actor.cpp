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
/*
void DiggerMan::doSomething()
{
	std::cout << "diggerman do something";
	//getWorld();
	int value;
	if (!isAlive())
		return;
	int x = getX(), y = getY();
	if (getWorld().getKey(value) == true){
		switch (value)
		{
		case KEY_PRESS_DOWN:	moveTo(x, y - 1); break;
		case KEY_PRESS_UP:	moveTo(x, y + 1); break;
		case KEY_PRESS_LEFT:	moveTo(x - 1, y); break;
		case KEY_PRESS_RIGHT:	moveTo(x + 1, y); break;
		case KEY_PRESS_SPACE:;
		case KEY_PRESS_TAB:;
		case KEY_PRESS_ESCAPE:;
		}
	}
	
}
*/

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
