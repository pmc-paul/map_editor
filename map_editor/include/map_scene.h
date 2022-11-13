#ifndef MAP_SCENE_H
#define MAP_SCENE_H

#include "waypoint.h"
#include "restricted_zone.h"

#include "map_file_parser.h"

#include "waypoint_dialog.h"

#include <QGraphicsScene>
#include <QDebug>

class Waypoint;
class WaypointDialog;

class MapConfig;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QGraphicsPixmapItem;
QT_END_NAMESPACE

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertWaypoint, InsertLine, SelectWaypoint, InsertRestrictedZone, MoveItem };

    explicit MapScene(QMenu *itemMenu, QObject *parent = nullptr);

    void displayMap(QString imageFileName);
    
    MapConfig *mapConfig;
    std::vector<Waypoint *> waypoints;

    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor itemColor() const { return myItemColor; }
    QColor lineColor() const { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);

public slots:
    void setMode(Mode mode);

signals:
    void waypointInserted(Waypoint *item);
    void restrictedZoneInserted(RestrictedZone *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    bool isItemChange(int type) const;

    QGraphicsPixmapItem *map;
    WaypointDialog *waypointDialog;

    Waypoint::WaypointType myWaypointType;
    RestrictedZone::RestrictedZoneType myRestrictedZoneType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;

    QFont myFont;

    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
};

#endif // MAP_SCENE_H
