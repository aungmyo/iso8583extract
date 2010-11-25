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

#ifndef ISOEXTRACT_H
#define ISOEXTRACT_H

#include <QObject>
#include <QHash>
#include <QMap>
#include <QDomDocument>
#include "isofield.h"

class IsoExtract : public QObject
{
    Q_OBJECT
public:
    explicit IsoExtract(QObject *parent = 0);
    IsoExtract(const QString& appPath);
    void process(QString message);
    QMap<int, IsoField*> isoMessage;
    QString msgType;

private:
    void readConfig();
    void persist(const QDomElement& element);
    void checkActiveBit(const QString& bitMap);
    void saveExtractedBitValue(int bitNo,QString& modifMessage);

    QString msg;
    QString applicationPath;
    QString bitMap;
    QDomDocument domDocument;
    QHash<int, QString> isoconfig;

signals:

public slots:

};

#endif // ISOEXTRACT_H
