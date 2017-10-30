#ifndef BOOK_INVENTORY_H
#define BOOK_INVENTORY_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QStack>
#include <QAbstractButton>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QDir>
#include <QAbstractItemView>
#include <QLabel>
#include <QGridLayout>
#include <QDate>
#include <QDoubleValidator>
#include <QDebug>
#include <QByteArray>
#include <QFileDialog>

namespace Ui {
class Book_Inventory;
}

class Book_Inventory : public QMainWindow
{
    Q_OBJECT


public:
    explicit Book_Inventory(QWidget *parent = 0);

    ~Book_Inventory();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_Exit_clicked();

    void on_cancelBox_clicked();

    void on_okBox_clicked();

    void on_backButton_clicked();

private:

    Ui::Book_Inventory *ui;
};

#endif // BOOK_INVENTORY_H
