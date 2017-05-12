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
	case KEY_PRESS_LEFT:
		if (x <= 0) {
			break;
		}
		if (getDirection() != left) {
			setDirection(left);
			break;
		}
		moveTo(x - 1, y);
		break;

	case KEY_PRESS_RIGHT:
		if (x >= 60) {
			break;
		}
		if (getDirection() != right) {
			setDirection(right);
			break;
		}
		moveTo(x + 1, y);
		break;

	case KEY_PRESS_DOWN:
		if (y <= 0) {
			break;
		}
		if (getDirection() != down) {
			setDirection(down);
			break;
		}
		moveTo(x, y - 1);
		break;

	case KEY_PRESS_UP:
		if (y >= 60) {
			break;
		}
		if (getDirection() != up) {
			setDirection(up);
			break;
		}
		moveTo(x, y + 1);
		break;

	}
}
/*
void DiggerMan::doSomething()
{
	int value;
	if (!isAlive())
		return;
	int x = getX(), y = getY();
	if (getWorld()->getKey(value) == true){
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
