#ifndef MAP_SCENE_H
#define MAP_SCENE_H

#include "waypoint.h"
#include "link.h"
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
    enum Mode { InsertWaypoint, InsertLink, InsertRestrictedZone, MoveItem };

    explicit MapScene(QMenu *itemMenu, QObject *parent = nullptr);

    void displayMap(QString imageFileName);
    void loadWaypoints();
    void loadWaypoint(Waypoint *waypoint);
    void loadLinks(std::vector<std::pair<int, int>> links);
    void loadLink(int startIndex, int endIndex);
    
    MapConfig *mapConfig;
    std::vector<Waypoint *> waypoints;

    QFont font() const { return myFont; }
    QColor lineColor() const { return myLineColor; }
    void setFont(const QFont &font);

    void setRotation(int rotation);

public slots:
    void setMode(Mode mode);
    void deleteItem();

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
    int mapRotation = 0;

    WaypointDialog *waypointDialog;

    RestrictedZone::RestrictedZoneType myRestrictedZoneType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;

    QFont myFont;
    QColor myLineColor;
};

#endif // MAP_SCENE_H
