#include "player.h"
#include <QDebug>
#include <iostream>

Player::Player(QGraphicsItem* parent) : Entity(parent)
{
	setPixmap(QPixmap("Images/sprites/PacMan/1.png").scaled(Px, Py));
	setPos(InitialPlayerPosX, InitialPlayerPosY);
	
	moveTimer = new QTimer(this);
	animationTimer = new QTimer(this);
	corectionTimer = new QTimer(this);
	deathTimer = new QTimer(this);
	
	animationTimer->start(PLAYER_MOVE_TIME * 10);
	currentDirection = 0;
	buffDirection = 0;
	inMove = false;
	alive = true;
	currentState = 1;
	deathState = 0;
	fillAnimationList();
	fillDeathAnimationList();
	
	connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));
	connect(animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
	connect(corectionTimer, SIGNAL(timeout()), this, SLOT(centeredPlayer()));
	connect(deathTimer, SIGNAL(timeout()), this, SLOT(die()));
	connect(this, SIGNAL(catched()), this, SLOT(startDeathAnimation()));
}

void Player::setDefault()
{
	setPixmap(QPixmap("Images/sprites/PacMan/1.png").scaled(Px, Py));
	setPos(InitialPlayerPosX, InitialPlayerPosY);
	currentDirection = 0;
	buffDirection = 0;
	inMove = false;
	alive = true;
	currentState = 1;
	deathState = 0;	
}

void Player::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Up && alive) {
		buffDirection = 'U';
		if (!corectionTimer->isActive())
			moveTimer->start(PLAYER_MOVE_TIME);
	}
	else if (event->key() == Qt::Key_Down && alive) {
		buffDirection = 'D';
		if (!corectionTimer->isActive())
			moveTimer->start(PLAYER_MOVE_TIME);
	}
	else if (event->key() == Qt::Key_Left && alive) {
		buffDirection = 'L';
		if (!corectionTimer->isActive())
			moveTimer->start(PLAYER_MOVE_TIME);
	}
	else if (event->key() == Qt::Key_Right && alive) {
		buffDirection = 'R';
		if (!corectionTimer->isActive())
			moveTimer->start(PLAYER_MOVE_TIME);
	}
}

void Player::centeredPlayer()
{
	if (currentDirection == 'U' || currentDirection == 'D') {
		if (centerY() % GRID_SIZE != GRID_SIZE / 2)
			if (centerY() % GRID_SIZE < GRID_SIZE / 2)
				setY(y() + 1);
			else
				setY(y() - 1);
		else {
			corectionTimer->stop();
			inMove = false;
		}
	}
	else if (currentDirection == 'L' || currentDirection == 'R')	{
		if (centerX() % GRID_SIZE != GRID_SIZE / 2)
			if (centerX() % GRID_SIZE < GRID_SIZE / 2)
				setX(x() + 1);
			else
				setX(x() - 1);
		else {
			corectionTimer->stop();
			inMove = false;
		}
	}
	emit playerPosChanged(QPointF(centerX(), centerY()));
	
	if (!inMove && currentDirection != buffDirection)
		moveTimer->start(PLAYER_MOVE_TIME);
}

void Player::checkMove()
{
	if (buffDirection == 'U') {
		if (gridUp() != WALL && gridUp() != BOX && centerX() % GRID_SIZE == GRID_SIZE / 2) {
			currentDirection = buffDirection;
			emit playerDirectionChanged(currentDirection);
		}
	}
	else if (buffDirection == 'D') {
		if (gridDown() != WALL && gridDown() != BOX && centerX() % GRID_SIZE == GRID_SIZE / 2) {
			currentDirection = buffDirection;
			emit playerDirectionChanged(currentDirection);
		}
	}
	else if (buffDirection == 'L') {
		if (gridLeft() != WALL && gridLeft() != BOX && centerY() % GRID_SIZE == GRID_SIZE / 2) {
			currentDirection = buffDirection;
			emit playerDirectionChanged(currentDirection);
		}
	}
	else if (buffDirection == 'R') {
		if (gridRight() != WALL && gridRight() != BOX && centerY() % GRID_SIZE == GRID_SIZE / 2) {
			currentDirection = buffDirection;
			emit playerDirectionChanged(currentDirection);
		}
	}
//	emit playerDirectionChanged(currentDirection);
}

