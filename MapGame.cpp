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
#include "MapGame.h"

#include "definitions.h"
#include "IconEngine.h"

#include <QFileInfo>



namespace SwissalpS { namespace QtNibblers {



MapGame::MapGame(QObject *pParent) :
	Map(pParent),
	eErrorCode(NullObject) {

} // construct(null-object)


MapGame::MapGame(const QByteArray &aFileContents, QObject *pParent) :
	Map(SssS_Nibblers_Surface_Width, SssS_Nibblers_Surface_Height, pParent),
	eErrorCode(NoError) {

	if ((SssS_Nibblers_Surface_Height * SssS_Nibblers_Surface_Width)
			> aFileContents.length()) {

		this->setErrorCode(FileLengthError);
		this->ubTotalColumns = this->ubTotalRows = 0u;

		return;

	} // if invalid length

	QVector<quint8> aStatesSpawns = IconEngine::statesSpawns();
	QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();

	int iPos = 0;
	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	quint8 ubState = L::FloorClean;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			ubState = quint8(aFileContents.at(iPos));

			// keep track of special ones

			if (aStatesSpawns.contains(ubState)) {

				this->aPointsSpawn.append(QPoint(ubColumns, ubRows));

			} else if (aStatesTeleporterEntrances.contains(ubState)) {

				this->hPointsTeleporterIn.insert(ubState, QPoint(ubColumns, ubRows));

			} else if (aStatesTeleporterExits.contains(ubState)) {

				this->hPointsTeleporterOut.insert(ubState, QPoint(ubColumns, ubRows));

			} // if special state we need to keep track of (new state)

			this->setTile(ubColumns, ubRows, ubState);

			iPos++;

		} // loop columns

	} // loop rows

} // construct


MapGame::~MapGame() {

	this->aPointsSpawn.clear();
	this->hPointsTeleporterIn.clear();
	this->hPointsTeleporterOut.clear();

} // dealloc


QVector<QPoint> MapGame::freeSpotForBonus() {

	//this->onDebugMessage("freeSpotForBonus");

	// find 4 adjacent cells that are not occupied

	QPoint oPoint;
	QVector<QPoint> aoPoints;
	static QVector<QPoint> aoFreePoints;

	if (aoFreePoints.isEmpty()) {

		quint8 ubColumns = 0u;
		quint8 ubRows = 0u;

		for (; ubRows < SssS_Nibblers_Surface_Height - 1u; ++ubRows) {

			for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width - 1u; ++ubColumns) {

				// OK, this one could work
				oPoint = QPoint(ubColumns, ubRows);
				if (this->isGoodForBonus(oPoint))
					aoFreePoints.append(oPoint);

			} // loop columns

		} // loop rows

	} // first call

	int iMaxPlusOne = aoFreePoints.length();
	if (0 == iMaxPlusOne) {

		this->onDebugMessage("No Space Found for bonus");

		return aoPoints;

	} // if no free space to put any bonus

	int iIndex;
	int iMin = 0;
	QPoint oPoint2;
	QVector<QPoint> aoTriedPoints;
	bool bOK = false;
	while (!bOK) {

		iIndex = iMin + (qrand() % (iMaxPlusOne - iMin));
		oPoint = aoFreePoints.at(iIndex);

		if (aoTriedPoints.contains(oPoint)) continue;

		if (this->isGoodForBonus(oPoint)) {

			bOK = true;
			continue;

		} // if good found

		aoTriedPoints.append(oPoint);

		if (aoTriedPoints.length() == aoFreePoints.length()) {

			// tried all but could not satisfy

			this->onDebugMessage("No Space Found for bonus");

			return aoPoints;

		} // if nothing found

	} // loop !bOK

	aoPoints.append(oPoint);
	aoPoints.append(L::warpPoint(oPoint, L::Right));
	oPoint2 = L::warpPoint(oPoint, L::Down);
	aoPoints.append(L::warpPoint(oPoint2, L::Right));
	aoPoints.append(oPoint2);

//	this->onDebugMessage("\n" + QString::number(aoPoints.at(0).x())
//						 + ":" + QString::number(aoPoints.at(0).y())
//						 + " " + QString::number(aoPoints.at(1).x())
//						 + ":" + QString::number(aoPoints.at(1).y())
//						 + " " + QString::number(aoPoints.at(2).x())
//						 + ":" + QString::number(aoPoints.at(2).y())
//						 + " " + QString::number(aoPoints.at(3).x())
//						 + ":" + QString::number(aoPoints.at(3).y()));

	return aoPoints;

} // freeSpotForBonus


// check if a bonus can be placed there
bool MapGame::isGoodForBonus(const QPoint oPoint) {

	static QVector<quint8> aubStatesFree;
	if (aubStatesFree.isEmpty())
		aubStatesFree = IconEngine::statesFloors()
						+ IconEngine::statesTeleporterExits()
						+ IconEngine::statesSpawns();
	quint8 ubState;
	QPoint oPoint2;

	ubState = this->tile(oPoint);
	if (!aubStatesFree.contains(ubState)) return false;

	ubState = this->tile(L::warpPoint(oPoint, L::Right));
	if (!aubStatesFree.contains(ubState)) return false;

	oPoint2 = L::warpPoint(oPoint, L::Down);
	ubState = this->tile(oPoint2);
	if (!aubStatesFree.contains(ubState)) return false;

	ubState = this->tile(L::warpPoint(oPoint2, L::Right));
	if (!aubStatesFree.contains(ubState)) return false;

	return true;

} // isGoodForBonus


// static main way to get a map
MapGame *MapGame::loadedMap(const QString sFilePath, QObject *pParent) {

	MapGame *pMap = new MapGame(pParent);

	QFileInfo oFI = QFileInfo(sFilePath);
	if (!(oFI.exists() && oFI.isFile())) {

		pMap->setErrorCode(FileNotFound);

		return pMap;

	} // if file does not exist

	QFile oFile(sFilePath);
	if (!oFile.open(QFile::ReadOnly)) {

		pMap->setErrorCode(FileReadError);

		return pMap;

	} // if failed to open

	QByteArray aFile = oFile.readAll();
	oFile.close();

	if ((SssS_Nibblers_Surface_Height * SssS_Nibblers_Surface_Width)
			> aFile.length()) {

		pMap->setErrorCode(FileLengthError);

		return pMap;

	} // if invalid length

	return new MapGame(aFile, pParent);

} // loadedMap


void MapGame::randomizeSpawnOrder() {

	int iPos;
	QVector<QPoint> aNew;
	while(this->aPointsSpawn.length()) {

		iPos = qrand() % this->aPointsSpawn.length();
		aNew.append(this->aPointsSpawn.takeAt(iPos));

	} // loop all out

	this->aPointsSpawn = aNew;

} // randomizeSpawns


QPoint MapGame::teleporterInForOut(const quint8 ubState) {

	if (ubState) {

		quint8 ubStatePartner = ubState - 1u;
		if (this->hPointsTeleporterIn.contains(ubStatePartner))
		return this->hPointsTeleporterIn.value(ubStatePartner);

	} // if got a value greater than nothing

	return QPoint(0xFFu, 0xFFu);

} // teleporterInForOut


QPoint MapGame::teleporterOutForIn(const quint8 ubState) {

	quint8 ubStatePartner = ubState + 1u;
	if (this->hPointsTeleporterOut.contains(ubStatePartner))
		return this->hPointsTeleporterOut.value(ubStatePartner);

	return QPoint(0xFFu, 0xFFu);

} // teleporterOutForIn



}	} // namespace SwissalpS::QtNibblers
