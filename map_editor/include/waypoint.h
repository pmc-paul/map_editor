#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QGraphicsEllipseItem>
#include <QVector>

QT_BEGIN_NAMESPACE
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE

// class Arrow;

class Waypoint : public QGraphicsEllipseItem
{
public:
    enum WaypointType { Start, Aisle, Shelf };

    Waypoint(QMenu *contextMenu, QGraphicsItem *parent = nullptr);

    void setType(WaypointType waypointType);
    void setType(QString waypointType);

    void setAisle(int aisle);
    void setShelf(QString shelf);

    void setPos(const QPointF &pos, float resolution, float origin[3]);

    // void removeArrow(Arrow *arrow);
    // void removeArrows();
    WaypointType getType() const { return type; }
    QString getTypeInQString();
    int getAisle() const { return aisle; }
    QString getShelf() const { return shelf; }


    // void addArrow(Arrow *arrow);

    float getMapX();
    float getMapY();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *hoverEvent) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *hoverEvent) override;

private:
    WaypointType type;
    int aisle = 1;
    QString shelf = "A";

    QMenu *myContextMenu;
    QPen *myPen;
    // QVector<Arrow *> arrows;


    // x, y position in map
    float mapX;
    float mapY;

    const int size = 20;
};


#endif // WAYPOINT_H