void Player::move()
{	
	checkMove();
	
	if (currentDirection == 'U') {
		if (gridUp() != WALL && gridUp() != BOX) {
			inMove = true;
			moveUp(STEP);
			emit playerPosChanged(QPointF(centerX(), centerY()));
		}
		else {
			moveTimer->stop();
			corectionTimer->start(PLAYER_MOVE_TIME);
		}
	}
	else if (currentDirection == 'D') {
		if (gridDown() != WALL && gridDown() != BOX) {
			inMove = true;
			moveDown(STEP);
			emit playerPosChanged(QPointF(centerX(), centerY()));
		}
		else {
			moveTimer->stop();
			corectionTimer->start(PLAYER_MOVE_TIME);
		}
	}
	else if (currentDirection == 'L') {
		if (gridLeft() != WALL && gridLeft() != BOX) {
			inMove = true;
			moveLeft(STEP);
			changeSide();
			emit playerPosChanged(QPointF(centerX(), centerY()));
		}
		else {
			moveTimer->stop();
			corectionTimer->start(PLAYER_MOVE_TIME);
		}
	}
	else if (currentDirection == 'R') {
		if (gridRight() != WALL && gridRight() != BOX) {
			inMove = true;
			moveRight(STEP);
			changeSide();
			emit playerPosChanged(QPointF(centerX(), centerY()));
		}
		else {
			moveTimer->stop();
			corectionTimer->start(PLAYER_MOVE_TIME);
		}
	}
}

void Player::changeState()
{
	if (currentState < 3)
		currentState++;
	else
		currentState = 0;
}

void Player::animate()
{
	if (inMove) {
		if (currentDirection == 'U') {
			setPixmap(animationList.at(0).at(currentState));
			changeState();
		}
		else if (currentDirection == 'D') {
			setPixmap(animationList.at(1).at(currentState));
			changeState();
		}
		else if (currentDirection == 'L') {
			setPixmap(animationList.at(2).at(currentState));
			changeState();
		}
		else if (currentDirection == 'R') {
			setPixmap(animationList.at(3).at(currentState));
			changeState();
		}
	}
}

void Player::startDeathAnimation()
{
	currentDirection = 0;
	inMove = false;
	alive = false;
	moveTimer->stop();
	corectionTimer->stop();
	deathTimer->start(DEATH_TIME);
}

void Player::die()
{
	if (deathState < 12)
		setPixmap(deathAnimation.at(deathState++));
	else {
		deathTimer->stop();
		
		emit newTry();
	}
}

void Player::stop()
{
	moveTimer->stop();
	corectionTimer->stop();
	inMove = false;
	alive = false;
	setPixmap(QPixmap("Images/sprites/PacMan/1.png").scaled(Px, Py));
}

void Player::fillAnimationList()
{
	QList<QPixmap> listUp;
	listUp << QPixmap("Images/sprites/PacMan/1.png").scaled(Px, Py);
	listUp << QPixmap("Images/sprites/PacMan/UP_2.png").scaled(Px, Py);
	listUp << QPixmap("Images/sprites/PacMan/UP_3.png").scaled(Px, Py);
	listUp << QPixmap("Images/sprites/PacMan/UP_2.png").scaled(Px, Py);

	QList<QPixmap> listDown;
	listDown << QPixmap("Images/sprites/PacMan/1.png").scaled(Px, Py);
	listDown << QPixmap("Images/sprites/PacMan/DOWN_2.png").scaled(Px, Py);
	listDown << QPixmap("Images/sprites/PacMan/DOWN_3.png").scaled(Px, Py);
	listDown << QPixmap("Images/sprites/PacMan/DOWN_2.png").scaled(Px, Py);

	QList<QPixmap> listLeft;
	listLeft << QPixmap("Images/sprites/PacMan/1.png").scaled(Px, Py);
	listLeft << QPixmap("Images/sprites/PacMan/LEFT_2.png").scaled(Px, Py);
	listLeft << QPixmap("Images/sprites/PacMan/LEFT_3.png").scaled(Px, Py);
	listLeft << QPixmap("Images/sprites/PacMan/LEFT_2.png").scaled(Px, Py);

	QList<QPixmap> listRight;
	listRight << QPixmap("Images/sprites/PacMan/1.png").scaled(Px, Py);
	listRight << QPixmap("Images/sprites/PacMan/RIGHT_2.png").scaled(Px, Py);
	listRight << QPixmap("Images/sprites/PacMan/RIGHT_3.png").scaled(Px, Py);
	listRight << QPixmap("Images/sprites/PacMan/RIGHT_2.png").scaled(Px, Py);

	animationList << listUp << listDown << listLeft << listRight;
}

void Player::fillDeathAnimationList()
{
//	deathAnimation << QPixmap("Images/sprites/PacMan/1.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/UP_2.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/1.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/2.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/3.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/4.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/5.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/6.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/7.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/8.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/9.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/10.png").scaled(Px, Py);
	deathAnimation << QPixmap("Images/sprites/PacMan/death/11.png").scaled(Px, Py);
}
