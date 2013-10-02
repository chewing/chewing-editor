#include "chewingeditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChewingEditor w;
    w.show();

    return a.exec();
}
