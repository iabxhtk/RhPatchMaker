#include "patchmaker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("RhPatchMaker");
    a.setOrganizationName("kthxbai toolz");


    PatchMaker w;
    w.show();

    return a.exec();
}
