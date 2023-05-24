#pragma once

#include <QButtonGroup>
#include <QtWidgets/QDialog>
#include "ui_accountdialog.h"
#include "AccountsManager.h"

class AccountDialog : public QDialog
{
    Q_OBJECT

public:
    AccountDialog(QWidget* parent = nullptr);
    ~AccountDialog();
    void set_default_words(QString name, QString username, QString password);
    Account get_result();

private:
    Ui::AccountDialog ui;
    Account result;
    void closeEvent(QCloseEvent* event);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
};
