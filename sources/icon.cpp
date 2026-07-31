#include "icon.h"

Icon::Icon(QGraphicsPixmapItem* parent, int point) : QObject(), QGraphicsPixmapItem(parent)
{
	fillBluePointsList();
	despawnTimer = new QTimer(this);
	setPixmap(bluePointsList[point]);
	
	despawnTimer->start(DESPAWN_TIME);
	connect(despawnTimer, SIGNAL(timeout()), this, SLOT(despawn()));
}

Icon::Icon(QGraphicsPixmapItem* parent) : QObject(), QGraphicsPixmapItem(parent)
{
	fillIconList();
	fillPinkPointsList();
	setPos(FruitPosX, FruitPosY);
}

Icon::Icon(QGraphicsPixmapItem* parent, char icon) :
	QObject(), QGraphicsPixmapItem(parent)
{
	fillIconList();
//	despawnTimer = new QTimer(this);
	
	this->icon = icon;
	switch (this->icon) {
		case LIVE:
			setPixmap(iconList[0]);
			break;
		case CHERRY:
			setPixmap(iconList[1]);
			break;
		case STRAWBERRY:
			setPixmap(iconList[2]);
			break;
		case PEACH:
			setPixmap(iconList[3]);
			break;
		case APPLE:
			setPixmap(iconList[4]);
			break;
		case GRAPES:
			setPixmap(iconList[5]);
			break;
		case GALAXIAN:
			setPixmap(iconList[6]);
			break;
		case BELL:
			setPixmap(iconList[7]);
			break;
		case KEY:
			setPixmap(iconList[8]);
			break;			
	}
}

void Icon::setFruit(char icon) 
{
	this->icon = icon;
	switch (this->icon) {
		case LIVE:
			setPixmap(iconList[0]);
			break;
		case CHERRY:
			selectFruit(1);
			break;
		case STRAWBERRY:
			selectFruit(2);
			break;
		case PEACH:
			selectFruit(3);
			break;
		case APPLE:
			selectFruit(4);
			break;
		case GRAPES:
			selectFruit(5);
			break;
		case GALAXIAN:
			selectFruit(6);
			break;
		case BELL:
			selectFruit(7);
			break;
		case KEY:
			selectFruit(8);
			break;
	}
}

int Icon::catched()
{
	switch (icon) {
		case CHERRY:
			setPixmap(pinkPointsList[0]);
			return 100; 
			break;
		case STRAWBERRY:
			setPixmap(pinkPointsList[1]);
			return 300;
			break;
		case PEACH:
			setPixmap(pinkPointsList[2]);
			return 500;
			break;
		case APPLE:
			setPixmap(pinkPointsList[3]);
			return 700;
			break;
		case GRAPES:
			setPixmap(pinkPointsList[4]);
			return 1000;
			break;
		case GALAXIAN:
			setPixmap(pinkPointsList[5]);
			return 2000;
			break;
		case BELL:
			setPixmap(pinkPointsList[6]);
			return 3000;
			break;
		case KEY:
			setPixmap(pinkPointsList[7]);
			return 5000;
			break;
		default:
			return 0;
			break;
	}
}

void Icon::selectFruit(int numberOnList)
{
	setPixmap(iconList[numberOnList]);
}

void Icon::despawn()
{
	delete this;
}

void Icon::fillBluePointsList()
{
	bluePointsList << QPixmap(":/Images/sprites/Points/Icon_200.png").scaled(Px, Py);
	bluePointsList << QPixmap(":/Images/sprites/Points/Icon_400.png").scaled(Px, Py);
	bluePointsList << QPixmap(":/Images/sprites/Points/Icon_800.png").scaled(Px, Py);
	bluePointsList << QPixmap(":/Images/sprites/Points/Icon_1600.png").scaled(Px, Py);
}

void Icon::fillIconList()
{
	iconList << QPixmap(":/Images/sprites/PacMan/LEFT_2.png").scaled(Px, Py);
	iconList << QPixmap(":/Images/sprites/Icons/CHERRY.png").scaled(Px, Py);
	iconList << QPixmap(":/Images/sprites/Icons/STRAWBERRY.png").scaled(Px, Py);
	iconList << QPixmap(":/Images/sprites/Icons/PEACH.png").scaled(Px, Py);	
	iconList << QPixmap(":/Images/sprites/Icons/APPLE.png").scaled(Px, Py);
	iconList << QPixmap(":/Images/sprites/Icons/GRAPES.png").scaled(Px, Py);
	iconList << QPixmap(":/Images/sprites/Icons/GALAXIAN.png").scaled(Px, Py);
	iconList << QPixmap(":/Images/sprites/Icons/BELL.png").scaled(Px, Py);
	iconList << QPixmap(":/Images/sprites/Icons/KEY.png").scaled(Px, Py);
}

void Icon::fillPinkPointsList()
{
	pinkPointsList << QPixmap(":/Images/sprites/Icons/PINK_100.png").scaled(Px, Py);
	pinkPointsList << QPixmap(":/Images/sprites/Icons/PINK_300.png").scaled(Px, Py);
	pinkPointsList << QPixmap(":/Images/sprites/Icons/PINK_500.png").scaled(Px, Py);
	pinkPointsList << QPixmap(":/Images/sprites/Icons/PINK_700.png").scaled(Px, Py);
	pinkPointsList << QPixmap(":/Images/sprites/Icons/PINK_1000.png").scaled(Px, Py);
	pinkPointsList << QPixmap(":/Images/sprites/Icons/PINK_2000.png").scaled(Px, Py);
	pinkPointsList << QPixmap(":/Images/sprites/Icons/PINK_3000.png").scaled(Px, Py);
	pinkPointsList << QPixmap(":/Images/sprites/Icons/PINK_5000.png").scaled(Px, Py);
}
