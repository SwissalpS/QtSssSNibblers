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
#ifndef ICONENGINE_H
#define ICONENGINE_H

#include <QObject>
#include <QIcon>



namespace SwissalpS { namespace QtNibblers {



class IconEngine : public QObject {

	Q_OBJECT
	Q_DISABLE_COPY(IconEngine)

private:

	static IconEngine *pSingelton;

	// keep this private as we want only one instance
	// read; http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	explicit IconEngine(QObject *pParent = nullptr);

	// hide copy constructor or use Q_DISABLE_COPY(IconEngine)
	//explicit IconEngine(const IconEngine &other);
	// hide assign op
	// we leave just the declarations, so the compiler will warn us
	// if we try to use those two functions by accident
	//IconEngine& operator=(const IconEngine &other);

protected:
	// caches
	QHash<quint8, QIcon> hCells;
	QHash<quint8, QIcon> hCellsForBuilder;
	QHash<quint8, QIcon> hLevels;

	virtual QIcon getCell(const quint8 ubState);
	virtual QIcon getCellForBuilder(const quint8 ubState);
	virtual QIcon getLevel(const quint8 ubLevel);

public:
	static QIcon cell(const quint8 ubState, const bool bForBuilder = false);
	// destroy singelton
	static void drop();
	static QIcon level(const quint8 ubLevel);
	static QIcon makeFloor();
	static QIcon makeQuart(QIcon oIcon, const quint8 ubQuart);
	static QIcon makeTeleporter(const QString sChar);
	// public access to singelton instance
	static IconEngine *pIconEngine();
	virtual void removeCacheOfLevel(const quint8 ubLevel);

	static QVector<quint8> statesFloors();
	static QVector<quint8> statesFloorsWet();
	static QVector<quint8> statesPickups();
	static QVector<quint8> statesSnake(const quint8 ubSnake);
	static QVector<quint8> statesSnakes();
	static QVector<quint8> statesSpawns();
	static QVector<quint8> statesTeleporterEntrances();
	static QVector<quint8> statesTeleporterExits();
	static QVector<quint8> statesWalls();

	virtual ~IconEngine();

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("IconEngine:" + sMessage); }

}; // IconEngine



}	} // namespace SwissalpS::QtNibblers



#endif // ICONENGINE_H
