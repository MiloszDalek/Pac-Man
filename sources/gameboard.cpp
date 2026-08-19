#include "gameboard.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

EM_JS(void, saveHighscoreWeb, (int score), {
    localStorage.setItem("pacman_highscore", score.toString());
});

EM_JS(int, loadHighscoreWeb, (), {
    const score = localStorage.getItem("pacman_highscore");

    if (score === null) {
        return 0;
    }

    return parseInt(score);
});

GameBoard* webGameBoard = nullptr;

extern "C" {
    EMSCRIPTEN_KEEPALIVE
        void handleWebKey(int key)
    {
        if (webGameBoard)
            webGameBoard->handleWebKey(key);
    }

		EMSCRIPTEN_KEEPALIVE
		bool isGameOverWeb()
		{
			return webGameBoard && webGameBoard->isGameOver();
		}
}

#endif


GameBoard::GameBoard(QObject* parent) : QGraphicsScene(parent), score(0),
	highscore(0), level(1), dotCounter(0), ghostsEatenCounter(0),
	liveCounter(LIVE_NUM), liveAdded(false), isFruitOnMap(false), inputEnabled(false),
	gameOverActive(false),
	ghostNum(0), fruitCounter(0)
{
    #ifdef __EMSCRIPTEN__
        webGameBoard = this;
    #endif

    soundManager = new SoundManager();

    mazeImage = new Maze();
    addItem(mazeImage);
	
	eatenTimer = new QTimer(this);
	modeTimer = new QTimer(this);
	winTimer = new QTimer(this);
	startTimer = new QTimer(this);
	exitingTimer = new QTimer(this);
	fruitTimer = new QTimer(this);
	connect(eatenTimer, SIGNAL(timeout()), this, SLOT(checkIfEaten()));
	connect(eatenTimer, SIGNAL(timeout()), this, SLOT(checkIfTouched()));
	connect(eatenTimer, SIGNAL(timeout()), this, SLOT(checkDotThresholds()));
	connect(eatenTimer, SIGNAL(timeout()), this, SLOT(checkWin()));
	connect(winTimer, SIGNAL(timeout()), this, SLOT(setMapAfterWin()));
	connect(startTimer, SIGNAL(timeout()), this, SLOT(startGame()));
	connect(exitingTimer, SIGNAL(timeout()), this, SLOT(letGhostsOut()));
	connect(fruitTimer, SIGNAL(timeout()), this, SLOT(removeFruit()));
	
	placeDots();
	placeEnergizers();
	placeLives();
	
	loadHighscore();	
	initializeScoreText();
	initializeHighscoreText();
	initializeLevelText();
	createReadyText();
	
	fruitHitbox = QRectF(FruitPosX + GRID_SIZE / 2, FruitPosY + GRID_SIZE / 2, GRID_SIZE, GRID_SIZE);
	fruit = new Icon();
	fruit->setZValue(1);
	
	player = new Player();
	player->setZValue(2);
	addItem(player);
	
	blinky = new Blinky();
	blinky->setZValue(3);
	addItem(blinky);
	blinky->setDotThreshold(BLINKY_THRESHOLD);
	
	pinky = new Pinky();
	pinky->setZValue(3);
	addItem(pinky);
	pinky->setDotThreshold(PINKY_THRESHOLD);
	
	inky = new Inky();
	inky->setZValue(3);
	addItem(inky);
	inky->setDotThreshold(INKY_THRESHOLD);
	
	clyde = new Clyde();
	clyde->setZValue(3);
	addItem(clyde);
	clyde->setDotThreshold(CLYDE_THRESHOLD);
	
	ghosts << blinky << pinky << inky << clyde;
	fruitList << CHERRY << STRAWBERRY << PEACH << APPLE << GRAPES << GALAXIAN << BELL << KEY;
	
	setIntervals();
	connect(modeTimer, SIGNAL(timeout()), this, SLOT(changeGhostMode()));
	connect(player, SIGNAL(newTry()), this, SLOT(setNewTry()));
	connect(player, QOverload<QPointF>::of(&Player::playerPosChanged), this, &GameBoard::updatePlayerPos);
	connect(player, QOverload<int>::of(&Player::playerDirectionChanged), this, &GameBoard::updatePlayerDirection);
	connect(blinky, QOverload<QPointF>::of(&Blinky::blinkyPosChanged), this, &GameBoard::updateBlinkyPos);
    connect(this, SIGNAL(win()), mazeImage, SLOT(startAnimation()));
	
	addItem(readyText);
	
	eatenTimer->start(PLAYER_MOVE_TIME);
    soundManager->playBeginningSound();
	startTimer->start(BEGIN_TIME);
	
    // testTargetGirds();
}

