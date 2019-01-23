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
#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include <QVector>

#include "HistoryItem.h"
#include "PersistantObject.h"



namespace SwissalpS { namespace QtNibblers {



class History : public PersistantObject {

	Q_OBJECT
	Q_DISABLE_COPY(History)

private:

protected:

public:
	const QString sTagEntries = QStringLiteral("aEntries");
	const QString sTagVersion = QStringLiteral("sVersion");

	explicit History(const QString sPath, QObject *pParent = nullptr);
	virtual ~History();

	virtual void clear();
	virtual void clear(QVector<int> aiIndexes);
	virtual QVector<HistoryItem *> items();

public slots:
	virtual void addItem(HistoryItem *pHI);

}; // History



}	} // namespace SwissalpS::QtNibblers



#endif // HISTORY_H
