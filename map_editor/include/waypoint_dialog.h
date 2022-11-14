#ifndef WAYPOINT_DIALOG_H
#define WAYPOINT_DIALOG_H

#include "waypoint.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDebug>

class Waypoint;

QT_BEGIN_NAMESPACE
class QDialog;
class QDialogButtonBox;
class QComboBox;
class QSpinBox;
QT_END_NAMESPACE

class WaypointDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WaypointDialog(QWidget *parent = nullptr);

    QString getType();
    int getAisle();
    QString getShelf();

public slots:
    void setParametersFromWaypoint(Waypoint *waypoint);

private:
    QComboBox *typeEdit;
    QSpinBox *aisleEdit;
    QComboBox *shelfEdit;

    QDialogButtonBox *buttonBox;  
};

#endif // WAYPOINT_DIALOG_H
