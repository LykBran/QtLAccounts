#pragma once

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include "ui_mainwindow.h"
#include "AccountsManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    AccountsManager* manager = nullptr;
    QLabel* file_label = nullptr;

private slots:
    void on_actionAdd_triggered();
    void on_actionRemove_triggered();
    void on_actionUpdate_triggered();
    void on_actionOpen_Accounts_File_triggered();

    void on_actionCopy_Username_triggered();
    void on_actionCopy_Password_triggered();
    void on_actionCopy_triggered();
};
