#include "map_scene.h"
#include "waypoint.h"
#include "restricted_zone.h"
#include "main_window.h"

#include <QtWidgets>

const int InsertTextButton = 10;

MainWindow::MainWindow()
{
    createActions();
    createToolBox();    // barre de gauche 
    createMenus();      // barre du haut avec les options

    scene = new MapScene(viewMenu, this);
    connect(this, &MainWindow::deleteItem, 
            scene, &MapScene::deleteItem);

    // scene->setSceneRect(QRectF(0, 0, 1000, 1000));
    // connect(scene, &MapScene::itemInserted,
    //         this, &MainWindow::itemInserted);
    // connect(scene, &MapScene::textInserted,
    //         this, &MainWindow::textInserted);
    // connect(scene, &MapScene::itemSelected,
    //         this, &MainWindow::itemSelected);
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    view->setDragMode(QGraphicsView::ScrollHandDrag);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("Map Editor"));
}

void MainWindow::openFile()
{
    qDebug() << "Open File button was pressed";

    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Map"), "/home/willi/map_editor_ws/maps/sim", tr("Map Files (*.yaml)"));

    if(fileName != "")
    {
        mapFileParser = new MapFileParser(fileName.toStdString());
        mapEditorConfig = mapFileParser->getMapEditorConfig();     

        scene->mapConfig = mapFileParser->getMapConfig();
        scene->displayMap(QString::fromStdString(scene->mapConfig->filePath));

        // set map editor settings from config 
        sceneScaleSpinBox->setValue(mapEditorConfig->scale); // weird glitch when scale and rotation equal to 100
        sceneRotationSpinBox->setValue(mapEditorConfig->rotation);

        view->verticalScrollBar()->setSliderPosition(mapEditorConfig->verticalScrollBarValue);
        view->horizontalScrollBar()->setSliderPosition(mapEditorConfig->horizontalScrollBarValue);
            
            // get topological map 
                // get waypoints
                // render waypoints

                // get restricted zones
                // render restricted zones
    }
    

    return;
}

void MainWindow::saveFile()
{
    qDebug() << "Save File button was pressed";

    // save map editor config
    mapEditorConfig->verticalScrollBarValue = view->verticalScrollBar()->value();
    mapEditorConfig->horizontalScrollBarValue = view->horizontalScrollBar()->value();

    mapFileParser->saveMapEditorParams();
    mapFileParser->saveWaypoints(scene->waypoints);

    // save map with waypoints and everything
    return;
}

void MainWindow::saveFileAs()
{
    qDebug() << "Save File As button was pressed";
    return;
}

void MainWindow::addWaypoint()
{
    scene->setMode(MapScene::InsertWaypoint);
    return;
}

void MainWindow::addLink()
{
    scene->setMode(MapScene::InsertLink);
    return;
}

void MainWindow::addRestrictedZone()
{
    qDebug() << "Add Restricted Zone button was pressed";
    return;
}

void MainWindow::viewWaypoints()
{
    qDebug() << "View Waypoints button was pressed";
    return;
}

void MainWindow::viewRestrictedZones()
{
    qDebug() << "View Restricted Zones button was pressed";
    return;
}

void MainWindow::sceneScaleChanged(const int &scale)
{
    mapEditorConfig->scale = scale;
    updateSceneTransform();

    return;
}

void MainWindow::sceneRotationChanged(const int &rotation)
{
    mapEditorConfig->rotation = rotation;
    updateSceneTransform();

    return;
}

void MainWindow::updateSceneTransform()
{
    double newScale = mapEditorConfig->scale / 100.0;

    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
    view->rotate(mapEditorConfig->rotation);

    scene->setRotation(mapEditorConfig->rotation);

    return;
}

// void MainWindow::itemSelected(QGraphicsItem *item)
// {
//     DiagramTextItem *textItem =
//     qgraphicsitem_cast<DiagramTextItem *>(item);
// }

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Map Editor"),
                       tr("The <b>Map Editor</b> allows you to edit maps "
                          "by adding waypoints and restricted zones."));
}

void MainWindow::createToolBox()
{
    // Waypoints Editor Widget
    QLabel *waypointsWidget = new QLabel("This widget will contain the tools\n required to add and edit waypoints.");

    // Restricted Zones Widget
    QLabel *restrictedZonesWidget = new QLabel("This widget will contain the tools\n required to add and edit restricted zones.");

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(waypointsWidget->sizeHint().width());
    toolBox->addItem(waypointsWidget, tr("Waypoints Editor Widget"));
    toolBox->addItem(restrictedZonesWidget, tr("Restricted Zones Editor Widget"));
}

