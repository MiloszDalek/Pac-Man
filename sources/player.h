#pragma once
#include <QKeyEvent>

#include "entity.h"
#define STEP 1
#define PLAYER_MOVE_TIME 8


class Player : public Entity {
	Q_OBJECT
  public:
	Player(QGraphicsItem* parent = nullptr);
	void stop();
	void setDefault();
	
  public slots:
	void move();
	void animate();
	void centeredPlayer();
	void startDeathAnimation();
	void die();
	
  signals:
	void catched();
	void newTry();
	void playerPosChanged(QPointF newPos);
	void playerDirectionChanged(int playerDirection);
	
  protected:
	void keyPressEvent(QKeyEvent* event);
	
  private:
	QTimer* moveTimer;
	QTimer* animationTimer;
	QTimer* corectionTimer;
	QTimer* deathTimer;
	
	int buffDirection;
	int deathState;
	bool inMove;
	bool alive;
	
	QList<QPixmap> deathAnimation;
	
	void fillAnimationList();
	void fillDeathAnimationList();
	void changeState();
	void checkMove();
};
