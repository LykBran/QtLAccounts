#include "mainwindow.h"
#include "accountdialog.h"
#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QStatusBar>
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
    dialog->setWindowTitle(tr("Add Account"));
    if (dialog->exec() == QDialog::Rejected)
    {
        return;
    }
    dialog->close();

    Account account = dialog->get_result();
    manager->write_account(account);
    QString text = account.name.c_str() + tr("  username:") + account.username.c_str() + tr("  password:") + account.password.c_str();
    ui.listWidget->addItem(text);
    ui.statusBar->showMessage(tr("Succesfully added the account!"), 3000);
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
    ui.statusBar->showMessage(tr("Succesfully removed the account!"), 3000);
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

    dialog->setWindowTitle(tr("Update Account"));
    if (dialog->exec() == QDialog::Rejected)
    {
        return;
    }
    dialog->close();

    Account account = dialog->get_result();
    manager->edit_account(index + 1, account);
    QString text = account.name.c_str() + tr("  username:") + account.username.c_str() + tr("  password:") + account.password.c_str();
    item->setText(text);
    ui.statusBar->showMessage(tr("Succesfully updated the account!"), 3000);
}

void MainWindow::on_actionOpen_Accounts_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose the file to store accounts"), ".", tr("LAccounts Database (*.lad)"));
    if (filename == "") { return; }
    if (ui.listWidget->isEnabled())
    {
        ui.listWidget->clear();
        delete manager;
        ui.statusBar->removeWidget(file_label);
        delete file_label;
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
    manager = new AccountsManager(filename.toStdString());
    ui.listWidget->setEnabled(true);
    for (Account account : manager->all_accounts())
    {
        QString text = account.name.c_str() + tr("  username:") + account.username.c_str() + tr("  password:") + account.password.c_str();
        ui.listWidget->addItem(text);
    }
    file_label = new QLabel(this);
    file_label->setText(filename);
    ui.statusBar->addPermanentWidget(file_label);
    ui.statusBar->showMessage(tr("Succesfully opened the account!"), 3000);
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