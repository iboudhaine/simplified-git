#include "mainwindow.h"

#include <QApplication>
#include "includes/Git.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    // Git git("C:\\Users\\Ilyas\\Desktop\\testrepo");
    // git.repl();
}
