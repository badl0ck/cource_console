#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include <QTime>
#include <iostream>
#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include "auxilary.h"
#include "auxilary.cpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Auxilary aux;
    aux.startRunning();

    return a.exec();
}
