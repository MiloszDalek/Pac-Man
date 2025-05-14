#include "ghost.h"

Ghost::Ghost(QGraphicsItem* parent) : Entity(parent)
{
	animationTimer = new QTimer(this);
	
	moveTimer = new QTimer(this);
	
	frightenedModeTimer = new QTimer(this);
	
	frightenedAnimationTimer = new QTimer(this);
	
	playerPos = QPointF(InitialPlayerPosX, InitialPlayerPosY);
	playerDirection = 'D';
	currentState = 0;
	dotThreshold = 0;
	mode = 's'; //	chase or scatter
	frightenedState = 'B'; // white or blue
	isFrightened = false;
	isEaten = false;
	exited = false;
	
	fillEyesList();
	fillFrightenedAnimationList();
	setFrightenedAnimationIntervals();
	
	connect(frightenedAnimationTimer, SIGNAL(timeout()), this, SLOT(changeFrightenedState()));
	connect(animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
	connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));
	connect(frightenedModeTimer, SIGNAL(timeout()), this, SLOT(setNormalMode()));
}

void Ghost::animate()
{
	if (isFrightened) {
		if (frightenedState == 'B') {
			setPixmap(frightenedAnimationList.at(0).at(currentState));
			changeState();
		}
		else if (frightenedState == 'W') {
			setPixmap(frightenedAnimationList.at(1).at(currentState));
			changeState();
		}
	}
	else if (isEaten) {
		if (currentDirection == 'U') {
			setPixmap(eyesList.at(0));
		}
		else if (currentDirection == 'D') {
			setPixmap(eyesList.at(1));
		}
		else if (currentDirection == 'L') {
			setPixmap(eyesList.at(2));
		}
		else if (currentDirection == 'R') {
			setPixmap(eyesList.at(3));
		}
	}
	else {
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

void Ghost::setDotThreshold(int threshold)
{
    dotThreshold = threshold;
}

void Ghost::checkEatenDotsThreshold(int dotsEaten)
{
    if (dotsEaten == dotThreshold)
		exited = true;
}

void Ghost::checkEatenDotsThresholdAlreadyExceeded(int dotsEaten)
{
  if (dotsEaten >= dotThreshold)
		exited = true;
}

void Ghost::changeState()
{
	if (currentState)
		currentState--;
	else
		currentState++;
}

void Ghost::changeFrightenedState()
{
	if (frightenedState == 'W')
		frightenedState = 'B';
	else if (frightenedState == 'B')
		frightenedState = 'W';
		
	if (!frightenedAnimationIntervals.isEmpty())
		frightenedAnimationTimer->start(frightenedAnimationIntervals.dequeue());
	else
		frightenedAnimationTimer->stop();
}

void Ghost::changeMode()
{
	if (mode == 's')
		mode = 'c';
	else if (mode == 'c')
		mode = 's';
}

void Ghost::setFrightenedMode()
{
	if (!isFrightened && !isEaten) {	
		isFrightened = true;
		frightenedState = 'B';
		changeDirectionToOpposite();
		changeMoveSpeed(FRIGHTENED_SPEED);
		frightenedModeTimer->start(FRIGHTENED_MODE_TIME);
		setFrightenedAnimationIntervals();
		frightenedAnimationTimer->start(frightenedAnimationIntervals.dequeue());
	}
	else if (!isEaten) {
		frightenedState = 'B';
		frightenedModeTimer->start(FRIGHTENED_MODE_TIME);
		setFrightenedAnimationIntervals();
		frightenedAnimationTimer->start(frightenedAnimationIntervals.dequeue());
	}
}

void Ghost::setNormalMode()
{
	if (isFrightened) {
		isFrightened = false;
		frightenedModeTimer->stop();
		if (isInsideBox())
			changeMoveSpeed(INSIDE_BOX_SPEED);
		else
			changeMoveSpeed(NORMAL_SPEED);

	}
}

void Ghost::changeDirectionToOpposite()
{
	if (exited && !isInsideBox() && !isInsideTunnel()) {
		if (currentDirection == 'U') {
			if (gridDown() != WALL && gridDown() != BOX) {
				nextRow = gridY() + 1;
				currentDirection = 'D';
			}
			else {
				nextRow = gridY();
				currentDirection = 'D';
			}
		}
		else if (currentDirection == 'D') {
			if (gridUp() != WALL && gridUp() != BOX) {
				nextRow = gridY() - 1;
				currentDirection = 'U';
			}
			else {
				nextRow = gridY();
				currentDirection = 'U';
			}
		}
		else if (currentDirection == 'L') {
			if (gridRight() != WALL && gridRight() != BOX) {
				nextCol = gridX() + 1;
				currentDirection = 'R';
			}
			else {
				nextCol = gridX();
				currentDirection = 'R';
			}
		}
		else if (currentDirection == 'R') {
			if (gridLeft() != WALL && gridLeft() != BOX) {
				nextCol = gridX() - 1;
				currentDirection = 'L';
			}
			else {
				nextCol = gridX();
				currentDirection = 'L';
			}
		}
	}
}

bool Ghost::moveToGridCenter()
{
	if (GRID_SIZE * nextRow - GRID_SIZE / 2 < y())
		moveUp(STEP);
	else if (GRID_SIZE * nextRow - GRID_SIZE / 2 > y())
		moveDown(STEP);
	else if (GRID_SIZE * nextCol - GRID_SIZE / 2 < x()) {
		moveLeft(STEP);
	}
	else if (GRID_SIZE * nextCol - GRID_SIZE / 2 > x()) {
		moveRight(STEP);
	}	
		
	if (GRID_SIZE * nextRow - GRID_SIZE / 2 == y() && GRID_SIZE * nextCol - GRID_SIZE / 2 == x())
		return true;
	else
		return false;
}

void Ghost::chooseRandomNextGrid()
{
	int direction = QRandomGenerator::global()->bounded(4);
	 
	if (direction == 0 && gridUp() != WALL && gridUp() != BOX && currentDirection != 'D') {
        nextRow = gridY() - 1;
        currentDirection = 'U';
    }
    else if (direction == 1 && gridDown() != WALL && gridDown() != BOX && currentDirection != 'U') {
        nextRow = gridY() + 1;
        currentDirection = 'D';
    }
    else if (direction == 2 && gridLeft() != WALL && gridLeft() != BOX && currentDirection != 'R') {
        nextCol = gridX() - 1;
        if (gridX() == -2) {
			nextCol = 28;
			changeSide();
		}
        currentDirection = 'L';
    }
    else if (direction == 3 && gridRight() != WALL && gridRight() != BOX && currentDirection != 'L') {
        nextCol = gridX() + 1;
        if (gridX() == 29) {
			nextCol = 0;
			changeSide();
		}
        currentDirection = 'R';
    }
    else {
        chooseRandomNextGrid();
    }
}

void Ghost::chooseNextGrid()
{
	chooseTargetGrid();
	
	double upDist = 10000;
	double downDist = 10000;
	double leftDist = 10000;
	double rightDist = 10000;
	
	if (gridUp() != WALL && gridUp() != BOX && currentDirection != 'D') {
		upDist = sqrt(pow((gridX() - targetCol), 2) + pow((gridY() - 1 - targetRow), 2));
	}
	if (gridDown() != WALL && gridDown() != BOX && currentDirection != 'U') {
		downDist = sqrt(pow((gridX() - targetCol), 2) + pow((gridY() + 1 - targetRow), 2));
	}
	if (gridLeft() != WALL && gridLeft() != BOX && currentDirection != 'R') {
		leftDist = sqrt(pow((gridX() - 1 - targetCol), 2) + pow((gridY() - targetRow), 2));
	}
	if (gridRight() != WALL && gridRight() != BOX && currentDirection != 'L') {
		rightDist = sqrt(pow((gridX() + 1 - targetCol), 2) + pow((gridY() - targetRow), 2));
	}
	
	if (upDist <= downDist && upDist <= leftDist && upDist <= rightDist) {
		nextRow = gridY() - 1;
		currentDirection = 'U';
	}
	else if (downDist < upDist && downDist < leftDist && downDist <= rightDist) {
		nextRow = gridY() + 1;
		currentDirection = 'D';
	}
	else if (leftDist <= downDist && leftDist < upDist && leftDist <= rightDist) {
		nextCol = gridX() - 1;
		if (gridX() == -2) {
			nextCol = 28;
			changeSide();
		}
		currentDirection = 'L';
	}
	else if (rightDist < downDist && rightDist < leftDist && rightDist < upDist) {
		nextCol = gridX() + 1;
		if (gridX() == 29) {
			nextCol = 0;
			changeSide();
		}
		currentDirection = 'R';
	}
}

void Ghost::moveWithinBox()
{
	if (currentDirection == 'U' && gridUp() == WALL) {
        currentDirection = 'D';
    }
    else if (currentDirection == 'D' && gridDown() == WALL) {
        currentDirection = 'U';
    }

    if (currentDirection == 'U') {
        moveUp(STEP);
    }
    else if (currentDirection == 'D') {
        moveDown(STEP);
    }
}

bool Ghost::moveToCenterOfBox()
{
	if (centerX() != 14 * GRID_SIZE) {
		if (centerX() < 14 * GRID_SIZE) {
			moveRight(STEP);
			currentDirection = 'R';
		}
		else if (centerX() > 14 * GRID_SIZE) {
			moveLeft(STEP);
			currentDirection = 'L';
		}
		return false;
	}
	else
		return true;
}

void Ghost::moveOutOfBox()
{
	moveUp(STEP);
	currentDirection = 'U';
	if (!isInsideBox()) {
		if (isFrightened)
			changeMoveSpeed(FRIGHTENED_SPEED);
		else	
			changeMoveSpeed(NORMAL_SPEED);
		if (nextCol == 14)
			currentDirection = 'R';
		else if (nextCol == 13)
			currentDirection = 'L';
	}
}

void Ghost::moveIntoBox()
{
	moveDown(STEP);
	currentDirection = 'D';
	
	if (centerY() == 15 * GRID_SIZE) {
		isEaten = false;
		changeMoveSpeed(INSIDE_BOX_SPEED);
	}
}

bool Ghost::isAboveBoxWhileEaten()
{
	return isEaten && centerX() == 14 * GRID_SIZE && centerY() >= 11 * GRID_SIZE && centerY() <= 15 * GRID_SIZE;
}

void Ghost::move()
{
	if (exited) {
		if (isInsideBox() && !isEaten) {
			if (moveToCenterOfBox()) {
				moveOutOfBox();
			}
		}
		else {
			if (isFrightened) {
				if (moveToGridCenter()) {
					chooseRandomNextGrid();
				}
			}
			else {
				if (isAboveBoxWhileEaten()) {
					if (moveToCenterOfBox()) {
						moveIntoBox();
					}
				}
				else if (moveToGridCenter()) {
					chooseNextGrid();
					if (isInsideTunnel() && !isEaten)
						changeMoveSpeed(TUNNEL_SPEED);
					else {
						if (!isEaten)
							changeMoveSpeed(NORMAL_SPEED);
						else
							changeMoveSpeed(EYE_SPEED);
					}
				}
			}
		}
	}
	else {
		moveWithinBox();
	}
}

void Ghost::changeMoveSpeed(int moveTime)
{
	moveTimer->start(moveTime);
}

void Ghost::setStartSpeed()
{
	if (isInsideBox())
		moveTimer->start(INSIDE_BOX_SPEED);
	else
		moveTimer->start(NORMAL_SPEED);
		
	animationTimer->start(ANIMATION_TIME);
}

void Ghost::stop()
{
	moveTimer->stop();
	animationTimer->stop();
	frightenedModeTimer->stop();
}

void Ghost::setPlayerPos(QPointF newPos)
{
	playerPos = newPos;
}

void Ghost::setPlayerDirection(int newPlayerDirection)
{
	playerDirection = newPlayerDirection;
}

bool Ghost::isFrightenedCheck()
{
	return isFrightened;
}

bool Ghost::isEatenCheck()
{
	return isEaten;
}

void Ghost::setEaten()
{
	isEaten = true;
	isFrightened = false;
	changeMoveSpeed(EYE_SPEED);
}

void Ghost::setFrightenedAnimationIntervals()
{
	frightenedAnimationIntervals.clear();
	frightenedAnimationIntervals.enqueue(3500);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
	frightenedAnimationIntervals.enqueue(250);
}

void Ghost::fillFrightenedAnimationList()
{
	QList<QPixmap> blueList;
	blueList << QPixmap("Images/sprites/Frightened/FRIGHTENED_1.png").scaled(Px, Py);
	blueList << QPixmap("Images/sprites/Frightened/FRIGHTENED_3.png").scaled(Px, Py);
	
	QList<QPixmap> whiteList;
	whiteList << QPixmap("Images/sprites/Frightened/FRIGHTENED_2.png").scaled(Px, Py);
	whiteList << QPixmap("Images/sprites/Frightened/FRIGHTENED_4.png").scaled(Px, Py);
	
	frightenedAnimationList << blueList << whiteList;
}

void Ghost::fillEyesList()
{
	eyesList << QPixmap("Images/sprites/Eyes/EYES_UP.png").scaled(Px, Py);
	eyesList << QPixmap("Images/sprites/Eyes/EYES_DOWN.png").scaled(Px, Py);
	eyesList << QPixmap("Images/sprites/Eyes/EYES_LEFT.png").scaled(Px, Py);
	eyesList << QPixmap("Images/sprites/Eyes/EYES_RIGHT.png").scaled(Px, Py);
}

QPointF Ghost::returnTargetGrid()
{
	return QPointF(targetCol, targetRow);
}
