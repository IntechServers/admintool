#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "settings.h"
#include <QMessageBox>
#include <QClipboard>

extern Settings *settings;
extern QList<ServerInfo *> serverList;

void MainWindow::HookEvents()
{
    this->ui->browserTable->installEventFilter(this);
    this->ui->commandText->installEventFilter(this);
    this->ui->sendChat->installEventFilter(this);
    this->ui->playerTable->installEventFilter(this);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings->SaveSettings();
    QMainWindow::closeEvent(event);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if((object == this->ui->commandText || object == this->ui->sendChat) && event->type() == QEvent::KeyPress)
    {
        Qt::Key key = (Qt::Key)(((QKeyEvent *)event)->key());
        if(key == Qt::Key_Up)
        {
            if(object == this->ui->commandText && this->commandHistory.size() > 0)
            {
                if(!this->commandIter->hasNext())
                {
                    this->commandIter->toFront();
                }
                this->ui->commandText->setText(this->commandIter->next());
            }
            else if(object == this->ui->sendChat && this->sayHistory.size() > 0)
            {
                if(!this->sayIter->hasNext())
                {
                    this->sayIter->toFront();
                }
                this->ui->sendChat->setText(this->sayIter->next());
            }
            return true;
        }
        else if(key == Qt::Key_Down)
        {
            if(object == this->ui->commandText && this->commandHistory.size() > 0)
            {
                if(!this->commandIter->hasPrevious())
                {
                    this->commandIter->toBack();
                }
                this->ui->commandText->setText(this->commandIter->previous());
            }
            else if(object == this->ui->sendChat && this->sayHistory.size() > 0)
            {
                if(!this->sayIter->hasPrevious())
                {
                    this->sayIter->toBack();
                }
                this->ui->sendChat->setText(this->sayIter->previous());
            }
            return true;
        }
    }
    else if(object == this->ui->browserTable && this->ui->browserTable->selectedItems().size() && event->type() == QEvent::KeyPress)
    {
        Qt::Key key = (Qt::Key)(((QKeyEvent *)event)->key());
        if(key == Qt::Key_Delete)
        {
            int index = this->ui->browserTable->selectedItems().at(0)->text().toInt();

            QMessageBox message(this);
            message.setInformativeText(QString("Delete %1?").arg(serverList.at(index-1)->ipPort));
            message.setText("Delete server from list?");
            message.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            message.setDefaultButton(QMessageBox::Cancel);
            int ret = message.exec();

            if(ret == QMessageBox::Ok)
            {
                for(int i = 0; i < this->ui->browserTable->rowCount(); i++)
                {
                    QTableWidgetItem *item = this->ui->browserTable->item(i, 0);

                    int other = item->data(Qt::DisplayRole).toInt();

                    if(other == index)
                    {
                        this->ui->browserTable->removeRow(i);
                        break;
                    }
                }

                for(int i = 0; i < this->ui->browserTable->rowCount(); i++)
                {
                    QTableWidgetItem *item = this->ui->browserTable->item(i, 0);

                    int other = item->data(Qt::DisplayRole).toInt();

                    if(other > index)
                    {
                        item->setData(Qt::DisplayRole, other-1);
                    }
                }

                ServerInfo *info = serverList.at(index-1);
                pLogHandler->removeServer(info);
                delete info;

                serverList.removeAt(index-1);
                settings->SaveSettings();
                if(this->ui->browserTable->selectedItems().size() == 0)
                {
                   //Clear rules and players
                    while(this->ui->rulesTable->rowCount() > 0)
                    {
                        this->ui->rulesTable->removeRow(0);
                    }

                    while(this->ui->playerTable->rowCount() > 0)
                    {
                        this->ui->playerTable->removeRow(0);
                    }

                    while(this->ui->infoTable->rowCount() > 0)
                    {
                        this->ui->infoTable->removeRow(0);
                    }
                    this->ui->chatOutput->setHtml("");
                    this->ui->commandOutput->setPlainText("");
                    this->ui->rconPassword->setText("");
                    this->ui->rconSave->setChecked(false);
                }
                return true;
            }
        }
    }
    else if(object == this->ui->playerTable && this->ui->playerTable->selectedItems().size() && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = (QKeyEvent *)event;

        if(key_event->matches(QKeySequence::Copy))
        {
            QClipboard *clipboard = QApplication::clipboard();

            QTableWidgetItem *item;
            QString copy;

            int row = -1;

            foreach(item, this->ui->playerTable->selectedItems())
            {
                if(row == -1)
                {
                    row = item->row();
                }
                else if(item->row() == row)
                {
                    copy.append('\t');
                }
                else
                {
                    copy.append('\n');
                    row = item->row();
                }
                copy.append(item->text());
            }
            clipboard->setText(copy);
            return true;
        }
    }
    return QMainWindow::eventFilter(object, event);
}
