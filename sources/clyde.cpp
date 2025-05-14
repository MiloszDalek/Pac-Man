#include "clyde.h"

#define MIN_PLAYER_DIST 8

Clyde::Clyde(QGraphicsItem* parent) : Ghost(parent)
{
	setPixmap(QPixmap("Images/sprites/Clyde/DOWN_2.png").scaled(Px, Py));
	setPos(InitialClydePosX, InitialClydePosY);
	currentDirection = 'D';
	fillAnimationList();
	
	nextRow = ROW_ABOVE_BOX;
	nextCol = LEFT;
}

void Clyde::setDefault()
{
	setPixmap(QPixmap("Images/sprites/Clyde/DOWN_2.png").scaled(Px, Py));
	setPos(InitialClydePosX, InitialClydePosY);
	currentDirection = 'D';
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

void Clyde::fillAnimationList()
{	
	QList<QPixmap> listUp;
	listUp << QPixmap("Images/sprites/Clyde/UP_1.png").scaled(Px, Py);
	listUp << QPixmap("Images/sprites/Clyde/UP_2.png").scaled(Px, Py);
	
	QList<QPixmap> listDown;
	listDown << QPixmap("Images/sprites/Clyde/DOWN_1.png").scaled(Px, Py);
	listDown << QPixmap("Images/sprites/Clyde/DOWN_2.png").scaled(Px, Py);
	
	QList<QPixmap> listLeft;
	listLeft << QPixmap("Images/sprites/Clyde/LEFT_1.png").scaled(Px, Py);
	listLeft << QPixmap("Images/sprites/Clyde/LEFT_2.png").scaled(Px, Py);
	
	QList<QPixmap> listRight;
	listRight << QPixmap("Images/sprites/Clyde/RIGHT_1.png").scaled(Px, Py);
	listRight << QPixmap("Images/sprites/Clyde/RIGHT_2.png").scaled(Px, Py);
	
	animationList << listUp << listDown << listLeft << listRight;
}

void Clyde::chooseTargetGrid()
{
	if (!isEaten) {
		if (mode == 'c') { //chase
		int playerX = playerPos.x() / GRID_SIZE;
		int playerY = playerPos.y() / GRID_SIZE;
		
		double distToPlayer = sqrt(pow(playerX - gridX(), 2) + pow(playerY - gridY(), 2));
		
			if (distToPlayer > MIN_PLAYER_DIST) {
				targetCol = playerPos.x() / GRID_SIZE;
				targetRow = playerPos.y() / GRID_SIZE;
			}
			else {
				targetCol = 1;
				targetRow = 29;
			}
		}
		else if (mode == 's') { //scatter
			targetCol = 1;
			targetRow = 29;
		}
	}
	else {
		targetCol = RIGHT;
		targetRow = ROW_ABOVE_BOX;
	}
}
