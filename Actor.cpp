#include "Actor.h"
#include "StudentWorld.h"

void DiggerMan::move(StudentWorld * world)
{
	int value;
	world->getKey(value);
	if (!isAlive())
		return;
	int x = getX(), y = getY();
	switch (value)
	{
	case KEY_PRESS_DOWN:	moveTo(x, y - 1); break;
	case KEY_PRESS_UP:		moveTo(x, y + 1); break;
	case KEY_PRESS_LEFT:	moveTo(x - 1, y); break;
	case KEY_PRESS_RIGHT:	moveTo(x + 1, y); break;
	case KEY_PRESS_SPACE:;
	case KEY_PRESS_TAB:;
	case KEY_PRESS_ESCAPE:;
	}
}
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