#ifdef __EMSCRIPTEN__
void GameBoard::handleWebKey(int key)
{
	if (inputEnabled)
		player->handleDirection(key);
}
#endif

#ifdef __EMSCRIPTEN__
bool GameBoard::isGameOver() const
{
	return gameOverActive;
}
#endif

void GameBoard::initializeScoreText()
{
	scoreText = new QGraphicsTextItem();
	
    int fontId = QFontDatabase::addApplicationFont(":/Images/Emulogic-zrEw.ttf");
	QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
	QFont font(fontFamily, FONT_SIZE);
	
	scoreText->setFont(font);
	scoreText->setDefaultTextColor(Qt::white);
	addItem(scoreText);
	
	updateScoreText();
}

void GameBoard::updateScoreText()
{
	QString scoreString = QString("%1").arg(score);
	
    int textWidth = 6 * GRID_SIZE;
    QFontMetrics metrics(scoreText->font());
    int textX = textWidth - metrics.horizontalAdvance(scoreString);

    scoreText->setPlainText(scoreString);
    scoreText->setPos(GRID_SIZE + textX, -2 * GRID_SIZE);
}

void GameBoard::initializeLevelText()
{
	levelText = new QGraphicsTextItem();
	
    int fontId = QFontDatabase::addApplicationFont(":/Images/Emulogic-zrEw.ttf");
	QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
	QFont font(fontFamily, FONT_SIZE);
	
	levelText->setFont(font);
	levelText->setDefaultTextColor(Qt::white);

	levelText->setPos(MAP_WIDTH - GRID_SIZE * 8, -2 * GRID_SIZE);
	addItem(levelText);
	
	updateLevelText();
}

void GameBoard::updateLevelText()
{
	QString levelString = QString("lvl %1").arg(level);
	
	levelText->setPlainText(levelString);
}

void GameBoard::initializeHighscoreText()
{
	highscoreText = new QGraphicsTextItem();
	
	QGraphicsTextItem* label = new QGraphicsTextItem();
	
    int fontId = QFontDatabase::addApplicationFont(":/Images/Emulogic-zrEw.ttf");
	QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
	QFont font(fontFamily, FONT_SIZE);
	
	label->setFont(font);
	label->setDefaultTextColor(Qt::white);
	QString labelString = QString("HIGHSCORE");
	QFontMetrics metrics(label->font());
	int labelX = MAP_WIDTH / 2 - metrics.horizontalAdvance(labelString) / 2;
	label->setPlainText(labelString);
	label->setPos(labelX, -3 * GRID_SIZE - 2);
	addItem(label);
	
	highscoreText->setFont(font);
	highscoreText->setDefaultTextColor(Qt::white);
	addItem(highscoreText);
	
	updateHighscoreText();
}

void GameBoard::updateHighscoreText()
{
	QString highscoreString = QString("%1").arg(highscore);

    QFontMetrics metrics(highscoreText->font());
    int textX = MAP_WIDTH / 2 - metrics.horizontalAdvance(highscoreString) / 2;

    highscoreText->setPlainText(highscoreString);
    highscoreText->setPos(textX, -2 * GRID_SIZE);
}

void GameBoard::saveHighscore() {

#ifdef __EMSCRIPTEN__

    saveHighscoreWeb(highscore);

#else

    QFile file("highscore.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << highscore;
    }

#endif

}

void GameBoard::loadHighscore() {

#ifdef __EMSCRIPTEN__

    highscore = loadHighscoreWeb();

#else

    QFile file("highscore.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in >> highscore;
    }

#endif

}

void GameBoard::checkIfHighscoreExceeded()
{
	updateScoreText();
	
	if (score > highscore) {
		highscore = score;
		updateHighscoreText();
		saveHighscore();
	}
	
	if (score >= EXTRA_LIVE_THRESHOLD && !liveAdded) {
		addOneLive();
		liveAdded = true;
	}
}

void GameBoard::placeLives()
{
	for (int i = 0; i < liveCounter; i++) {
		Icon* live = new Icon(nullptr, LIVE);
		lives << live;
		live->setPos(GRID_SIZE / 2 + 2 * GRID_SIZE * i, MAP_HEIGHT);
		addItem(live);
	}
}

