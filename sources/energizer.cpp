#include "energizer.h"

Energizer::Energizer(QGraphicsPixmapItem* parent) : QObject(), QGraphicsPixmapItem(parent)
{
	setPixmap(QPixmap("Images/sprites/energizer.png"));
	
	animationTimer = new QTimer(this);
	animationTimer->start(200);
	state = 0;
	
	connect(animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
	
	white = QPixmap("Images/sprites/energizer.png");
	black = QPixmap("Images/sprites/energizerBlack.png");
}

void Energizer::animate()
{
	if (state) {
		setPixmap(white);
		state--;
	}
	else {
		setPixmap(black);
		state++;
	}
}

int Energizer::centerX()
{
	return x() + boundingRect().width() / 2;
}

int Energizer::centerY()
{
	return y() + boundingRect().height() / 2;
}
