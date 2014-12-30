#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
#include <QStandardItem>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = NULL;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(UpdateTableView()));
    timer->stop();
    model = new QStandardItemModel(1,3,this); //1 Row and 3 Columns
    ui->tableView->setModel(model);
    InitModel();
    bPause = false;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(manager!=NULL) delete manager;
}

void MainWindow::on_StartButton_clicked()
{
    QString strTextToSearch = ui->TestToSearchEdit->text();
    QString strURL = ui->URLEdit->text();
    int nMaxThreads = ui->MaxThreadSpin->value();
    int nURLCount = ui->MaxUrlCountSpin->value();
    manager = new URLManager(strURL, nMaxThreads, nURLCount, strTextToSearch);
    manager->setStart();
    SetFirstURL();
    timer->start(1000);
    UpdateTableView();
}

void MainWindow::on_StopButton_clicked()
{
  timer->stop();
  if(manager!=NULL)
    manager->setStop();
}

void MainWindow::on_PauseButton_clicked()
{
   bPause?timer->start():timer->stop();
   if(manager!=NULL)
    manager->setPause();
   ui->PauseButton->setText(bPause?"Pause":"Continue");
   bPause = !bPause;
}

void MainWindow::UpdateTable()
{
    int nRowCount = model->rowCount();
    QList<URL>* pList = manager->getURLlist();
    int nURLCount = pList->size();
    for(int i=0;i<nURLCount;i++)
    {
        if(i<nRowCount)
        {
            model->setItem(i,1,new QStandardItem((*pList)[i].getLoadedStatus()));
            model->setItem(i,2,new QStandardItem((*pList)[i].getFoundStatus()));
        }
        else
        {
            QList<QStandardItem*> row;
            row.append(new QStandardItem((*pList)[i].strURL));
            row.append(new QStandardItem((*pList)[i].getLoadedStatus()));
            row.append(new QStandardItem((*pList)[i].getFoundStatus()));

            model->appendRow(row);
        }
    }
}

void MainWindow::SetFirstURL()
{
    QStandardItem* item = new QStandardItem(ui->URLEdit->text());
    InitModel();
    model->setItem(0,0,item);
}

void MainWindow::UpdateTableView()
{
    if(manager!=NULL)
        manager->doWork();
    UpdateTable();
}

void MainWindow::InitModel()
{
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("URL")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Loaded")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Found")));
}
