#pragma once
#include <QGraphicsPixmapItem>
#include <QList>
#include <QTimer>

#include "entity.h"

class Icon : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT
  public:
	Icon(QGraphicsPixmapItem*, int);
	Icon(QGraphicsPixmapItem*, char);
	Icon(QGraphicsPixmapItem* parent = nullptr);
	void setFruit(char);
	int catched();
	
  public slots:
	void despawn();

  private:
	void fillBluePointsList();
	void fillPinkPointsList();
	void fillIconList();
	void selectFruit(int);
	
	char icon;
  
	QList<QPixmap> bluePointsList;
	QList<QPixmap> pinkPointsList;
	QTimer* despawnTimer;
	QList<QPixmap> iconList;
};
