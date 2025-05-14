#include "introscreen.h"

IntroScreen::IntroScreen(QObject* parent) : QGraphicsScene(parent) {
    QGraphicsPixmapItem* background = new QGraphicsPixmapItem(QPixmap("Images/pac-man-title.png").scaled(540, 288));
    addItem(background);
}

void IntroScreen::keyPressEvent(QKeyEvent* event) {
    emit startGame();
}
