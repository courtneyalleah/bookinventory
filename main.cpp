#include "book_inventory.h"
#include <QApplication>
#include <QTableWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Book_Inventory w;
    w.resize(1000,650);
    w.show();
    w.setWindowTitle("Books Galore Inventory");



    return a.exec();
}
