#pragma once
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QSoundEffect>
#include <QPainter>
#include <QColor>
#include <QDebug>

#include "dot.h"
#include "energizer.h"
#include "icon.h"
#include "player.h"
#include "blinky.h"
#include "pinky.h"
#include "inky.h"
#include "clyde.h"
#include "maze.h"


class GameBoard : public QGraphicsScene {
	Q_OBJECT

  public:
	GameBoard(QObject* parent = nullptr);
	
  public slots:
	void checkIfEaten();
	void checkIfTouched();
	void checkWin();
	void setMapAfterWin();
	void setNewTry();
	void updateBlinkyPos(QPointF);
	void updatePlayerPos(QPointF);
	void updatePlayerDirection(int);
	void changeGhostMode();
	void changeGhostsToFrightenedMode();
	void startGame();
	void letGhostsOut();
	void checkDotThresholds();
	void removeFruit();
	void updateTargets(); //used only for test purpose
	
  signals:
	void win();

  private:
    void setMapBasic();
	void placeDots();
	void placeEnergizers();
	void placeLives();
	void placeFruitInBar();
	void dead();
	void setIntervals();
	void increaseScore();
	void initializeScoreText();
	void updateScoreText();
	void initializeHighscoreText();
	void updateHighscoreText();
	void initializeLevelText();
	void updateLevelText();
	void saveHighscore();
	void loadHighscore();
	void checkIfHighscoreExceeded();
	void decreaseLiveByOne();
	void addOneLive();
	void gameOver();
	void createReadyText();
	void loadSoundEffects();
	void testTargetGirds(); //used only for test purpose
	QPixmap createCirclePixmap(const QColor&, int); //used only for test purpose
	
	int score;
	int highscore;
	int level;

	int dotCounter;
	int ghostsEatenCounter;
	int liveCounter;
	bool liveAdded;
	bool isFruitOnMap;
	int ghostNum;
	int fruitCounter;
	
	QList<char> fruitList;
	QList<Icon*> fruitsBar;
	QList<Energizer*> energizers;
	QList<Dot*> dots;
	QList<Ghost*> ghosts;
	QList<Icon*> lives;
	QQueue<int> intervals;
	QTimer* eatenTimer;
	QTimer* modeTimer;
	QTimer* winTimer;
	QTimer* startTimer;
	QTimer* exitingTimer;
	QTimer* fruitTimer;
	
	QGraphicsTextItem* scoreText;
	QGraphicsTextItem* highscoreText;
	QGraphicsTextItem* readyText;
	QGraphicsTextItem* levelText;
	
	QSoundEffect* eatenGhostSound;
	QSoundEffect* extraLiveSound;
	QSoundEffect* eatenDotSound;
	QSoundEffect* beginningSound;
	QSoundEffect* deathSound;
	QSoundEffect* eatenFruitSound;
	
	QRectF fruitHitbox;
	
	Player* player;
	Blinky* blinky;
	Pinky* pinky;
	Inky* inky;
	Clyde* clyde;
	Icon* fruit;
	
	/*used only for test purpose*/
	QGraphicsPixmapItem* testBlinky;
	QGraphicsPixmapItem* testPinky;
	QGraphicsPixmapItem* testInky;
	QGraphicsPixmapItem* testClyde;
};
