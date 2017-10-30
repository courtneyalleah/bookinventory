#include "book_inventory.h"
#include "ui_book_inventory.h"

double profit = 0;
Book_Inventory::Book_Inventory(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Book_Inventory)
{

    ui->setupUi(this);

    // user cannot edit tables directly
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->retailPriceLineEdit->setValidator(new QDoubleValidator(0,10000, 2, this));
    ui->wholesalePriceLineEdit->setValidator(new QDoubleValidator(0,10000,2,this));

    QFile file("test.txt");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     while (!file.atEnd()) {




         QTextStream in(&file);
         QList< QStringList > lists;
         QString line;
         do {
             line = in.readLine();
             lists << line.split("\t");
         } while (!line.isNull());

         // Set the table size (assuming the rows are all the same length).
         ui->tableWidget->setRowCount( lists.size() );
         ui->tableWidget->setColumnCount( lists[0].size() );

         for ( int row = 0; row < lists.size(); ++row ) {
             for ( int column = 0; column < lists[row].size(); ++column ) {
                 ui->tableWidget->setItem(row, column, new QTableWidgetItem(lists[row][column]));
             }
         }


     }
}

Book_Inventory::~Book_Inventory()
{
    delete ui;
}

void Book_Inventory::on_Exit_clicked()
{
    QCoreApplication::quit();
}

void Book_Inventory::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// opens new window to display the current inventory
void Book_Inventory::on_pushButton_6_clicked()
{
    QTableWidget *table = new QTableWidget;
    QString data;

    table->setColumnCount(ui->tableWidget->columnCount());
    table->setRowCount(ui->tableWidget->rowCount());
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("ID"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Retail Price"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Wholesale Price"));
    table->setColumnWidth(3, 120);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setFixedSize(600,500);
    table->show();

    int columns=table->columnCount();
    int rows=table->rowCount();

    // copies table from homepage to table in new window
    for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            data = ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString();
            table->setItem(i,j,new QTableWidgetItem(data));
        }
    }

}

// handles deletion of book in inventory
void Book_Inventory::on_pushButton_2_clicked()
{

    QItemSelectionModel *selectionModel = ui->tableWidget->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    if (selectedRows.size() > 0) {
        // There is at lease one selected row.

        QMessageBox msg;
        msg.setText("Are you sure you want to delete the selected book(s)?");
        msg.setStandardButtons(QMessageBox::Yes |QMessageBox::No);
        msg.setDefaultButton(QMessageBox::Cancel);
        int ret = msg.exec();
        int countRow = selectedRows.count();

        // determine what should be done after user decides to delete/not delete book
        switch(ret){
           case QMessageBox::Yes:

                // iterate through selected books and delete
                for(int i =countRow;i>0;i--)
                    ui->tableWidget->removeRow(selectedRows.at(i-1).row());
                break;

           case QMessageBox::No:
                msg.close();
                break;
        }
    }

    else{
        QMessageBox msg;
        msg.setText("You have not selected a book to remove.");
        msg.exec();
    }
}

