#pragma once
#include "ghost.h"

class Clyde : public Ghost {
  public:
	Clyde(QGraphicsItem* parent = nullptr);
	void setDefault();
	
  private:
	void fillAnimationList();
	void chooseTargetGrid() override;
};
