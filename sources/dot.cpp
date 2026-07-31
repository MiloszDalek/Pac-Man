#include "dot.h"

Dot::Dot(QGraphicsPixmapItem* parent) : QGraphicsPixmapItem(parent)
{
	setPixmap(QPixmap(":/Images/sprites/dot.png").scaled(4, 4));
}

int Dot::centerX()
{
	return x() + boundingRect().width() / 2;
}

int Dot::centerY()
{
	return y() + boundingRect().height() / 2;
}
