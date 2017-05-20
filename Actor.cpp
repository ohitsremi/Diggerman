#include "Actor.h"
#include "StudentWorld.h"

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
		if (m_water == 0)
			break;
		else {
		world->addProjectile(x, y, getDirection());
		m_water--;
		}
		break;
	}
}

void Projectile::doSomething(StudentWorld * world)
{
	Direction d = getDirection();
	int x = getX(), y = getY();

	if (world->doesCollide(x, y, d))
	{
		setVisible(false);
		isAlive = false;
		return;
	}
	else if (distance == 4) //how far does the squirt travel
	{
		setVisible(false);
		isAlive = false;
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

void Oil::doSomething(StudentWorld * world)
{
	if (!isAlive)
		return;
	if (!isVisible() && world->isWithinDistance(this, 4))
	{
		setVisible(true);
		return;
	}
	if (world->isWithinDistance(this, 3))
	{
		isAlive = false;
		world->playSound(SOUND_FOUND_OIL);
		world->increaseScore(1000);
		world->decreaseOil();
	}

}
