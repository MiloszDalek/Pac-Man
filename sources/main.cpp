#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QBrush>

#include "gameboard.h"
#include "introscreen.h"
#include "gameView.h"


#ifdef __EMSCRIPTEN__

#include <emscripten.h>

IntroScreen* webIntroScreen = nullptr;

extern "C" {
EMSCRIPTEN_KEEPALIVE
    void startGameFromWeb()
    {
        if (webIntroScreen)
            webIntroScreen->startGameFromWeb();
    }

}

#endif


int main(int argc, char *argv[])
{
	QApplication game(argc, argv);
	
	IntroScreen* introScreen = new IntroScreen();

    #ifdef __EMSCRIPTEN__
    webIntroScreen = introScreen;
    #endif

    GameView* gameView = new GameView(introScreen);

    introScreen->setSceneRect(0, 0, MAP_WIDTH, MAP_HEIGHT + ADDED_HEIGHT);

	QObject::connect(introScreen, &IntroScreen::startGame, [&]() {

        GameBoard* gameBoard = new GameBoard();

        gameBoard->setBackgroundBrush(QBrush(Qt::black));
        gameBoard->setSceneRect(0, -ADDED_HEIGHT * 0.6, MAP_WIDTH, MAP_HEIGHT + ADDED_HEIGHT);

        gameView->setScene(gameBoard);
        gameView->show();
    });

    gameView->show();
	
	return game.exec();
}
