#include "inky.h"

Inky::Inky(QGraphicsItem* parent) : Ghost(parent)
{
	setPixmap(QPixmap("Images/sprites/Inky/DOWN_2.png").scaled(Px, Py));
	setPos(InitialInkyPosX, InitialInkyPosY);
	currentDirection = 'D';
	fillAnimationList();
	
	nextRow = ROW_ABOVE_BOX;
	nextCol = RIGHT;
	
	blinkyPos = QPointF(InitialBlinkyPosX, InitialBlinkyPosY);
}

void Inky::setDefault()
{
	setPixmap(QPixmap("Images/sprites/Inky/DOWN_2.png").scaled(Px, Py));
	setPos(InitialInkyPosX, InitialInkyPosY);
	currentDirection = 'D';
	nextRow = ROW_ABOVE_BOX;
	nextCol = RIGHT;
	
	blinkyPos = QPointF(InitialBlinkyPosX, InitialBlinkyPosY);	
	playerPos = QPointF(InitialPlayerPosX, InitialPlayerPosY);
	playerDirection = 'D';
	currentState = 0;
	mode = 's'; //	chase or scatter
	frightenedState = 'B'; // white or blue
	isFrightened = false;
	isEaten = false;
	exited = false;
	
	setFrightenedAnimationIntervals();
}

void Inky::fillAnimationList()
{	
	QList<QPixmap> listUp;
	listUp << QPixmap("Images/sprites/Inky/UP_1.png").scaled(Px, Py);
	listUp << QPixmap("Images/sprites/Inky/UP_2.png").scaled(Px, Py);
	
	QList<QPixmap> listDown;
	listDown << QPixmap("Images/sprites/Inky/DOWN_1.png").scaled(Px, Py);
	listDown << QPixmap("Images/sprites/Inky/DOWN_2.png").scaled(Px, Py);
	
	QList<QPixmap> listLeft;
	listLeft << QPixmap("Images/sprites/Inky/LEFT_1.png").scaled(Px, Py);
	listLeft << QPixmap("Images/sprites/Inky/LEFT_2.png").scaled(Px, Py);
	
	QList<QPixmap> listRight;
	listRight << QPixmap("Images/sprites/Inky/RIGHT_1.png").scaled(Px, Py);
	listRight << QPixmap("Images/sprites/Inky/RIGHT_2.png").scaled(Px, Py);
	
	animationList << listUp << listDown << listLeft << listRight;
}

void Inky::chooseTargetGrid()
{
	if (!isEaten) {
		if (mode == 'c') { //chase
			
			int infrontCol = playerPos.x() / GRID_SIZE;
			int infrontRow = playerPos.x() / GRID_SIZE;
			
			if (playerDirection == 'U') {
				infrontCol = playerPos.x() / GRID_SIZE;
				infrontRow = playerPos.y() / GRID_SIZE - 2;
			}
			else if (playerDirection == 'D') {
				infrontCol = playerPos.x() / GRID_SIZE;
				infrontRow = playerPos.y() / GRID_SIZE + 2;
			}
			else if (playerDirection == 'L') {
				infrontCol = playerPos.x() / GRID_SIZE - 2;
				infrontRow = playerPos.y() / GRID_SIZE;
			}
			else if (playerDirection == 'R') {
				infrontCol = playerPos.x() / GRID_SIZE + 2;
				infrontRow = playerPos.y() / GRID_SIZE;
			}
			
			targetCol = infrontCol + (infrontCol - blinkyPos.x() / GRID_SIZE);
			targetRow = infrontRow + (infrontRow - blinkyPos.y() / GRID_SIZE);
		}
		else if (mode == 's') { //scatter
			targetCol = 26;
			targetRow = 29;
		}
	}
	else {
		targetCol = RIGHT;
		targetRow = ROW_ABOVE_BOX;
	}
}

void Inky::setBlinkyPos(QPointF newPos)
{
	blinkyPos = newPos;
}
