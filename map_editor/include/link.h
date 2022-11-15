#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>

class Waypoint;

class Link : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Link(Waypoint *startItem, Waypoint *endItem, QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { myColor = color; }
    Waypoint *startItem() const { return myStartItem; }
    Waypoint *endItem() const { return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    Waypoint *myStartItem;
    Waypoint *myEndItem;
    QColor myColor = Qt::black;
};

#endif // LINK_H
