#pragma once
#include <QGraphicsPixmapItem>
#include <QTimer>

class Energizer : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT
  public:
	Energizer(QGraphicsPixmapItem* parent = nullptr);

  public slots:
	void animate();

	int centerX();
	int centerY();

  private:
	QTimer* animationTimer;
	int state;

	QPixmap white;
	QPixmap black;
};
