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
#include "History.h"

#include <QJsonArray>

namespace SwissalpS { namespace QtNibblers {



History::History(const QString sPath, QObject *pParent) :
	PersistantObject(sPath, pParent) {

	if (!this->isOK()) return;

} // construct


History::~History() {

} // dealloc


void History::addItem(HistoryItem *pHI) {

	QJsonObject oJo = this->toJSONobject();
	QJsonArray oEntries = oJo.value(sTagEntries).toArray();

	oEntries.append(pHI->toJSON());

	oJo.insert(sTagEntries, oEntries);
	this->oJdoc.setObject(oJo);

	this->bChanged = true;
	this->save();

} // addItem


void History::clear() {

	QJsonArray oEntries;
	QJsonObject oJo = this->toJSONobject();

	oJo.insert(sTagEntries, oEntries);
	this->oJdoc.setObject(oJo);

	this->bChanged = true;
	this->save();

} // clear


void History::clear(QVector<int> aiIndexes) {

	if (0 == aiIndexes.length()) return;

	int iIndex;
	int iHighest;
	int iHighestIndex;
	QVector<int> aiIndexesSorted;
	while (aiIndexes.length()) {

		iHighest = -1;
		iHighestIndex = -1;
		for (int i = 0; i < aiIndexes.length(); ++i) {

			iIndex = aiIndexes.at(i);
			if (iIndex > iHighest) {

				iHighest = iIndex;
				iHighestIndex = i;

			} // if found higher index

		} // loop

		if (0 <= iHighest) {

			// move to sorted array
			aiIndexesSorted.append(iHighest);
			aiIndexes.removeAt(iHighestIndex);

		} else {

			// can this happen?
			// could not find any???
			this->onDebugMessage("couldn't find another highest index "
								 + QString::number(aiIndexes.length()));
			break;

		} // if got highest or not

	} // loop aiIndexes.length()

	QJsonObject oJo = this->toJSONobject();
	QJsonArray oEntries = oJo.value(sTagEntries).toArray();

	for (int i = 0; i < aiIndexesSorted.length(); ++i)
		oEntries.removeAt(aiIndexesSorted.at(i));

	oJo.insert(sTagEntries, oEntries);
	this->oJdoc.setObject(oJo);

	this->bChanged = true;
	this->save();

	aiIndexesSorted.clear();

} // clear(QVector<int>)


QVector<HistoryItem *> History::items() {

	QVector<HistoryItem *>apOut;
	QJsonArray oEntries = this->toJSONobject().value(sTagEntries).toArray();

	for (int i = 0; i < oEntries.count(); ++i) {

		apOut.append(new HistoryItem(oEntries.at(i).toObject(), this));

	} // loop

	return apOut;

} // items



}	} // namespace SwissalpS::QtNibblers
