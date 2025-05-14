#include "pinky.h"

Pinky::Pinky(QGraphicsItem* parent) : Ghost(parent)
{
	setPixmap(QPixmap("Images/sprites/Pinky/UP_2.png").scaled(Px, Py));
	setPos(InitialPinkyPosX, InitialPinkyPosY);
	currentDirection = 'U';
	fillAnimationList();
	
	nextRow = ROW_ABOVE_BOX;
	nextCol = LEFT;
}

void Pinky::setDefault()
{
	setPixmap(QPixmap("Images/sprites/Pinky/UP_2.png").scaled(Px, Py));
	setPos(InitialPinkyPosX, InitialPinkyPosY);
	currentDirection = 'U';	
	nextRow = ROW_ABOVE_BOX;
	nextCol = LEFT;
	
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

void Pinky::fillAnimationList()
{	
	QList<QPixmap> listUp;
	listUp << QPixmap("Images/sprites/Pinky/UP_1.png").scaled(Px, Py);
	listUp << QPixmap("Images/sprites/Pinky/UP_2.png").scaled(Px, Py);
	
	QList<QPixmap> listDown;
	listDown << QPixmap("Images/sprites/Pinky/DOWN_1.png").scaled(Px, Py);
	listDown << QPixmap("Images/sprites/Pinky/DOWN_2.png").scaled(Px, Py);
	
	QList<QPixmap> listLeft;
	listLeft << QPixmap("Images/sprites/Pinky/LEFT_1.png").scaled(Px, Py);
	listLeft << QPixmap("Images/sprites/Pinky/LEFT_2.png").scaled(Px, Py);
	
	QList<QPixmap> listRight;
	listRight << QPixmap("Images/sprites/Pinky/RIGHT_1.png").scaled(Px, Py);
	listRight << QPixmap("Images/sprites/Pinky/RIGHT_2.png").scaled(Px, Py);
	
	animationList << listUp << listDown << listLeft << listRight;
}

void Pinky::chooseTargetGrid()
{
	if (!isEaten) {
		if (mode == 'c') { //chase
			if (playerDirection == 'U') {
				targetCol = playerPos.x() / GRID_SIZE;
				targetRow = playerPos.y() / GRID_SIZE - 4;
			}
			else if (playerDirection == 'D') {
				targetCol = playerPos.x() / GRID_SIZE;
				targetRow = playerPos.y() / GRID_SIZE + 4;
			}
			else if (playerDirection == 'L') {
				targetCol = playerPos.x() / GRID_SIZE - 4;
				targetRow = playerPos.y() / GRID_SIZE;
			}
			else if (playerDirection == 'R') {
				targetCol = playerPos.x() / GRID_SIZE + 4;
				targetRow = playerPos.y() / GRID_SIZE;
			}
		}
		else if (mode == 's') { //scatter
			targetCol = 1;
			targetRow = 1;
		}
	}
	else {
		targetCol = RIGHT;
		targetRow = ROW_ABOVE_BOX;
	}
}
