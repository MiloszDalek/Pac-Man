#pragma once

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QObject>

class IntroScreen : public QGraphicsScene {
    Q_OBJECT
public:
	IntroScreen(QObject* parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent* event) override;
signals:
    void startGame();
};
