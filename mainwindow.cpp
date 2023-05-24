#include "mainwindow.h"
#include "accountdialog.h"
#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.listWidget->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete manager;
}

void MainWindow::on_actionAdd_triggered()
{
    AccountDialog* dialog = new AccountDialog(this);
    dialog->setWindowTitle("Add Account");
    if (dialog->exec() == QDialog::Rejected)
    {
        delete dialog;
        return;
    }
    dialog->close();

    Account account = dialog->get_result();
    manager->write_account(account);
    QString text = account.name.c_str() + tr("  username:") + account.username.c_str() + tr("  password:") + account.password.c_str();
    ui.listWidget->addItem(text);
    delete dialog;
}

void MainWindow::on_actionRemove_triggered()
{
    QList<QListWidgetItem*> items = ui.listWidget->selectedItems();
    if (items.isEmpty()) { return; }
    QListWidgetItem* item = items[0];
    size_t index = ui.listWidget->currentRow();

    manager->erase_account(index + 1);
    ui.listWidget->removeItemWidget(item);
    delete item;
}

void MainWindow::on_actionUpdate_triggered()
{
    QList<QListWidgetItem*> items = ui.listWidget->selectedItems();
    if (items.isEmpty()) { return; }
    QListWidgetItem* item = items[0];
    size_t index = ui.listWidget->currentRow();

    AccountDialog* dialog = new AccountDialog(this);
    Account old = manager->all_accounts()[index];
    dialog->set_default_words(old.name.c_str(), old.username.c_str(), old.password.c_str());

    dialog->setWindowTitle("Update Account");
    if (dialog->exec() == QDialog::Rejected)
    {
        delete dialog;
        return;
    }
    dialog->close();

    Account account = dialog->get_result();
    manager->edit_account(index + 1, account);
    QString text = account.name.c_str() + tr("  username:") + account.username.c_str() + tr("  password:") + account.password.c_str();
    item->setText(text);
    delete dialog;
}

void MainWindow::on_actionOpen_Accounts_File_triggered()
{
    if (ui.listWidget->isEnabled())
    {
        ui.listWidget->clear();
        delete manager;
    }
    else
    {
        ui.actionAdd->setEnabled(true);
        ui.actionRemove->setEnabled(true);
        ui.actionUpdate->setEnabled(true);
        ui.actionCopy_Username->setEnabled(true);
        ui.actionCopy_Password->setEnabled(true);
        ui.actionCopy->setEnabled(true);
    }
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose the file to store accounts"), ".", tr("LAccounts Database (*.lad)"));
    manager = new AccountsManager(filename.toStdString());
    ui.listWidget->setEnabled(true);
    for (Account account : manager->all_accounts())
    {
        QString text = account.name.c_str() + tr("  username:") + account.username.c_str() + tr("  password:") + account.password.c_str();
        ui.listWidget->addItem(text);
    }
}

void MainWindow::on_actionCopy_Username_triggered()
{
    QClipboard* clip = QApplication::clipboard();
    QList<QListWidgetItem*> items = ui.listWidget->selectedItems();
    if (items.isEmpty()) { return; }
    size_t index = ui.listWidget->currentRow();
    clip->setText(manager->all_accounts()[index].username.c_str());
}

void MainWindow::on_actionCopy_Password_triggered()
{
    QClipboard* clip = QApplication::clipboard();
    QList<QListWidgetItem*> items = ui.listWidget->selectedItems();
    if (items.isEmpty()) { return; }
    size_t index = ui.listWidget->currentRow();
    clip->setText(manager->all_accounts()[index].password.c_str());
}

void MainWindow::on_actionCopy_triggered()
{
    QClipboard* clip = QApplication::clipboard();
    QList<QListWidgetItem*> items = ui.listWidget->selectedItems();
    if (items.isEmpty()) { return; }
    QListWidgetItem* item = items[0];
    clip->setText(item->text());
}