void GameBoard::addOneLive()
{
	Icon* live = new Icon(nullptr, LIVE);
	lives << live;
	live->setPos(GRID_SIZE / 2 + 2 * GRID_SIZE * liveCounter, MAP_HEIGHT);
	addItem(live);
	liveCounter++;
    soundManager->playExtraLiveSound();
}

void GameBoard::placeFruitInBar()
{ 
	int fruitNum;
	
	if (level <= 8)
		fruitNum = level - 1;
	else
		fruitNum = 7;
		
	Icon* fruit = new Icon(nullptr, fruitList[fruitNum]);

	if (fruitsBar.size() >= MAX_FRUITS_NUM) {
		removeItem(fruitsBar.first());
		delete fruitsBar.first();
		fruitsBar.removeFirst();
		
		foreach (Icon* fruit, fruitsBar) {
			fruit->setX(fruit->x() + GRID_SIZE * 2);
		}
	}
	
	fruit->setPos(MAP_WIDTH - fruitsBar.size() * GRID_SIZE * 2 - GRID_SIZE * 2, MAP_HEIGHT);
	fruitsBar << fruit;
	addItem(fruit);
}

void GameBoard::placeDots()
{
	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLS; x++) {
            if (maze[y][x] == 3) {
				Dot* dot = new Dot();
				dots << dot;
				dot->setPos(GRID_SIZE * x + 8, GRID_SIZE * y + 8);
				addItem(dot);
			}
		}
	}
}

void GameBoard::placeEnergizers()
{
	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLS; x++) {
			if (maze[y][x] == 4) {
				Energizer* energizer = new Energizer();
				energizers << energizer;
				energizer->setPos(GRID_SIZE * x, GRID_SIZE * y);
				addItem(energizer);
			}
		}
	}
}

void GameBoard::checkIfEaten()
{
	QRectF playerHitbox = QRectF(player->x() + GRID_SIZE / 2, player->y() + GRID_SIZE / 2, GRID_SIZE, GRID_SIZE);
	
	foreach (Dot* dot, dots) {
		QPointF dotPos(dot->centerX(), dot->centerY());
		
		if(playerHitbox.contains(dotPos)) {
            soundManager->playEatenDotSound();
			removeItem(dot);
			dots.removeOne(dot);
			delete dot;
			dotCounter++;
			score += 10;
			checkIfHighscoreExceeded();
		}
	}
	
	foreach (Energizer* energizer, energizers) {
		QPointF enerPos(energizer->centerX(), energizer->centerY());
		
		if(playerHitbox.contains(enerPos)) {
			removeItem(energizer);
			energizers.removeOne(energizer);
			delete energizer;
			score += 50;
			ghostsEatenCounter = 0;
			changeGhostsToFrightenedMode();
			checkIfHighscoreExceeded();
		}
	}
}

void GameBoard::checkDotThresholds()
{
	foreach (Ghost* ghost, ghosts) {
		ghost->checkEatenDotsThreshold(dotCounter);	
	}
	int fruitNum;
	if (dotCounter == FIRST_FRUIT_THRESHOLD && fruitCounter == 0) {
		
		if (level <= 8)
			fruitNum = level - 1;
		else
			fruitNum = 7;
			
		fruitCounter++;
		fruit->setFruit(fruitList[fruitNum]);
		addItem(fruit);
		isFruitOnMap = true;
	}
	else if (dotCounter == SECOND_FRUIT_THRESHOLD && fruitCounter  == 1) {
		
		if (level <= 8)
			fruitNum = level - 1;
		else
			fruitNum = 7;		
		
		fruitCounter++;
		fruit->setFruit(fruitList[fruitNum]);
		addItem(fruit);
		isFruitOnMap = true;
		fruitTimer->start(FRUIT_DESPAWN_TIME);
	}
	
	if (isFruitOnMap) {
		QRectF playerHitbox = QRectF(player->centerX() - GRID_SIZE / 2,
				player->centerY() - GRID_SIZE / 2, GRID_SIZE, GRID_SIZE);
		
		if (playerHitbox.intersects(fruitHitbox)) {
            soundManager->playEatenFruitSound();
			score += fruit->catched();
			checkIfHighscoreExceeded();
			isFruitOnMap = false;
			placeFruitInBar();
			fruitTimer->start(FRUIT_DESPAWN_TIME / 2);
		}
	}
}

void GameBoard::removeFruit()
{
	fruitTimer->stop();
	isFruitOnMap = false;
	removeItem(fruit);
}

void GameBoard::changeGhostsToFrightenedMode()
{
	foreach (Ghost* ghost, ghosts) {
		ghost->setFrightenedMode();
	}
}

