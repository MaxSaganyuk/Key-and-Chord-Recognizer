#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdint>
#include <QApplication>
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.show();
    //w.ShowNoteTest();


    return a.exec();
}
