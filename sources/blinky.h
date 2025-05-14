#pragma once
#include "ghost.h"

class Blinky : public Ghost {
	Q_OBJECT
  public:
	Blinky(QGraphicsItem* parent = nullptr);
	void setDefault();
	
  private slots:
    void blinkyPosUpdate();	
	
  signals:
	void blinkyPosChanged(QPointF newPos);
	
  private:
	void fillAnimationList();
	void chooseTargetGrid() override;
};
