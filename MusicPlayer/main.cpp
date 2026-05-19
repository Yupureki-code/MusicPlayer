#include "stdafx.h"
#include "MusicPlayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MusicPlayer window;
    window.show();
    return app.exec();
}
