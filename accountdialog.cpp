#include "accountdialog.h"
#include <QPushButton>

AccountDialog::AccountDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

AccountDialog::~AccountDialog()
{
}

void AccountDialog::set_default_words(QString name, QString username, QString password)
{
    ui.lineEdit_name->setText(name);
    ui.lineEdit_username->setText(username);
    ui.lineEdit_password->setText(password);
}

Account AccountDialog::get_result()
{
    return result;
}

void AccountDialog::closeEvent(QCloseEvent* event)
{
    reject();
}

void AccountDialog::on_pushButton_ok_clicked()
{
    result.name = ui.lineEdit_name->text().toStdString();
    result.username = ui.lineEdit_username->text().toStdString();
    result.password = ui.lineEdit_password->text().toStdString();
    accept();
}

void AccountDialog::on_pushButton_cancel_clicked()
{
    reject();
}