#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtGui/QApplication>



#include "platforms/platform_base.h"
#include "yodaframework/ycontainermanager.h"

#include "mainmodule/boxhandle.h"
#include "loadthread.h"



extern BoxHandle boxHandel;
extern YContainerManager* g_moduleManager ;


int main(int argc, char *argv[])
{  
    //QApplication a(argc, argv);
    PlatformBase* app = createApplication(argc,argv);
    QStringList args = qApp->arguments();

    if(app->isAlreadyRunning()){
        QString args ;
        for(int i=1;i<argc;i++)
        {
            args.append(" ");
            args.append(argv[i]);
        }
        app->sendInstanceCommand(args);
        return 0;
    }
    app->registeMsgReceivedSlot(&boxHandel,SLOT(OnInstanceMsgReceived(const QString&)));

    g_moduleManager = new YContainerManager();


    // startup main Window and all setting
    boxHandel.mainStartup();

    app->setStyleSheet("file:///F:/Tech/QT/depoly/style.qss");


    //load Plugins
    LoadThread loadThread;
    loadThread.start();

    return platform->exec();
}
