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
#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QVector>
#include "Lingo.h"



namespace SwissalpS { namespace QtNibblers {



class Map : public QObject {

	Q_OBJECT

private:

protected:
	quint8 ubTotalColumns;
	quint8 ubTotalRows;
	QVector<QVector<quint8>> aaubRows;

public:
	explicit Map(QObject *pParent = nullptr);
	explicit Map(const quint8 ubColumns, const quint8 ubRows, QObject *pParent = nullptr);
	virtual ~Map();

	inline bool isNull() { return (0 == this->ubTotalColumns) || (0 == this->ubTotalRows); }
	virtual void fillAll(const quint8 ubState = L::FloorClean);
	virtual QPixmap pixmap(const bool bSimple = true) const;
	virtual void setTile(const quint8 ubColumn, const quint8 ubRow, quint8 ubState);
	inline virtual void setTile(const QPoint oPoint, quint8 ubState) {
		this->setTile(quint8(oPoint.x()), quint8(oPoint.y()), ubState); }

	virtual quint8 tile(const quint8 ubColumn, const quint8 ubRow) const;
	inline virtual quint8 tile(const QPoint oPoint) const {
		return this->tile(quint8(oPoint.x()), quint8(oPoint.y())); }

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Map:" + sMessage); }

}; // Map



}	} // namespace SwissalpS::QtNibblers



#endif // MAP_H
