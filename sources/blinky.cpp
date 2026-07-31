#include "blinky.h"

Blinky::Blinky(QGraphicsItem* parent) : Ghost(parent)
{
	setPixmap(QPixmap(":/Images/sprites/Blinky/LEFT_2.png").scaled(Px, Py));
	setPos(InitialBlinkyPosX, InitialBlinkyPosY);
	currentDirection = 'L';
	fillAnimationList();
	nextRow = ROW_ABOVE_BOX;
	nextCol = LEFT;
	
	connect(moveTimer, SIGNAL(timeout()), this, SLOT(blinkyPosUpdate()));
}

void Blinky::blinkyPosUpdate()
{
	if ((int)y() % GRID_SIZE == GRID_SIZE / 2 && (int)x() % GRID_SIZE == GRID_SIZE / 2)
		emit blinkyPosChanged(QPointF(x(), y()));
}

void Blinky::setDefault()
{
	setPixmap(QPixmap(":/Images/sprites/Blinky/LEFT_2.png").scaled(Px, Py));
	setPos(InitialBlinkyPosX, InitialBlinkyPosY);
	currentDirection = 'L';
	nextRow = ROW_ABOVE_BOX;
	nextCol = LEFT;
	
	playerPos = QPointF(InitialPlayerPosX, InitialPlayerPosY);
	playerDirection = 'D';
	currentState = 0;
	mode = 's'; //	chase or scatter
	frightenedState = 'B'; // white or blue
	isFrightened = false;
	isEaten = false;
	exited = true;
	
	setFrightenedAnimationIntervals();
}

void Blinky::fillAnimationList()
{	
	QList<QPixmap> listUp;
	listUp << QPixmap(":/Images/sprites/Blinky/UP_1.png").scaled(Px, Py);
	listUp << QPixmap(":/Images/sprites/Blinky/UP_2.png").scaled(Px, Py);
	
	QList<QPixmap> listDown;
	listDown << QPixmap(":/Images/sprites/Blinky/DOWN_1.png").scaled(Px, Py);
	listDown << QPixmap(":/Images/sprites/Blinky/DOWN_2.png").scaled(Px, Py);
	
	QList<QPixmap> listLeft;
	listLeft << QPixmap(":/Images/sprites/Blinky/LEFT_1.png").scaled(Px, Py);
	listLeft << QPixmap(":/Images/sprites/Blinky/LEFT_2.png").scaled(Px, Py);
	
	QList<QPixmap> listRight;
	listRight << QPixmap(":/Images/sprites/Blinky/RIGHT_1.png").scaled(Px, Py);
	listRight << QPixmap(":/Images/sprites/Blinky/RIGHT_2.png").scaled(Px, Py);
	
	animationList << listUp << listDown << listLeft << listRight;
}

void Blinky::chooseTargetGrid()
{
	if (!isEaten) {
		if (mode == 'c') {	//chase
			targetCol = playerPos.x() / GRID_SIZE;
			targetRow = playerPos.y() / GRID_SIZE;
		}
		if (mode == 's') { //scatter
			targetCol = 26;
			targetRow = 1;
		}
	}
	else {
		targetCol = RIGHT;
		targetRow = ROW_ABOVE_BOX;
	}
	
	
}
