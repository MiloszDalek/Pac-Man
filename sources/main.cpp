#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QBrush>

#include "gameboard.h"
#include "introscreen.h"
#include "gameView.h"

int main(int argc, char *argv[])
{
	QApplication game(argc, argv);
	
	IntroScreen* introScreen = new IntroScreen();
    GameView* introView = new GameView(introScreen);

    introScreen->setSceneRect(0, 0, MAP_WIDTH, MAP_HEIGHT + ADDED_HEIGHT);

    introView->show();

	QObject::connect(introScreen, &IntroScreen::startGame, [&]() {

        GameBoard* gameBoard = new GameBoard();

        gameBoard->setBackgroundBrush(QBrush(Qt::black));
        gameBoard->setSceneRect(0, -ADDED_HEIGHT * 0.6, MAP_WIDTH, MAP_HEIGHT + ADDED_HEIGHT);

        GameView* gameView = new GameView(gameBoard);
        
        gameView->show();
        introView->close();
    });
	
	return game.exec();
}
