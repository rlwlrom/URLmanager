#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "urlmanager.h"
#include "downloader.h"
#include <QTimer>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void UpdateTableView();
    void on_StartButton_clicked();

    void on_StopButton_clicked();

    void on_PauseButton_clicked();

private:
    void UpdateTable();
    void SetFirstURL();
    void InitModel();

    Ui::MainWindow *ui;
    URLManager* manager;
    QTimer *timer;
    bool bPause;
    QStandardItemModel *model;

};

#endif // MAINWINDOW_H
