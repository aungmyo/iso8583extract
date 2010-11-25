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

#include "isoextract.h"
#include <QDebug>
#include <QFile>
#include <QtGui/QMessageBox>
#include "isofield.h"

IsoExtract::IsoExtract(QObject *parent) : QObject(parent)
{
}

IsoExtract::IsoExtract(const QString& appPath)
{
    applicationPath = appPath;
}

void IsoExtract::process(QString message)
{
    msg = message;
    isoMessage.clear();
    readConfig();

    int headerLength = isoconfig.value(0).toInt();
    int bitmapLength = 16;
    msgType = message.mid((0+headerLength),4);

//    qDebug() << "Iso Msg: " << message;
//    qDebug() << "Msg typ: " << message.mid((0+headerLength),4);
//    qDebug() << "Primary bit map: " << message.mid((4+headerLength),16);
//    qDebug() << "Bit 4: " << message.mid((4+headerLength),1).toInt(0,16);
//    qDebug() << "Test: " << QString("009").toInt();


    if (message.mid((4+headerLength),1).toInt(0,16) >= 8)
    {
//        qDebug() << "Secondary bit map: " << message.mid((20+headerLength),16);
        bitmapLength = 32;
    }

    bitMap = msg.mid((4+headerLength),bitmapLength);

    checkActiveBit(bitMap);

}


void IsoExtract::checkActiveBit(const QString& bitMap)
{
    QString modifMsg = msg;
    int bit = 0;
    int factor = 0;
    int bitMapLength = bitMap.length();
    int headerLength = isoconfig.value(0).toInt();
    if (headerLength > 0)
        modifMsg = modifMsg.mid(headerLength);

    msgType = modifMsg.mid(0,4);
    modifMsg = modifMsg.mid((4+bitMapLength));

    // save bitmap data in bit 1
    isoMessage.insert(1, new IsoField(QString("%1").arg(bitMapLength),bitMap));

    for (int i = 0; i < bitMapLength; i++)
    {
        bit = bitMap.mid(i,1).toInt(0,16);
        factor = 4*i;
        switch (bit) {
        case 1:
            saveExtractedBitValue(4+factor,modifMsg);
            break;
        case 2:
            saveExtractedBitValue(3+factor,modifMsg);
            break;
        case 3:
            saveExtractedBitValue(3+factor,modifMsg);
            saveExtractedBitValue(4+factor,modifMsg);
            break;
        case 4:
            saveExtractedBitValue(2+factor,modifMsg);
            break;
        case 5:
            saveExtractedBitValue(2+factor,modifMsg);
            saveExtractedBitValue(4+factor,modifMsg);
            break;
        case 6:
            saveExtractedBitValue(2+factor,modifMsg);
            saveExtractedBitValue(3+factor,modifMsg);
            break;
        case 7:
            saveExtractedBitValue(2+factor,modifMsg);
            saveExtractedBitValue(3+factor,modifMsg);
            saveExtractedBitValue(4+factor,modifMsg);
            break;
        case 8:
            saveExtractedBitValue(1+factor,modifMsg);
            break;
        case 9:
            saveExtractedBitValue(1+factor,modifMsg);
            saveExtractedBitValue(4+factor,modifMsg);
            break;
        case 10:
            saveExtractedBitValue(1+factor,modifMsg);
            saveExtractedBitValue(3+factor,modifMsg);
            break;
        case 11:
            saveExtractedBitValue(1+factor,modifMsg);
            saveExtractedBitValue(3+factor,modifMsg);
            saveExtractedBitValue(4+factor,modifMsg);
            break;
        case 12:
            saveExtractedBitValue(1+factor,modifMsg);
            saveExtractedBitValue(2+factor,modifMsg);
            break;
        case 13:
            saveExtractedBitValue(1+factor,modifMsg);
            saveExtractedBitValue(2+factor,modifMsg);
            saveExtractedBitValue(4+factor,modifMsg);
            break;
        case 14:
            saveExtractedBitValue(1+factor,modifMsg);
            saveExtractedBitValue(2+factor,modifMsg);
            saveExtractedBitValue(3+factor,modifMsg);
            break;
        case 15:
            saveExtractedBitValue(1+factor,modifMsg);
            saveExtractedBitValue(2+factor,modifMsg);
            saveExtractedBitValue(3+factor,modifMsg);
            saveExtractedBitValue(4+factor,modifMsg);
            break;
        }

    }

    // debug
//    QMapIterator<int, IsoField*> i(isoMessage);
//    while (i.hasNext()) {
//        i.next();
//        qDebug() << i.key() << ": " << i.value()->lengthType << ", " << i.value()->value << endl;
//    }

}

void IsoExtract::saveExtractedBitValue(int bitNo,QString& modifMessage)
{
    int length = 0;
    QString lengthType = isoconfig.value(bitNo);
    if (lengthType == "LLVAR")
    {
        length = modifMessage.mid(0,2).toInt();
        modifMessage = modifMessage.mid(2);
    }
    else if (lengthType == "LLLVAR")
    {
        length = modifMessage.mid(0,3).toInt();
        modifMessage = modifMessage.mid(3);
    }
    else
        length = lengthType.toInt();


    QString arg1 = lengthType;
    QString arg2 = modifMessage.mid(0,length);
    if (bitNo == 1)
    {
        arg1 = QString("%1").arg(bitMap.length());
        arg2 = bitMap;
    }
    else
        modifMessage = modifMessage.mid(length);

    IsoField* isoField = new IsoField(arg1,arg2);
    isoMessage.insert(bitNo,isoField);
}


void IsoExtract::readConfig()
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    QString filePath = applicationPath + "/config.xml";
//    qDebug() << filePath << endl;
    QFile file(filePath);
    bool error = false;

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "WARNING:" << file.fileName() << " - " << file.errorString() << endl;
        error = true;
    }

    if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
                                &errorColumn)) {
        qWarning() << "WARNING:" << file.fileName() << " - " << errorStr << endl;
        error = true;
    }

    if (error)
    {
        QFile defaultConfig(":/config.xml");
        domDocument.setContent(&defaultConfig, true, &errorStr, &errorLine, &errorColumn);
    }

    QDomElement root = domDocument.documentElement();
//    QDomElement config = root.firstChildElement("iso8583-extract-config");
    persist(root);

}

void IsoExtract::persist(const QDomElement& element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull())
    {
        QString tag = child.tagName();
        if (tag == "header")
            isoconfig.insert(0,child.attribute("length"));
        else if (tag == "bit")
            isoconfig.insert(child.attribute("number").toInt(),child.attribute("length"));

        child = child.nextSiblingElement();
    }

//    QHashIterator<int, QString> i(isoconfig);
//    while (i.hasNext()) {
//        i.next();
//        qDebug() << i.key() << ": " << i.value() << endl;
//    }
}
