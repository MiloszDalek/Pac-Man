#pragma once
#include "entity.h"
#include <QGraphicsPixmapItem>
#include <QTimer>

class Maze : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT
  public:
	Maze(QGraphicsPixmapItem* parent = nullptr);
	
  public slots:
  	void changeColor();
  	void startAnimation();

  private:
	void setIntervals();
	QTimer* animationTimer;
	QQueue<int> intervals;
	int color;
};
