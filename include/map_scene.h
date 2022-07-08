#ifndef MAP_SCENE_H
#define MAP_SCENE_H

#include "waypoint.h"
#include "restricted_zone.h"

#include <QGraphicsScene>
#include <QDebug>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertWaypoint, InsertRestrictedZone, MoveItem };

    explicit MapScene(QMenu *itemMenu, QObject *parent = nullptr);

    void displayMap(QString imageFileName);

public slots:
    void setMode(Mode mode);
    void setItemType(Waypoint::WaypointType type);

signals:
    void waypointInserted(Waypoint *item);
    void restrictedZoneInserted(RestrictedZone *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    bool isItemChange(int type) const;

    QGraphicsPixmapItem *map;

    Waypoint::WaypointType myWaypointType;
    RestrictedZone::RestrictedZoneType myRestrictedZoneType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
};

#endif // MAP_SCENE_H
