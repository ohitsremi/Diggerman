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
void Sonar::doSomething(StudentWorld * world)
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
	if (world->isWithinDistance(this, 3))
	{
		alive = false;
		world->playSound(SOUND_GOT_GOODIE);
		world->increaseScore(75);
		world->increaseSonar();
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
	if (world->isWithinDistanceOfProtester(this, 3))
	{
		world->increaseScore(100);
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
	int x = getX(), y = getY();
	if (!isAlive()) //1 check if alive
		return;
	int ticksToWaitBetweenMoves = std::max(0, 3 - (int)world->getLevel() / 4);
	if (status == rest) //2 check for rest status
	{
		if (ticks >= ticksToWaitBetweenMoves)
		{
			status = active;
			ticks = 0;
			return;
		}
		ticks++;
		return;
	}

	if (status == leave) //3 Check Leave status
	{
		world->playSound(SOUND_PROTESTER_GIVE_UP);
		//		std::array<std::array<bool, 64>, 64> exitP;
		world->exitPath(x, y);
		setVisible(false);
		return;
	}
	if (status == stunned) // Check Stunned status
	{
		if (stunTicks > std::max(50, 100 - (int)world->getLevel() * 10))
		{
			status = active;
			stunTicks = 0;
			return;
		}
		stunTicks++;
		return;
	}

	if (status == active) // Check active status
	{
		if (nonRestingTicks > 15)
		{
			if (nonShoutingTicks > 15)
			{
				if (world->isWithinDistance(this, 4)) // 4 If within distance of diggerman, shout at him
				{
					world->playSound(SOUND_PROTESTER_YELL);
					world->decHealth();
					nonShoutingTicks = 0;
					return;
				}
			}
			else
				nonShoutingTicks++;
		}
		nonRestingTicks++;
		recentPerpTicks++;
		if (world->diggermanAhead(this, x, y)) // 5 If diggerman is ahead 
		{
			Direction d = getDirection(); // Face Diggerman and take 1 step toward
			if (d == right) {
				moveTo(x + 1, y);
				status = rest;
				numSquaresToMoveInCurrentDirection = 0;
				return;
			}
			if (d == left) {
				moveTo(x - 1, y);
				status = rest;
				numSquaresToMoveInCurrentDirection = 0;
				return;
			}
			if (d == up) {
				moveTo(x, y + 1);
				status = rest;
				numSquaresToMoveInCurrentDirection = 0;
				return;
			}
			if (d == down) {
				moveTo(x, y - 1);
				status = rest;
				numSquaresToMoveInCurrentDirection = 0;
				return;
			}
		}

		if (!world->diggermanAhead(this, x, y)) // 6, can't see diggerman, then continue to walk
		{
			numSquaresToMoveInCurrentDirection--; //decrement
			if (numSquaresToMoveInCurrentDirection <= 0)
			{
				numSquaresToMoveInCurrentDirection = world->rangeRandomNumGenerator(8, 60); // choose distance
				world->randomDirection(this, x, y); //choose direction
			}
		}

		if (world->atIntersection(getDirection(), x, y))
		{
			if (recentPerpTurn == true)
			{
				if (recentPerpTicks > 200)
				{
					recentPerpTicks = 0;
					recentPerpTurn = false;
				}
			}
			if (recentPerpTurn == false)
			{
				Direction d = getDirection();
				if (d == left || d == right)
				{
					recentPerpTurn = true;
					numSquaresToMoveInCurrentDirection = world->rangeRandomNumGenerator(8, 60);
					if (!world->doesCollide(x, y + 1) && !world->doesCollide(x, y - 1))
					{
						int n = world->rangeRandomNumGenerator(0, 1);
						if (n == 0)
							setDirection(up);
						if (n == 1)
							setDirection(down);
					}
					else if (!world->doesCollide(x, y + 1))
						setDirection(up);
					else if (!world->doesCollide(x, y - 1))
						setDirection(down);
				}
				else if (d == up || d == down)
				{
					recentPerpTurn = false;
					numSquaresToMoveInCurrentDirection = world->rangeRandomNumGenerator(8, 60);
					if (!world->doesCollide(x + 1, y) && !world->doesCollide(x - 1, y))
					{
						int n = world->rangeRandomNumGenerator(0, 1);
						if (n == 0)
							setDirection(right);
						if (n == 1)
							setDirection(left);
					}
					else if (!world->doesCollide(x + 1, y))
						setDirection(right);
					else if (!world->doesCollide(x - 1, y))
						setDirection(left);
				}
			}
		}

		if (!move(world))
		{
			numSquaresToMoveInCurrentDirection = 0;
			return;
		}

	}
}

bool Protester::move(StudentWorld *world)
{
	Direction d = getDirection();
	int x = getX(), y = getY();
	if (d == right && !world->doesCollide(x + 1, y))
	{
		moveTo(x + 1, y);
		status = rest;
		return true;
	}
	if (d == left && !world->doesCollide(x - 1, y))
	{
		moveTo(x - 1, y);
		status = rest;
		return true;
	}
	if (d == up && !world->doesCollide(x, y + 1))
	{
		moveTo(x, y + 1);
		status = rest;
		return true;
	}
	if (d == down && !world->doesCollide(x, y - 1))
	{
		moveTo(x, y - 1);
		status = rest;
		return true;
	}
	return false;
}
void Protester::exitField()
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
