#include "map_scene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QPointF>

MapScene::MapScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myRestrictedZoneType = RestrictedZone::Step;

    myLineColor = Qt::black;
    line = nullptr;

    waypointDialog = new WaypointDialog(qobject_cast<QWidget *>(parent));
}

void MapScene::displayMap(QString imageFileName)
{
    map = addPixmap(QPixmap(imageFileName));
    map->setZValue(-1);
}

void MapScene::setRotation(int rotation)
{
    mapRotation = rotation;

    for(Waypoint *waypoint : waypoints)
        waypoint->setRotation(mapRotation);
}

void MapScene::setMode(Mode mode)
{
    myMode = mode;
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    Waypoint *waypoint;
    RestrictedZone *restrictedZone;
    switch (myMode) {
        case InsertWaypoint:
        {
            // create waypoint and add to scene
            waypoint = new Waypoint(myItemMenu);
            addItem(waypoint);

            // set waypoint position to the mouse position
            waypoint->setPos(mouseEvent->scenePos(), mapConfig->resolution, mapConfig->origin);

            // add waypoint to vector to modify it later
            waypoints.push_back(waypoint);

            qDebug() << "waypoints vector size: " << waypoints.size();

            // dialog to configure new waypoint
            waypointDialog->exec();

            if(waypointDialog->result() == QDialog::Accepted)
            {
                waypoint->setType(waypointDialog->getType());
                waypoint->setAisle(waypointDialog->getAisle());
                waypoint->setShelf(waypointDialog->getShelf());
            }

            emit waypointInserted(waypoint);

            myMode = MoveItem;
            break;
        }

        case InsertLink:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                                mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 1));
            addItem(line);
            break;

        case InsertRestrictedZone:
        {
            qDebug() << "inserting restricted zone...";
            restrictedZone = new RestrictedZone(myRestrictedZoneType, myItemMenu);
            // item->setBrush(myItemColor);
            // addItem(item);
            // item->setPos(mouseEvent->scenePos());
            emit restrictedZoneInserted(restrictedZone);
            break;
        }

        case SelectWaypoint:
        {
            qDebug() << "item selected...";
            waypoint = new Waypoint(myItemMenu);
            // item->setBrush(myItemColor);
            // addItem(item);
            // item->setPos(mouseEvent->scenePos());
            emit itemSelected(waypoint);;
            break;
        }

        default:
            ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLink && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // if (line != nullptr && myMode == InsertLink) {
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

    myMode = MoveItem;

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MapScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    for(Waypoint *waypoint : waypoints)
    {
        if(waypoint->isSelected())
        {
            // load params in dialog
            waypointDialog->setParametersFromWaypoint(waypoint);    // might be more user friendly to just keep previous parameters
            waypointDialog->exec();

            // update waypoint
            if(waypointDialog->result() == QDialog::Accepted)
            {
                waypoint->setType(waypointDialog->getType());
                waypoint->setAisle(waypointDialog->getAisle());
                waypoint->setShelf(waypointDialog->getShelf());
            }

            break;
        }
    }

    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

bool MapScene::isItemChange(int type) const
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}

void MapScene::deleteItem()
{
    for(int i = 0; i < waypoints.size(); i++)
    {
        if(waypoints[i]->isSelected())
        {
            Waypoint *waypoint = waypoints[i];

            // remove from scene
            this->removeItem((QGraphicsItem *) waypoint);

            // remove from vector
            waypoints.erase(waypoints.begin() + i);

            // delete
            delete waypoint;

            break;
        }
    }
}

