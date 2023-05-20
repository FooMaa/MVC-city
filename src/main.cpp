#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    w.resize(1000,750);
    w.setWindowTitle("City");
    return a.exec();
}
