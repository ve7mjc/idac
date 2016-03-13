#include <QtCore/QCoreApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Controller controller;

    QString token = "1234";

    // Call QCoreApplication holding loop
    // if config loaded correctly
    if (!controller.loadConfig(token))
        return a.exec();

//    if (a.arguments().count() <= 1) {
//        qDebug() << "please supply a config file\r\nexample: acd --config yourconfig.conf";
//        return true;
//    }

//    // Process CL Arguments
//    QStringList args = a.arguments();

//    // Recover Config File
//    QString configFile;
//    for (int i = 0; i < args.count(); i++)
//    {
//        if (((args.at(i) == "-c") || (args.at(i) == "--config")) && (args.count() > (i+1)))
//            configFile = args.at(i+1);
//    }

//    AccessController ac(configFile);
//    ac.start();


}