void MainWindow::createActions()
{
    // File Actions
    openFileAction = new QAction(QIcon(":/images/open.png"), tr("Open &File"), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("Open file"));
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    saveFileAction = new QAction(QIcon(":/images/save.png"), tr("Save &File"), this);
    saveFileAction->setShortcut(tr("Ctrl+S"));
    saveFileAction->setStatusTip(tr("Save file"));
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);

    saveFileAsAction = new QAction(QIcon(":/images/save_as.png"), tr("Save &File As"), this);
    saveFileAsAction->setStatusTip(tr("Save file as..."));
    connect(saveFileAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Map Editor"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Edit Actions
    addWaypointAction = new QAction(QIcon(":/images/add-waypoint.png"),
                                tr("A&dd a New Waypoint"), this);
    addWaypointAction->setStatusTip(tr("Add a new waypoint"));
    connect(addWaypointAction, &QAction::triggered, this, &MainWindow::addWaypoint);

    addLinkAction = new QAction(QIcon(":/images/linepointer.png"),
                                tr("A&dd a New Link"), this);
    addLinkAction->setStatusTip(tr("Add a new link"));
    connect(addLinkAction, &QAction::triggered, this, &MainWindow::addLink);

    addRestrictedZoneAction = new QAction(QIcon(":/images/background2.png"),
                                tr("Add a New R&estricted Zone"), this);
    addRestrictedZoneAction->setStatusTip(tr("Add a new restricted zone"));
    connect(addRestrictedZoneAction, &QAction::triggered, this, &MainWindow::addRestrictedZone);

    deleteItemAction = new QAction(QIcon(":/images/delete.png"),
                                tr("Delete Item"), this);
    deleteItemAction->setStatusTip(tr("Delete selected item"));
    connect(deleteItemAction, &QAction::triggered, this, &MainWindow::deleteItem);


    // View Actions
    viewWaypointsAction = new QAction(QIcon(":/images/background3.png"),
                                tr("W&aypoints"), this);
    viewWaypointsAction->setStatusTip(tr("View waypoints"));
    viewWaypointsAction->setCheckable(true);
    viewWaypointsAction->setChecked(true);
    connect(viewWaypointsAction, &QAction::triggered, this, &MainWindow::viewWaypoints);

    viewRestrictedZonesAction = new QAction(QIcon(":/images/background4.png"),
                                tr("R&estricted Zones"), this);
    viewRestrictedZonesAction->setStatusTip(tr("View restricted zones"));
    viewRestrictedZonesAction->setCheckable(true);
    viewRestrictedZonesAction->setChecked(true);
    connect(viewRestrictedZonesAction, &QAction::triggered, this, &MainWindow::viewRestrictedZones);

    // Help Actions
    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveFileAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addWaypointAction);
    editMenu->addAction(addLinkAction);
    editMenu->addAction(addRestrictedZoneAction);
    editMenu->addAction(deleteItemAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(viewWaypointsAction);
    viewMenu->addAction(viewRestrictedZonesAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}

void MainWindow::createToolbars()
{
    // File Toolbar
    openToolbar = addToolBar(tr("Open"));
    openToolbar->addAction(openFileAction);
    openToolbar->addAction(saveFileAction);
    openToolbar->addAction(saveFileAsAction);

    // Editor Toolbar
    editToolbar = addToolBar(tr("Edit"));
    editToolbar->addAction(addWaypointAction);
    editToolbar->addAction(addLinkAction);
    editToolbar->addAction(addRestrictedZoneAction);
    editToolbar->addAction(deleteItemAction);

    // View Toolbar
    sceneScaleSpinBox = new QSpinBox;
    sceneScaleSpinBox->setRange(50, 400);
    sceneScaleSpinBox->setSingleStep(10);
    sceneScaleSpinBox->setSuffix("%");
    sceneScaleSpinBox->setValue(100);

    connect(sceneScaleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(sceneScaleChanged(int)));


    sceneRotationSpinBox = new QSpinBox;
    sceneRotationSpinBox->setRange(0, 360);
    sceneRotationSpinBox->setSingleStep(10);
    sceneRotationSpinBox->setSuffix(" deg");
    sceneRotationSpinBox->setValue(0);

    connect(sceneRotationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(sceneRotationChanged(int)));

    QHBoxLayout *scaleLayout = new QHBoxLayout;
    scaleLayout->addWidget(new QLabel("Scale:"));
    scaleLayout->addWidget(sceneScaleSpinBox);
    scaleLayout->addWidget(new QLabel("Rotation:"));
    scaleLayout->addWidget(sceneRotationSpinBox);

    QWidget *scaleWidget = new QWidget();
    scaleWidget->setLayout(scaleLayout);

    viewToolbar = addToolBar(tr("View type"));
    viewToolbar->addAction(viewWaypointsAction);
    viewToolbar->addAction(viewRestrictedZonesAction);
    viewToolbar->addWidget(scaleWidget);
}
