#include "map_scene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>

MapScene::MapScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myWaypointType = Waypoint::Step;
    myRestrictedZoneType = RestrictedZone::Step;
    line = nullptr;
}

void MapScene::displayMap(QString imageFileName)
{
    map = addPixmap(QPixmap(imageFileName));
}

void MapScene::setMode(Mode mode)
{
    myMode = mode;
}

void MapScene::setItemType(Waypoint::WaypointType type)
{
    myWaypointType = type;
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    Waypoint *waypoint;
    RestrictedZone *restrictedZone;
    switch (myMode) {
        case InsertWaypoint:
            waypoint = new Waypoint(myWaypointType, myItemMenu);
            // item->setBrush(myItemColor);
            // addItem(item);
            // item->setPos(mouseEvent->scenePos());
            emit waypointInserted(waypoint);
            break;

        case InsertRestrictedZone:
            restrictedZone = new RestrictedZone(myRestrictedZoneType, myItemMenu);
            // item->setBrush(myItemColor);
            // addItem(item);
            // item->setPos(mouseEvent->scenePos());
            emit restrictedZoneInserted(restrictedZone);
            break;

    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // if (myMode == InsertLine && line != nullptr) {
    //     QLineF newLine(line->line().p1(), mouseEvent->scenePos());
    //     line->setLine(newLine);
    // } else if (myMode == MoveItem) {
    //     QGraphicsScene::mouseMoveEvent(mouseEvent);
    // }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // if (line != nullptr && myMode == InsertLine) {
    //     QList<QGraphicsItem *> startItems = items(line->line().p1());
    //     if (startItems.count() && startItems.first() == line)
    //         startItems.removeFirst();
    //     QList<QGraphicsItem *> endItems = items(line->line().p2());
    //     if (endItems.count() && endItems.first() == line)
    //         endItems.removeFirst();

    //     removeItem(line);
    //     delete line;

    //     // if (startItems.count() > 0 && endItems.count() > 0 &&
    //     //     startItems.first()->type() == DiagramItem::Type &&
    //     //     endItems.first()->type() == DiagramItem::Type &&
    //     //     startItems.first() != endItems.first()) {
    //     //     DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
    //     //     DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
    //     //     Arrow *arrow = new Arrow(startItem, endItem);
    //     //     arrow->setColor(myLineColor);
    //     //     startItem->addArrow(arrow);
    //     //     endItem->addArrow(arrow);
    //     //     arrow->setZValue(-1000.0);
    //     //     addItem(arrow);
    //     //     arrow->updatePosition();
    //     // }
    // }
    // line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool MapScene::isItemChange(int type) const
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}
