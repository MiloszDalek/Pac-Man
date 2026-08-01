#include "introscreen.h"
#include "entity.h"

IntroScreen::IntroScreen(QObject* parent) : QGraphicsScene(parent) {
    setSceneRect(0, 0, MAP_WIDTH, MAP_HEIGHT + ADDED_HEIGHT);

    title = new QGraphicsPixmapItem(QPixmap(":/Images/pac-man-title.png").scaled(540, 288));
    QRectF titleRect = title->boundingRect();
    title->setPos((MAP_WIDTH - titleRect.width()) / 2, MAP_HEIGHT / 2 - titleRect.height() / 2);

    addItem(title);

    credits = new QGraphicsTextItem("Created by Miłosz Dałek | Inspired by Pac-Man");

    QFont font("Arial", 10);
    credits->setFont(font);
    credits->setDefaultTextColor(Qt::white);

    QRectF creditsRect = credits->boundingRect();
    credits->setPos((MAP_WIDTH - creditsRect.width()) / 2, MAP_HEIGHT + ADDED_HEIGHT - creditsRect.height() - 10);

    addItem(credits);
}

void IntroScreen::keyPressEvent(QKeyEvent* event) {
    emit startGame();
}
