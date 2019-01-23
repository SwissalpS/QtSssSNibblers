/*
 * QtSssSNibblers: SwissalpS Nibbles written with Qt-Framework
 * Copyright (C) 2018-2019 Luke J. Zimmermann aka SwissalpS <SwissalpS@LukeZimmermann.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "HistoryItem.h"

#include <QDateTime>



namespace SwissalpS { namespace QtNibblers {



HistoryItem::HistoryItem(QObject *pParent) :
	QObject(pParent),
	bFakes(false),
	sName("Null"),
	ubCountAI(0xFFu),
	ubCountHuman(0xFFu),
	ubLevelsDone(0xFFu),
	ubLevelStart(0xFFu),
	ubLivesLost(0xFFu),
	ubSpeedIndex(0xFFu),
	ulScore(0xFFFFFFFFu),
	illTimeStamp(0xFFFFFFFFFFFFFFFF) {

} // construct


HistoryItem::HistoryItem(const QJsonObject oJSON, QObject *pParent) :
	QObject(pParent),
	bFakes(false),
	sName(""),
	ubCountAI(0u),
	ubCountHuman(0u),
	ubLevelsDone(0u),
	ubLevelStart(0u),
	ubLivesLost(0u),
	ubSpeedIndex(0u),
	ulScore(0u),
	illTimeStamp(0) {

	this->bFakes = oJSON.value(sTagFakes).toBool();
	this->sName = oJSON.value(sTagName).toString();
	this->ubCountAI = quint8(oJSON.value(sTagCountAI).toInt());
	this->ubCountHuman = quint8(oJSON.value(sTagCountHuman).toInt());
	this->ubLevelsDone = quint8(oJSON.value(sTagLevelsDone).toInt());
	this->ubLevelStart = quint8(oJSON.value(sTagLevelStart).toInt());
	this->ubLivesLost = quint8(oJSON.value(sTagLivesLost).toInt());
	this->ubSpeedIndex = quint8(oJSON.value(sTagSpeedIndex).toInt());
	this->ulScore = quint32(oJSON.value(sTagScore).toInt());
	this->illTimeStamp = qint64(oJSON.value(sTagTimeStamp).toDouble());

} // construct


HistoryItem::HistoryItem(const bool bFakes, const QString sName,
						 const quint8 ubCountAI, const quint8 ubCountHuman,
						 const quint8 ubLevelsDone, const quint8 ubLevelStart,
						 const quint8 ubLivesLost, const quint8 ubSpeedIndex,
						 const quint32 ulScore, const qint64 illTimeStamp,
						 QObject *pParent) :
	QObject(pParent),
	bFakes(bFakes),
	sName(sName),
	ubCountAI(ubCountAI),
	ubCountHuman(ubCountHuman),
	ubLevelsDone(ubLevelsDone),
	ubLevelStart(ubLevelStart),
	ubLivesLost(ubLivesLost),
	ubSpeedIndex(ubSpeedIndex),
	ulScore(ulScore),
	illTimeStamp(illTimeStamp) {

	if (0 == illTimeStamp) this->illTimeStamp = QDateTime::currentSecsSinceEpoch();

} // construct


HistoryItem::~HistoryItem() {

} // dealloc


QJsonObject HistoryItem::toJSON() const {

	QJsonObject oOut;
	oOut.insert(sTagCountAI, this->ubCountAI);
	oOut.insert(sTagCountHuman, this->ubCountHuman);
	oOut.insert(sTagFakes, this->bFakes);
	oOut.insert(sTagLevelsDone, this->ubLevelsDone);
	oOut.insert(sTagLevelStart, this->ubLevelStart);
	oOut.insert(sTagLivesLost, this->ubLivesLost);
	oOut.insert(sTagName, this->sName);
	oOut.insert(sTagScore, qint32(this->ulScore));
	oOut.insert(sTagSpeedIndex, this->ubSpeedIndex);
	oOut.insert(sTagTimeStamp, qint64(this->illTimeStamp));

	return oOut;

} // toJSON



}	} // namespace SwissalpS::QtNibblers
