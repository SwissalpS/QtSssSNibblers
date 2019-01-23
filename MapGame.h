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
#ifndef MAPGAME_H
#define MAPGAME_H

#include "Map.h"
#include <QHash>
#include <QObject>
#include <QVector>



namespace SwissalpS { namespace QtNibblers {



class MapGame : public SwissalpS::QtNibblers::Map {

	Q_OBJECT

public:

	enum ErrorCode {
		NoError = 0u,
		NullObject = 1u,
		FileNotFound = 2u,
		FileReadError = 4u,
		FileLengthError = 8u,
	};
	Q_ENUM(ErrorCode)

private:

protected:
	ErrorCode eErrorCode;

	QVector<QPoint> aPointsSpawn;
	QHash<quint8, QPoint> hPointsTeleporterIn;
	QHash<quint8, QPoint> hPointsTeleporterOut;

	explicit MapGame(QObject *pParent = nullptr);
	explicit MapGame(const QByteArray &aFileContents, QObject *pParent = nullptr);

	virtual bool isGoodForBonus(const QPoint oPoint);

public:
	// use this to get a map. If invalid will return a null-map
	static MapGame *loadedMap(const QString sFilePath, QObject *pParent = nullptr);
	virtual ~MapGame();

	inline virtual ErrorCode errorCode() const { return this->eErrorCode; }
	virtual QVector<QPoint> freeSpotForBonus();
	inline virtual void setErrorCode(const ErrorCode eCode) { this->eErrorCode = eCode; }
	inline virtual QVector<QPoint> spawnPoints() { return this->aPointsSpawn; }
	virtual QPoint teleporterInForOut(const quint8 ubState);
	virtual QPoint teleporterOutForIn(const quint8 ubState);

public slots:
	void randomizeSpawnOrder();

}; // MapGame



}	} // namespace SwissalpS::QtNibblers



#endif // MAPGAME_H
