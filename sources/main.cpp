#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QBrush>

#include "gameboard.h"
#include "introscreen.h"

int main(int argc, char *argv[])
{
	QApplication game(argc, argv);
	
	IntroScreen* introScreen = new IntroScreen();
    QGraphicsView* introView = new QGraphicsView(introScreen);
    introView->setFixedSize(MAP_WIDTH + 2, MAP_HEIGHT + ADDED_HEIGHT);
    introView->setBackgroundBrush(QBrush(Qt::black));

    introView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    introView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    introView->setFrameShape(QFrame::NoFrame);

    introView->show();

	QObject::connect(introScreen, &IntroScreen::startGame, [&]() {
        GameBoard* gameBoard = new GameBoard();
		gameBoard->setBackgroundBrush(QBrush(Qt::black));
		gameBoard->setSceneRect(0, -(GRID_SIZE / 2), MAP_WIDTH, MAP_HEIGHT);

		QGraphicsView* gameView = new QGraphicsView(gameBoard);
		gameView->setFixedSize(MAP_WIDTH + 2, MAP_HEIGHT + ADDED_HEIGHT);

        gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gameView->setFrameShape(QFrame::NoFrame);
        
        gameView->show();
        introView->close();
    });
	
	return game.exec();
}
