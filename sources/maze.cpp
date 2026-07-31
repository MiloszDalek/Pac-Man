#include "maze.h"

Maze::Maze(QGraphicsPixmapItem* parent) : QObject(), QGraphicsPixmapItem(parent)
{
	setPixmap(QPixmap(":/Images/maze.png").scaled(MAP_WIDTH, MAP_HEIGHT));
	animationTimer = new QTimer(this);
	color = 0;

	connect(animationTimer, SIGNAL(timeout()), this, SLOT(changeColor()));
}

void Maze::changeColor()
{
	if (color) {
		setPixmap(QPixmap(":/Images/maze.png").scaled(MAP_WIDTH, MAP_HEIGHT));
		color--;
	}
	else {
		setPixmap(QPixmap(":/Images/white_maze.png").scaled(MAP_WIDTH, MAP_HEIGHT));
		color++;
	}
	if (!intervals.isEmpty())
		animationTimer->start(intervals.dequeue());
	else
		animationTimer->stop();
}

void Maze::startAnimation()
{
	setIntervals();
	animationTimer->start(intervals.dequeue());
}

void Maze::setIntervals()
{
	intervals.clear();
	intervals.enqueue(2000);
	intervals.enqueue(250);
	intervals.enqueue(250);
	intervals.enqueue(250);
	intervals.enqueue(250);
	intervals.enqueue(250);
	intervals.enqueue(250);
	intervals.enqueue(250);
	intervals.enqueue(250);
	intervals.enqueue(250);
}