void GameBoard::checkIfTouched()
{
	QRectF playerHitbox = QRectF(player->centerX() - 2, player->centerY() - 2, 4, 4);
	
	foreach (Ghost* ghost, ghosts) {
		QRectF ghostHitbox = QRectF(ghost->centerX() - 2, ghost->centerY() - 2, 4, 4);
		if (playerHitbox.intersects(ghostHitbox)) {
			if (!ghost->isFrightenedCheck() && !ghost->isEatenCheck()) {
				dead();
				break;
			}
		}
	}
	
	playerHitbox = QRectF(player->centerX() - GRID_SIZE / 2, player->centerY() - GRID_SIZE / 2, GRID_SIZE, GRID_SIZE);
	
	foreach (Ghost* ghost, ghosts) {
		QRectF ghostHitbox = QRectF(ghost->centerX() - GRID_SIZE / 2, ghost->centerY() - GRID_SIZE / 2, GRID_SIZE, GRID_SIZE);
		if (playerHitbox.intersects(ghostHitbox)) {
			if (ghost->isFrightenedCheck() && !ghost->isEatenCheck()) {
                soundManager->playEatenGhostSound();
				ghost->setEaten();
				Icon* point = new Icon(nullptr, ghostsEatenCounter);
				point->setPos(ghost->x(), ghost->y());
				addItem(point);
				ghostsEatenCounter++;
				increaseScore();
				checkIfHighscoreExceeded();
			}
		}
	}
}

void GameBoard::dead()
{
	foreach (Ghost* ghost, ghosts) {
		removeItem(ghost);
	}
	modeTimer->stop();
	eatenTimer->stop();
	exitingTimer->stop();
	decreaseLiveByOne();
	
	emit player->catched();
    soundManager->playDeathSound();
}

void GameBoard::decreaseLiveByOne()
{
	if (liveCounter > 0) {
		delete lives.at(--liveCounter);
		lives.removeAt(liveCounter);
	}
	else
		liveCounter--;
}

void GameBoard::increaseScore()
{
	score += pow(2, ghostsEatenCounter) * 100;
}

void GameBoard::updatePlayerPos(QPointF newPos)
{
	foreach (Ghost* ghost, ghosts) {
		ghost->setPlayerPos(newPos);
	}
}

void GameBoard::updateBlinkyPos(QPointF newPos)
{
	inky->setBlinkyPos(newPos);
}

void GameBoard::updatePlayerDirection(int newPlayerDirection)
{
	foreach (Ghost* ghost, ghosts) {
		ghost->setPlayerDirection(newPlayerDirection);
	}

}

void GameBoard::setIntervals()
{
	intervals.clear();
	intervals.enqueue(7000);
	intervals.enqueue(20000);
	intervals.enqueue(7000);
	intervals.enqueue(20000);
	intervals.enqueue(5000);
	intervals.enqueue(20000);
	intervals.enqueue(5000);
}

void GameBoard::changeGhostMode()
{
	foreach (Ghost* ghost, ghosts) {
		ghost->changeMode();
		if (!ghost->isFrightenedCheck() && !ghost->isEatenCheck())
			ghost->changeDirectionToOpposite();
	}
	if (!intervals.isEmpty())
		modeTimer->start(intervals.dequeue());
	else
		modeTimer->stop();
}

void GameBoard::gameOver()
{
	gameOverActive = true;
	QGraphicsTextItem* gameOverInfo = new QGraphicsTextItem();
	
    int fontId = QFontDatabase::addApplicationFont(":/Images/Emulogic-zrEw.ttf");
	QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
	QFont font(fontFamily, FONT_SIZE);
	
	gameOverInfo->setFont(font);
	gameOverInfo->setDefaultTextColor(Qt::red);
	QString gameOverString = QString("GAME OVER");
	QFontMetrics metrics(gameOverInfo->font());
	int gameOverInfoX = MAP_WIDTH / 2 - metrics.horizontalAdvance(gameOverString) / 2;
	gameOverInfo->setPlainText(gameOverString);
	gameOverInfo->setPos(gameOverInfoX, MAP_HEIGHT / 2 + GRID_SIZE);
	addItem(gameOverInfo);
	removeItem(player);
	
	foreach(Ghost* ghost, ghosts) {
		delete ghost;
	}
	delete player;
	delete fruit;
}

