#include <QApplication>
#include "window.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window window;
    window.setFixedSize(830, 420);
    window.show();
    return a.exec();
}
