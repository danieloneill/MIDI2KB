#include <QApplication>

#include "mainwindow.h"
#include "listener.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("Reinsoft");
    a.setOrganizationDomain("reinsoft.org");
    a.setApplicationName("DummyDrummer");

    MainWindow w;
    w.show();

    Listener l(&w);

    l.connect( &w, &MainWindow::connectClicked, &l, &Listener::connectClicked );
    l.connect( &l, &Listener::connectedChanged, &w, &MainWindow::connectedChanged );
    l.connect( &l, &Listener::message, &w, &MainWindow::setStatus );
    l.connect( &l, &Listener::devicesScanned, &w, &MainWindow::scanResults );
    w.connect( &w, &MainWindow::scanRequested, &l, &Listener::rescan );
    w.connect( &w, &MainWindow::destroyed, &a, &QApplication::quit );
    w.connect( &w, &MainWindow::quitRequested, &a, &QApplication::quit );
    l.connect( &l, &Listener::midiEvent, &w, &MainWindow::midiEvent );
    w.connect( &w, &MainWindow::queueKey, &l, &Listener::queueKey );

    l.rescan();

    return a.exec();
}
