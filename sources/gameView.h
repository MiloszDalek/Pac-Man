#pragma once

#include <QGraphicsView>
#include <QResizeEvent>

class GameView : public QGraphicsView
{
public:
    explicit GameView(QGraphicsScene* scene, QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;
};