void GameBoard::createReadyText()
{
	readyText = new QGraphicsTextItem();
	
    int fontId = QFontDatabase::addApplicationFont(":/Images/Emulogic-zrEw.ttf");
	QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
	QFont font(fontFamily, FONT_SIZE);
	
	readyText->setFont(font);
	readyText->setDefaultTextColor(Qt::yellow);
	QString readyString = QString("READY!");
	QFontMetrics metrics(readyText->font());
	int readyTextX = MAP_WIDTH / 2 - metrics.horizontalAdvance(readyString) / 2;
	readyText->setPlainText(readyString);
	readyText->setPos(readyTextX, MAP_HEIGHT / 2 + GRID_SIZE);
	readyText->setZValue(3);
}

void GameBoard::checkWin()
{
	if (dots.isEmpty() && energizers.isEmpty()) {
		eatenTimer->stop();
		modeTimer->stop();
		foreach(Ghost* ghost, ghosts) {
			removeItem(ghost);
			ghost->stop();
		}
		player->stop();
		exitingTimer->stop();
		winTimer->start(TIME_AFTER_WIN);
		emit win();
	}
}

void GameBoard::setMapAfterWin()
{
	winTimer->stop();
	dotCounter = 0;
	ghostNum = 0;
	fruitCounter = 0;
	level++;
	removeItem(player);
	placeDots();
	placeEnergizers();
	updateLevelText();
	setMapBasic();
}

void GameBoard::letGhostsOut()
{
	ghosts.at(ghostNum)->checkEatenDotsThresholdAlreadyExceeded(dotCounter);
	if (++ghostNum > 3)
		exitingTimer->stop();
}

void GameBoard::startGame()
{
	startTimer->stop();
	removeItem(readyText);
	inputEnabled = true;
	
	foreach(Ghost* ghost, ghosts) {
		ghost->setStartSpeed();
	}
	modeTimer->start(intervals.dequeue());
	
	player->setFlag(QGraphicsItem::ItemIsFocusable);
	player->setFocus();
}

void GameBoard::setMapBasic()
{
	inputEnabled = false;
	player->setDefault();
	addItem(player);

	blinky->setDefault();
	addItem(blinky);

	pinky->setDefault();
	addItem(pinky);

	inky->setDefault();
	addItem(inky);

	clyde->setDefault();
	addItem(clyde);

	ghostNum = 0;
	eatenTimer->start(PLAYER_MOVE_TIME);
	
	setIntervals();
	addItem(readyText);
	startTimer->start(START_TIME);
}

void GameBoard::setNewTry()
{
	if (liveCounter >= 0) {
		removeItem(player);
		setMapBasic();
		foreach(Ghost* ghost, ghosts) {
			ghost->stop();
		}
		exitingTimer->start(EXITING_TIME);
	}
	else {
		qDebug() << "GAME OVER" << "score:" << score;
		gameOver();
	}	
}


//used only for test purpose
QPixmap GameBoard::createCirclePixmap(const QColor &color, int diameter)
{
	QPixmap pixmap(diameter, diameter);
	pixmap.fill(Qt::transparent);
	
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(color);
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(0, 0, diameter, diameter);
	return pixmap;
}


void GameBoard::testTargetGirds()
{
	testBlinky = new QGraphicsPixmapItem(createCirclePixmap(Qt::red, Px / 2));
	testBlinky->setZValue(4);
	addItem(testBlinky);
	testPinky = new QGraphicsPixmapItem(createCirclePixmap(QColor(255, 170, 250), Px / 2));
	testPinky->setZValue(4);
	addItem(testPinky);
	testInky = new QGraphicsPixmapItem(createCirclePixmap(QColor(0, 255, 240), Px / 2));
	testInky->setZValue(4);
	addItem(testInky);
	testClyde = new QGraphicsPixmapItem(createCirclePixmap(QColor(255, 165, 0), Px / 2));
	testClyde->setZValue(4);
	addItem(testClyde);
	
	QTimer* testTimer = new QTimer(this);
	connect(testTimer, SIGNAL(timeout()), this, SLOT(updateTargets()));
	testTimer->start(PLAYER_MOVE_TIME);
}

void GameBoard::updateTargets()
{
	testBlinky->setPos(blinky->returnTargetGrid().x() * GRID_SIZE, blinky->returnTargetGrid().y() * GRID_SIZE);
	testPinky->setPos(pinky->returnTargetGrid().x() * GRID_SIZE, pinky->returnTargetGrid().y() * GRID_SIZE);
	testInky->setPos(inky->returnTargetGrid().x() * GRID_SIZE, inky->returnTargetGrid().y() * GRID_SIZE);
	testClyde->setPos(clyde->returnTargetGrid().x() * GRID_SIZE, clyde->returnTargetGrid().y() * GRID_SIZE);
}
