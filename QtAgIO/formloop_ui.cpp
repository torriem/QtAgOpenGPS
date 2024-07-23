#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>
#include <QScreen>
#include <QGuiApplication>
#include <QQmlEngine>
#include "qmlutil.h"
#include "formloop.h"

void FormLoop::setupGUI()
{
    //Load the QML UI and display it in the main area of the GUI
    setProperty("title", "QtAgIO");

    //load the QML into a view
    rootContext()->setContextProperty("screenPixelDensity",QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio());
    rootContext()->setContextProperty("mainForm", this);
    rootContext()->setContextProperty("prefix", ":");
    load(QUrl("qrc:/qml/Main.qml"));
}
