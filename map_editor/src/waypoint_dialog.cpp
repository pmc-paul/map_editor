#include "waypoint_dialog.h"

#include <QObject>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>

WaypointDialog::WaypointDialog(QWidget *parent)
    : QDialog(parent)
{
    // create form's widgets
    typeEdit = new QComboBox(this);
    QStringList types = { "Start", "Aisle", "Shelf" };
    typeEdit->addItems(types);

    aisleEdit = new QSpinBox();
    aisleEdit->setMinimum(1);
    aisleEdit->setMaximum(10);

    shelfEdit = new QComboBox();
    QStringList shelves = { "A", "B", "C", "D", "E", "F", "G" };
    shelfEdit->addItems(shelves);

    // create form
    QGroupBox *params = new QGroupBox(tr("Parameters:"));

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("&Type:"), typeEdit);
    formLayout->addRow(tr("&Aisle:"), aisleEdit);
    formLayout->addRow(tr("&Shelf:"), shelfEdit);
    
    params->setLayout(formLayout);

    // create apply and cancel button
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // add widgets to layout
    QVBoxLayout * vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->addWidget(params);
    vBoxLayout->addWidget(buttonBox);

    setLayout(vBoxLayout);
    setWindowTitle("Waypoint Editor");
}

void WaypointDialog::setParametersFromWaypoint(Waypoint *waypoint)
{
    this->typeEdit->setCurrentText(waypoint->getTypeInQString());
    this->aisleEdit->setValue(waypoint->getAisle());
    this->shelfEdit->setCurrentText(waypoint->getShelf());
}

QString WaypointDialog::getType()
{
    return typeEdit->currentText();
}

int WaypointDialog::getAisle()
{
    return aisleEdit->value();
}

QString WaypointDialog::getShelf()
{
    return shelfEdit->currentText();
}