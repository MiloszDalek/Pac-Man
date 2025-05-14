#pragma once
#include "ghost.h"

class Pinky : public Ghost {
  public:
	Pinky(QGraphicsItem* parent = nullptr);
	void setDefault();
	
  private:
	void fillAnimationList();
	void chooseTargetGrid() override;
};
