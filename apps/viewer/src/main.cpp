#include "mainwindowdaffviewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString sPath = "C:/dev/ExternalLibs/OpenDAFF/opendaff-v1.7/opendaff_withcred/matlab/ExampleUnityUpperHemisphere.ms.daff";
    if( argc == 2 )
        sPath = argv[1];

    MainWindowDAFFViewer w( NULL, sPath );
    w.show();

    return a.exec();
}
