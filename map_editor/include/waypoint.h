#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QVector>

QT_BEGIN_NAMESPACE
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
class QGraphicsTextItem;
QT_END_NAMESPACE

class Link;

class Waypoint : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 15 };
    enum WaypointType { Start, Aisle, Shelf };

    Waypoint(QMenu *contextMenu, QGraphicsItem *parent = nullptr);

    void setWaypointType(WaypointType waypointType);
    void setWaypointType(QString waypointType);

    void setAisle(int aisle);
    void setShelf(QString shelf);

    void setPos(const QPointF &pos, float resolution, float origin[3]);

    void setRotation(int rotation);

    
    WaypointType getWaypointType() const { return waypointType; }
    QString getTypeInQString();
    int getAisle() const { return aisle; }
    QString getShelf() const { return shelf; }
    int getSize() const { return size; }

    int type() const override { return Type; }
    

    void addLink(Link *link);
    void removeLink(Link *link);
    void removeLinks();


    float getMapX();
    float getMapY();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *hoverEvent) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *hoverEvent) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    // parameters
    WaypointType waypointType;
    int aisle = 0;
    QString shelf = "";

    // text centered in ellipse
    QString text;

    QMenu *myContextMenu;
    QPen *myPen;
    QVector<Link *> links;


    // x, y position in map
    float mapX;
    float mapY;

    int mapRotation;

    const int size = 20;
};


#endif // WAYPOINT_H
