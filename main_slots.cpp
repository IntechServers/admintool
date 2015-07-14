#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "worker.h"
#include "query.h"
#include "serverinfo.h"
#include "settings.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QPalette>

extern QPalette defaultPalette;
extern QMap<int, QString> appIDMap;
extern Settings *settings;
extern QList<ServerInfo *> serverList;
extern QColor errorColor;
extern QColor queryingColor;

void MainWindow::ConnectSlots()
{
    this->ui->commandText->connect(this->ui->commandText, &QLineEdit::returnPressed, this, &MainWindow::processCommand);
    this->ui->rconPassword->connect(this->ui->rconPassword, &QLineEdit::returnPressed, this, &MainWindow::rconLogin);
    this->ui->rconPassword->connect(this->ui->rconPassword, &QLineEdit::textChanged, this, &MainWindow::passwordUpdated);
    this->ui->rconSave->connect(this->ui->rconSave, &QCheckBox::toggled, this, &MainWindow::rconSaveToggled);
    this->ui->commandText->connect(this->ui->commandOutput, &QPlainTextEdit::textChanged, this, &MainWindow::commandOutputUpdated);
    this->ui->rconLogin->connect(this->ui->rconLogin, &QPushButton::released, this, &MainWindow::rconLogin);
    this->ui->actionAdd_Server->connect(this->ui->actionAdd_Server, &QAction::triggered, this, &MainWindow::addServer);
    this->ui->actionDark_Theme->connect(this->ui->actionDark_Theme, &QAction::triggered, this, &MainWindow::darkThemeTriggered);
    this->ui->browserTable->connect(this->ui->browserTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::browserTableItemSelected);
}


void MainWindow::addServer()
{
    QMessageBox message(this);
    while(true)
    {
        bool ok;
        QString server = QInputDialog::getText(this, tr("Add Server"), tr("IP:Port"), QLineEdit::Normal,"", &ok);

        if (ok && !server.isEmpty())
        {
            AddServerError error = this->CheckServerList(server);
            if(error == AddServerError_None)
            {
                ServerInfo *info = new ServerInfo(server);
                serverList.append(info);

                settings->SaveSettings();

                int row = this->ui->browserTable->rowCount();
                this->ui->browserTable->insertRow(row);

                QTableWidgetItem *item = new QTableWidgetItem();
                QTableWidgetItem *id = new QTableWidgetItem();
                id->setData(Qt::DisplayRole, row+1);

                item->setTextColor(queryingColor);
                item->setText(QString("Querying server %1...").arg(server));
                this->ui->browserTable->setItem(row, 0, id);
                this->ui->browserTable->setItem(row, 4, item);

                InfoQuery *infoQuery = new InfoQuery(this);
                infoQuery->query(info, id);
                break;
            }
            else if(error == AddServerError_AlreadyExists)//Valid ip but exists.
            {
                message.setText("Server already exists");
                message.exec();
                break;
            }
            else
            {
                message.setText("Invalid IP or Port");
                message.exec();
            }
        }
        else
            break;
    }
}

void MainWindow::browserTableItemSelected()
{
    if(this->ui->browserTable->selectedItems().size() == 0)
    {
        this->ui->rconPassword->setEnabled(false);
        this->ui->commandText->setEnabled(false);
        this->ui->commandOutput->setEnabled(false);
        this->ui->rconSave->setEnabled(false);
        this->ui->rconSave->setChecked(false);
        this->ui->rconLogin->setEnabled(false);
        return;
    }

    this->ui->rconPassword->setEnabled(true);
    this->ui->commandText->setEnabled(true);
    this->ui->commandOutput->setEnabled(true);
    this->ui->rconSave->setEnabled(true);
    this->ui->rconLogin->setEnabled(true);

    QTableWidgetItem *item = this->ui->browserTable->selectedItems().at(0);
    int index = item->text().toInt();

    this->ui->rconSave->blockSignals(true);
    this->ui->rconPassword->blockSignals(true);
    this->ui->commandOutput->blockSignals(true);
    this->ui->rconSave->setChecked(serverList.at(index-1)->saveRcon);
    this->ui->rconPassword->setText(serverList.at(index -1)->rconPassword);
    this->ui->commandOutput->setPlainText(serverList.at(index-1)->rconOutput);
    this->ui->commandOutput->moveCursor(QTextCursor::End);
    this->ui->rconSave->blockSignals(false);
    this->ui->rconPassword->blockSignals(false);
    this->ui->commandOutput->blockSignals(false);

    this->UpdateSelectedItemInfo(true, true);
}

void MainWindow::darkThemeTriggered()
{
    if(this->ui->menuTheme->actions().at(0)->isChecked())
    {
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(50,50,50));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(60,60,60));
        palette.setColor(QPalette::AlternateBase, QColor(80,80,80));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(50,50,50));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);

        palette.setColor(QPalette::Highlight, QColor(80,80,80).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(palette);
    }
    else
    {
        qApp->setPalette(defaultPalette);
    }
}