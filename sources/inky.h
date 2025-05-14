#pragma once
#include "ghost.h"

class Inky : public Ghost {
  public:
	Inky(QGraphicsItem* parent = nullptr);
	void setDefault();
	void setBlinkyPos(QPointF);
	
  private:
	void fillAnimationList();
	void chooseTargetGrid() override;
	QPointF blinkyPos;
};
