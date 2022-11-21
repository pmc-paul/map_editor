#include "waypoint.h"
#include "link.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

Waypoint::Waypoint()
{
    setRect(0, 0, size, size);
    setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    setBrush(Qt::white);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptHoverEvents(true);
}

Waypoint::Waypoint(float resolution, float origin[3], QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), myContextMenu(contextMenu)
{
    this->resolution = resolution;
    this->origin = origin;

    setWaypointType(WaypointType::Start);

    setRect(0, 0, size, size);
    setPen(*myPen);
    setBrush(Qt::white);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptHoverEvents(true);
}

void Waypoint::removeLink(Link *link)
{
    links.removeAll(link);
}

void Waypoint::removeLinks()
{
    // need a copy here since removeLink() will
    // modify the links container
    const auto linksCopy = links;
    for (Link *link : linksCopy) {
        link->startItem()->removeLink(link);
        link->endItem()->removeLink(link);
        scene()->removeItem(link);
        delete link;
    }
}

void Waypoint::addLink(Link *link)
{
    links.append(link);
}

void Waypoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant Waypoint::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == Waypoint::ItemPositionChange) {
        for (Link *link : qAsConst(links))
            link->updatePosition();

        if(scenePos().x() != 0 && scenePos().y())
            calculateMapPosition(scenePos(), false);
    }

    return value;
}

void Waypoint::setWaypointType(QString waypointType)
{
    if(waypointType == "Start")
        setWaypointType(WaypointType::Start);
    else if(waypointType == "Aisle")
        setWaypointType(WaypointType::Aisle);
    else if(waypointType == "Shelf")
        setWaypointType(WaypointType::Shelf);
    else
        qDebug() << "Selected type not recognized.";
}

void Waypoint::setWaypointType(WaypointType waypointType)
{
    this->waypointType = waypointType;
    switch (this->waypointType) {
        case Start:
            myPen = new QPen(Qt::green, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            text = "S";
            break;
        case Aisle:
            myPen = new QPen(Qt::blue, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            break;
        case Shelf:
            myPen = new QPen(Qt::red, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            break;
        default:
            myPen = new QPen(Qt::gray, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            text = "";
            break;
    }

    setPen(*myPen);
}

QString Waypoint::getTypeInQString()
{
    switch (this->waypointType) {
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
    if(waypointType == WaypointType::Aisle)
    {
        this->aisle = aisle;
        text = QString::number(aisle);
    }
}

void Waypoint::setShelf(QString shelf)
{
    if(waypointType == WaypointType::Shelf)
    {
        this->shelf = shelf;
        text = shelf;
    }
}

void Waypoint::setPos(const QPointF &pos)
{
    QGraphicsEllipseItem::setPos(pos.x() - 0.5*size, pos.y() - 0.5*size);

    calculateMapPosition(pos, true);
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

void Waypoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // order is important to print text above ellipse (since the ellipse is filled with white)
    QGraphicsEllipseItem::paint(painter, option, widget);

    painter->setPen(Qt::black);
    painter->drawText(this->rect(), Qt::AlignCenter, text);

    
}

void Waypoint::calculateMapPosition(const QPointF &pos, bool exact_pose)
{
    QPointF pos_copy(pos);
    if(!exact_pose)
    {
        QPointF offset(0.5*size, 0.5*size);
        pos_copy += offset;
    }

    mapX = pos_copy.x() * resolution + origin[0];
    mapY = - (pos_copy.y() * resolution + origin[1]);
}