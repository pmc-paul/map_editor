#include "waypoint.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

Waypoint::Waypoint(QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), myContextMenu(contextMenu)
{
    setType(WaypointType::Start);

    setRect(0, 0, size, size);
    setPen(*myPen);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptHoverEvents(true);
}

// void Waypoint::removeArrow(Arrow *arrow)
// {
//     arrows.removeAll(arrow);
// }

// void Waypoint::removeArrows()
// {
//     // need a copy here since removeArrow() will
//     // modify the arrows container
//     const auto arrowsCopy = arrows;
//     for (Arrow *arrow : arrowsCopy) {
//         arrow->startItem()->removeArrow(arrow);
//         arrow->endItem()->removeArrow(arrow);
//         scene()->removeItem(arrow);
//         delete arrow;
//     }
// }

// void Waypoint::addArrow(Arrow *arrow)
// {
//     arrows.append(arrow);
// }

void Waypoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant Waypoint::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // if (change == QGraphicsItem::ItemPositionChange) {
    //     for (Arrow *arrow : qAsConst(arrows))
    //         arrow->updatePosition();
    // }

    return value;
}

void Waypoint::setType(QString type)
{
    if(type == "Start")
        setType(WaypointType::Start);
    else if(type == "Aisle")
        setType(WaypointType::Aisle);
    else if(type == "Shelf")
        setType(WaypointType::Shelf);
    else
        qDebug() << "Selected type not recognized.";
}

void Waypoint::setType(WaypointType type)
{
    this->type = type;
    switch (this->type) {
        case Start:
            myPen = new QPen(Qt::green, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            break;
        case Aisle:
            myPen = new QPen(Qt::blue, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            break;
        case Shelf:
            myPen = new QPen(Qt::red, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            break;
        default:
            myPen = new QPen(Qt::gray, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            break;
    }
}

QString Waypoint::getTypeInQString()
{
    switch (this->type) {
        case Start:
            return "Start";
        case Aisle:
            return "Aisle";
        case Shelf:
            return "Shelf";
    }
}

void Waypoint::setAisle(int aisle)
{
    this->aisle = aisle;
}

void Waypoint::setShelf(QString shelf)
{
    this->shelf = shelf;
}

void Waypoint::setPos(const QPointF &pos, float resolution, float origin[3])
{
    QGraphicsEllipseItem::setPos(pos.x() - 0.5*size, pos.y() - 0.5*size);

    mapX = pos.x() * resolution + origin[0];
    mapY = - (pos.y() * resolution + origin[1]);
}

void Waypoint::hoverEnterEvent(QGraphicsSceneHoverEvent *hoverEvent)
{
    QPen *hoverPen = new QPen(*myPen);
    hoverPen->setWidth(5);
    setPen(*hoverPen);

    QGraphicsItem::hoverEnterEvent(hoverEvent);
}

void Waypoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *hoverEvent)
{
    setPen(*myPen);
    QGraphicsItem::hoverLeaveEvent(hoverEvent);
}

float Waypoint::getMapX() { return mapX; }
float Waypoint::getMapY() { return mapY; }