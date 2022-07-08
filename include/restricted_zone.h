#ifndef RESTRICTED_ZONE_H
#define RESTRICTED_ZONE_H

#include <QGraphicsPixmapItem>
#include <QVector>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE

// class Arrow;

class RestrictedZone : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    enum RestrictedZoneType { Step, Conditional, StartEnd, Io };

    RestrictedZone(RestrictedZoneType diagramType, QMenu *contextMenu, QGraphicsItem *parent = nullptr);

    // void removeArrow(Arrow *arrow);
    // void removeArrows();
    RestrictedZoneType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    // void addArrow(Arrow *arrow);
    QPixmap image() const;
    int type() const override { return Type; }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    RestrictedZoneType myDiagramType;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    // QVector<Arrow *> arrows;
};


#endif // RESTRICTED_ZONE_H