// handles purchase request by user
 void Book_Inventory::on_pushButton_3_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableWidget->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    if (selectedRows.size() > 0) {
        // There is at lease one selected row.

        QMessageBox msg;
        msg.setText("Are you sure you want to purchase the selected book(s)?");
        msg.setStandardButtons(QMessageBox::Yes |QMessageBox::No);
        msg.setDefaultButton(QMessageBox::Cancel);
        int ret = msg.exec();

        // determine what should be done after user decides to purchase/not purchase book
        switch(ret){

            // book has been purchased and user will see informationa bout their purchase
           case QMessageBox::Yes:
                {
                    QString data;
                    double countProfit;
                    QString copy;
                    QWidget *confirmation = new QWidget;
                    QLabel *label = new QLabel(this);
                    QLabel *label2 = new QLabel(this);
                    QGridLayout *gridLayout = new QGridLayout;
                    QTableWidget *purchaseComplete = new QTableWidget(this);


                    purchaseComplete->setEditTriggers(QAbstractItemView::NoEditTriggers);
                    purchaseComplete->setColumnCount(3);
                    purchaseComplete->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                    purchaseComplete->setHorizontalHeaderItem(0,new QTableWidgetItem("Book"));
                    purchaseComplete->setHorizontalHeaderItem(1,new QTableWidgetItem("Book ID"));
                    purchaseComplete->setHorizontalHeaderItem(2,new QTableWidgetItem("Purchase Amount"));
                    purchaseComplete->setRowCount(selectedRows.size());

                    // display books the user purchased in the pop up window
                    for(int i=0; i<purchaseComplete->rowCount();i++)
                    {
                        //add row to transaction table.
                        ui->transactionsTable->insertRow(ui->transactionsTable->rowCount());
                        ui->transactionsTable->setItem(ui->transactionsTable->rowCount()-1,0,new QTableWidgetItem(QDate::currentDate().toString()));

                        for(int j=0;j<purchaseComplete->columnCount()*purchaseComplete->rowCount();j++){
                            data =selectionModel->selectedRows(j).value(i).data().toString();

                            // data = index.model()->data(index.model()->index(i,j)).toString();
                            purchaseComplete->setItem(i,j,new QTableWidgetItem(data));

                            // copy purchased info to transaction history
                            copy = purchaseComplete->model()->data(purchaseComplete->model()->index(i,j)).toString();

                            ui->transactionsTable->setItem(ui->transactionsTable->rowCount()-1,j+1,new QTableWidgetItem(copy));
                        }

                    }


                    // calculate and update profit
                    for(int i=0; i<purchaseComplete->rowCount();i++){
                        countProfit=purchaseComplete->item(i,2)->text().toDouble();
                        profit+=countProfit;
                    }

                        ui->totalProfit->setText("$"+QString::number(profit));

                label->setText("Your book has been purchased! You can view information about your transaction below.");
                label2->setText("Transaction for: " +QDate::currentDate().toString());
                gridLayout->addWidget(label,0,0,1,1);
                gridLayout->addWidget(label2,1,0,1,1);
                gridLayout->addWidget(purchaseComplete,2,0,1,1);
                confirmation->setLayout(gridLayout);
                confirmation->setWindowTitle("Purchase complete!");
                confirmation->show();

                // iterate through purchased books and delete
                for(int i=selectedRows.count();i>0;i--){
                    ui->tableWidget->removeRow(selectedRows.at(i-1).row());
                }

                break;

                }

           case QMessageBox::No:
                msg.close();
                break;
        }
    }

    else{
        QMessageBox msg;
        msg.setText("You have not selected a book to purchase.");
        msg.exec();
    }
}

void Book_Inventory::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


// cancel add book request, return to homepage
void Book_Inventory::on_cancelBox_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->bookNameLineEdit->clear();
    ui->bookIDLineEdit->clear();
    ui->retailPriceLineEdit->clear();
    ui->wholesalePriceLineEdit->clear();
}

// After clicking ok, there will be error message if missing information. Otherwise, inventory will update and a message box appears.
void Book_Inventory::on_okBox_clicked()
{
    // error message
    QMessageBox missingInfoError;
    if(ui->bookNameLineEdit->text().isEmpty()||ui->bookIDLineEdit->text().isEmpty()||ui->retailPriceLineEdit->text().isEmpty()||ui->wholesalePriceLineEdit->text().isEmpty())
    {
        missingInfoError.setText("Please fill in required fields.");
        missingInfoError.exec();
    }

    else
    {
    // add new row
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    QString name;
    QString id;
    QString retailPrice;
    QString wholesalePrice;

    // add name to table
    name = ui->bookNameLineEdit->text();
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(name));

    // add id to table
    id = ui->bookIDLineEdit->text();
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(id));

    // add retail price to table
    retailPrice = ui->retailPriceLineEdit->text();
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(retailPrice));

    // add wholesale price to table
    wholesalePrice = ui->wholesalePriceLineEdit->text();
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(wholesalePrice));

    ui->bookNameLineEdit->clear();
    ui->bookIDLineEdit->clear();
    ui->retailPriceLineEdit->clear();
    ui->wholesalePriceLineEdit->clear();

    ui->stackedWidget->setCurrentIndex(0);
    QMessageBox msg;
    msg.setText("Your book has been added to the inventory.");
    msg.exec();
    }
}

void Book_Inventory::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



