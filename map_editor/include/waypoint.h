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

    Waypoint();
    Waypoint(float resolution, float origin[3], QMenu *contextMenu, QGraphicsItem *parent = nullptr);

    void setWaypointType(WaypointType waypointType);
    void setWaypointType(QString waypointType);
    void setWaypointType(std::string waypointType) { setWaypointType(QString().fromStdString(waypointType)); }

    void setAisle(int aisle);
    void setShelf(QString shelf);
    void setShelf(std::string shelf) { setShelf(QString().fromStdString(shelf)); }

    void setPos(const QPointF &pos);

    void setRotation(int rotation) { mapRotation = rotation; }
    void setResolution(float resolution) { this->resolution = resolution; }
    void setOrigin(float origin[3]) { this->origin = origin; }

    void setContextMenu(QMenu *contextMenu) { myContextMenu = contextMenu; }
    
    WaypointType getWaypointType() const { return waypointType; }
    QString getTypeInQString();
    int getAisle() const { return aisle; }
    QString getShelf() const { return shelf; }
    int getSize() const { return size; }

    QVector<Link *> getLinks() const { return links; }
    std::vector<Link *> getLinksAsStdVector() const { return links.toStdVector(); }

    int type() const override { return Type; }
    

    void addLink(Link *link);
    void removeLink(Link *link);
    void removeLinks();


    void setMapX(float mapX) { this->mapX = mapX; }
    void setMapY(float mapY) { this->mapY = mapY; }

    float getMapX() { return mapX; }
    float getMapY() { return mapY; }

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

    // map parameters
    int mapRotation;
    float resolution;
    float *origin;

    float mapX;
    float mapY;
    
    void calculateMapPosition(const QPointF &pos, bool exact_pose);

    // text centered in ellipse
    QString text;

    QMenu *myContextMenu;
    QPen *myPen;
    QVector<Link *> links;

    const int size = 20;   
};


#endif // WAYPOINT_H
