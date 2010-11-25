/***************************************************************************
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 * Copyright (C) 2010 Kadek Wisnu Arsadhi kadek.wisnu@gmail.com            *
 *                                                                         *
 *                                                                         *
 * iso8583 extract - Extract bit fields of any ISO 8583 financial          *
 * based messages under GPL Licence                                        *
 *                                                                         *
 ***************************************************************************/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui/QMessageBox>
#include <QDesktopWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QMapIterator>
#include <QList>
#include <QStringList>
#include "isoextract.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(TITLE);
    setWindowIcon(QIcon(ICON));

    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint);
    setFixedSize(WIDTH,HEIGHT);

    QDesktopWidget *desktop = QApplication::desktop();
    move(desktop->width()/2-WIDTH/2,(desktop->height()/2-HEIGHT/2)-50);

    isoExtract = new IsoExtract(QApplication::applicationDirPath());

}

MainWindow::~MainWindow()
{
    delete ui;
    delete isoExtract;
}

void MainWindow::on_extractBtn_clicked()
{
    QString msg = ui->inputTxt->text();
    if (msg.isEmpty())
        return;
    isoExtract->process(msg);

    QTreeWidget* tree = ui->resultTree;
    tree->clear();

    QTreeWidgetItem *parentItem = new QTreeWidgetItem(tree,QStringList(isoExtract->msgType));

    QList<QTreeWidgetItem *> items;

    QMap<int, IsoField*> isoMsgMap = isoExtract->isoMessage;

    QMapIterator<int, IsoField*> i(isoMsgMap);
    while (i.hasNext()) {
        i.next();
        QStringList bitmsg;
        bitmsg << QString("Bit %1").arg(i.key()) << i.value()->lengthType << i.value()->value;
        items.append(new QTreeWidgetItem((QTreeWidget*)0, bitmsg));
    }
    parentItem->addChildren(items);
    tree->expandItem(parentItem);
//    tree->setSelectionMode(QAbstractItemView::ExtendedSelection);

}

void MainWindow::on_infoBtn_clicked()
{
    about();
}

void MainWindow::on_sampleBtn_clicked()
{
    ui->inputTxt->text();
    ui->inputTxt->setText("0200323A644B88818000160001000000020000120103033129341410033112011201601100002100C00000000C0000000008    054 07000026    00000003700012013018TLK081311081322   360");
    on_extractBtn_clicked();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About iso8583 Extract"),
            tr("The <b>iso8583 Extract</b> v.1.0.0 (25.11.2010)"
               "<br />"
               "<br />\"Extract bit fields of any ISO 8583 financial based messages\""
               "<br />\"It can be adjusted in order to match any messages\""
               "<br />\"Configuration is based on XML with simple, clear, and readable format\""
               "<br />\"Open source under GPL license, implement modern look and feel interface\""
               "<br />\"This software uses the QSkinObject - another great open source project\""
               "<br />\"Check out on http://sourceforge.net/projects/qskinwindows/"

               "<br /><br />(c) 2010 Kadek Wisnu Arsadhi - <i>kadek.wisnu@gmail.com</i>"

               ));
}
