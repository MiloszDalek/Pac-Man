#pragma once
#include <QGraphicsPixmapItem>
#include <QList>
#include <QTimer>
#include <QQueue>
#include <QRandomGenerator>
#include <QDebug>
#include <cmath>

#define STEP 1

#define GRID_SIZE 20

#define Px (GRID_SIZE * 2)
#define Py (GRID_SIZE * 2)

#define FONT_SIZE 16

#define MAX_FRUITS_NUM 8

#define InitialPlayerPosX (GRID_SIZE * 13)
#define InitialPlayerPosY (GRID_SIZE * 22 + GRID_SIZE / 2)

#define InitialBlinkyPosX (13 * GRID_SIZE) 
#define InitialBlinkyPosY (11 * GRID_SIZE - GRID_SIZE / 2)

#define InitialPinkyPosX (13 * GRID_SIZE) 
#define InitialPinkyPosY (14 * GRID_SIZE - GRID_SIZE / 2)

#define InitialInkyPosX (11 * GRID_SIZE)
#define InitialInkyPosY (14 * GRID_SIZE - GRID_SIZE / 2)

#define InitialClydePosX (15 * GRID_SIZE)
#define InitialClydePosY (14 * GRID_SIZE - GRID_SIZE / 2)

#define	FruitPosX (13 * GRID_SIZE)
#define FruitPosY (17 * GRID_SIZE - GRID_SIZE / 2)

#define EXTRA_LIVE_THRESHOLD 10000
#define TIME_AFTER_WIN 4500

#define ROWS 31
#define COLS 28

#define WALL 1
#define BOX 2

#define ROW_ABOVE_BOX 11
#define LEFT 13
#define RIGHT 14

#define NORMAL_SPEED 8
#define INSIDE_BOX_SPEED 16
#define FRIGHTENED_SPEED (NORMAL_SPEED * 2)
#define EYE_SPEED 4
#define TUNNEL_SPEED (NORMAL_SPEED * 2)

#define FRIGHTENED_MODE_TIME 6000
#define ANIMATION_TIME 150
#define START_TIME 2500
#define BEGIN_TIME 4100
#define DEATH_TIME 140
#define EXITING_TIME 2000
#define DESPAWN_TIME 3000
#define FRUIT_DESPAWN_TIME 10000

#define BLINKY_THRESHOLD 0
#define PINKY_THRESHOLD 30
#define INKY_THRESHOLD 60
#define CLYDE_THRESHOLD 90

#define FIRST_FRUIT_THRESHOLD 70
#define SECOND_FRUIT_THRESHOLD 170

#define MAP_HEIGHT (GRID_SIZE * ROWS)
#define MAP_WIDTH (GRID_SIZE * COLS)

#define ADDED_HEIGHT (5 * GRID_SIZE)

#define LIVE_NUM 4

#define LIVE 'l'
#define CHERRY 'c'
#define STRAWBERRY 's'
#define PEACH 'p'
#define APPLE 'a'
#define GRAPES 'g'
#define GALAXIAN 'x'
#define BELL 'b'
#define KEY 'k'

extern int maze[31][28];

class Entity : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT
  public:
	Entity(QGraphicsItem* parent = nullptr);
	int centerX();
	int centerY();

  protected:
	int currentDirection;
	int currentState;
  
	void moveLeft(int);
	void moveRight(int);
	void moveDown(int);
	void moveUp(int);
	
	void changeSide();
	
	int gridX();
	int gridY();
	
	int gridUp();
	int gridDown();
	int gridLeft();
	int gridRight();
	
	bool isInsideBox();
	bool isInsideTunnel();
	
	QList<QList<QPixmap>> animationList;
};
