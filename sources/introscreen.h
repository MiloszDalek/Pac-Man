#pragma once

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QObject>

class IntroScreen : public QGraphicsScene {
    Q_OBJECT
  public:
	  IntroScreen(QObject* parent = nullptr);
      void startGameFromWeb();
  protected:
    void keyPressEvent(QKeyEvent* event) override;
  private:
    QGraphicsPixmapItem* title;
    QGraphicsTextItem* credits;
  signals:
    void startGame();
};
