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

    void setRotation(int rotation);

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

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    // parameters
    WaypointType type;
    int aisle = 0;
    QString shelf = "";

    // text centered in ellipse
    QString text;

    QMenu *myContextMenu;
    QPen *myPen;
    // QVector<Arrow *> arrows;


    // x, y position in map
    float mapX;
    float mapY;

    int mapRotation;

    const int size = 20;
};


#endif // WAYPOINT_H
