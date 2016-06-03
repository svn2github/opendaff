#include "QDAFFViewerWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString sPath = "D://Users//stienen//dev//ExternalLibs//OpenDAFF//opendaff-1.7_withcred//matlab//ExampleFigureOfEightFront.ms.daff";
    if( argc == 2 )
        sPath = argv[1];

    QDAFFViewerWindow w( NULL, sPath );
    w.show();

    return a.exec();
}
