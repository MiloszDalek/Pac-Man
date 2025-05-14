#pragma once
#include <QGraphicsPixmapItem>

class Dot : public QGraphicsPixmapItem {
  public:
	Dot(QGraphicsPixmapItem* parent = nullptr);
	
	int centerX();
	int centerY();
};
