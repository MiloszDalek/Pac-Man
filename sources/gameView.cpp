#include "gameView.h"

GameView::GameView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setBackgroundBrush(QBrush(Qt::black));
}

void GameView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    if (scene())
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}