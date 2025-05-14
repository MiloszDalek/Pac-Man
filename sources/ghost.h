#pragma once
#include "entity.h"

class Ghost : public Entity {
	Q_OBJECT
  public:
	Ghost(QGraphicsItem* parent = nullptr);
	void setPlayerPos(QPointF);
	void setPlayerDirection(int);
	void changeMode();
	void changeDirectionToOpposite();
	void changeMoveSpeed(int);
	void setDotThreshold(int);
	void checkEatenDotsThreshold(int);
	void checkEatenDotsThresholdAlreadyExceeded(int);
	void setFrightenedMode();
	void setStartSpeed();
	void stop();
	void setEaten();
	bool isFrightenedCheck();
	bool isEatenCheck();
	QPointF returnTargetGrid();  //only for test purpose
	
  public slots:
	void animate();
	void move();
	void setNormalMode();
	void changeFrightenedState();
  
  protected:
	void changeState();
	bool moveToGridCenter();
	void chooseNextGrid();
	void chooseRandomNextGrid();
	void moveWithinBox();
	bool moveToCenterOfBox();
	void moveOutOfBox();
	void moveIntoBox();
	void fillFrightenedAnimationList();
	void fillEyesList();
	void setFrightenedAnimationIntervals();
	bool isAboveBoxWhileEaten();
	
	virtual void chooseTargetGrid() = 0;
	
	int targetRow;
	int targetCol;
	int nextRow;
	int nextCol;
	
	int dotThreshold;
	int dotsEaten;
	
	bool isFrightened;
	bool isEaten;
	bool exited;
	
	QPointF playerPos;
	int playerDirection;
	
	char mode;
	char frightenedState;
	
	QTimer* animationTimer;
	QTimer* moveTimer;
	QTimer* frightenedModeTimer;
	QTimer* frightenedAnimationTimer;
	QQueue<int> frightenedAnimationIntervals;
	
	QList<QList<QPixmap>> frightenedAnimationList;
	QList<QPixmap> eyesList;
};
