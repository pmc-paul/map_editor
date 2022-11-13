#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "map_file_parser.h"

class MapScene;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   MainWindow();

private slots:
    void openFile();
    void saveFile();
    void saveFileAs();

    void addWaypoint();
    void addRestrictedZone();
    void deleteItem();

    void viewWaypoints();
    void viewRestrictedZones();

    // void itemInserted(DiagramItem *item);
    // void textInserted(QGraphicsTextItem *item);
    // void itemSelected(QGraphicsItem *item);

    void sceneScaleChanged(const int &scale);
    void sceneRotationChanged(const int &rotation);

    void updateSceneTransform();

    void about();

private:
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();

    MapFileParser *mapFileParser;
    MapEditorConfig *mapEditorConfig;

    MapScene *scene;
    QGraphicsView *view;

    // Actions
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *saveFileAsAction;
    QAction *exitAction;

    QAction *addWaypointAction;
    QAction *addRestrictedZoneAction;
    QAction *deleteItemAction;

    QAction *viewWaypointsAction;
    QAction *viewRestrictedZonesAction;

    QAction *aboutAction;

    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *aboutMenu;

    // Toolbar
    QToolBar *openToolbar;
    QToolBar *editToolbar;
    QToolBar *viewToolbar;

    QSpinBox *sceneScaleSpinBox;
    QSpinBox *sceneRotationSpinBox;

    // Toolbox
    QToolBox *toolBox;
};

#endif // MAIN_WINDOW_H
