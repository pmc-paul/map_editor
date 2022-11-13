#include "main_window.h"

#include <QApplication>

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(diagram_scene);

    QApplication app(argv, args);
    MainWindow mainWindow;
    mainWindow.showMaximized();

    return app.exec();
}
