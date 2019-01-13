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

	static QVector<quint8> aubStatesFree;
	if (aubStatesFree.isEmpty())
		aubStatesFree = IconEngine::statesFloors()
						+ IconEngine::statesTeleporterExits()
						+ IconEngine::statesSpawns();

	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	quint8 ubState;
	QPoint oPoint;
	QPoint oPoint2;
	QVector<QPoint> aoPoints;
	aoPoints.clear();
	QVector<QPoint> aoFreeCells;
	aoFreeCells.clear();

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			oPoint = QPoint(ubColumns, ubRows);
			ubState = this->tile(oPoint);
			if (!aubStatesFree.contains(ubState)) continue;

			ubState = this->tile(L::warpPoint(oPoint, L::Right));
			if (!aubStatesFree.contains(ubState)) continue; // TODO: optimize as we have already checked this one

			oPoint2 = L::warpPoint(oPoint, L::Down);
			ubState = this->tile(oPoint2);
			if (!aubStatesFree.contains(ubState)) continue;

			ubState = this->tile(L::warpPoint(oPoint2, L::Right));
			if (!aubStatesFree.contains(ubState)) continue;

			// OK, this one could work
			aoFreeCells.append(oPoint);

		} // loop columns

	} // loop rows

	int iMaxPlusOne = aoFreeCells.length();
	if (0 == iMaxPlusOne) {

		this->onDebugMessage("No Space Found for bonus");

		return aoPoints;

	} // if no free space to put any bonus

	int iMin = 0;
	int iIndex = iMin + (qrand() % (iMaxPlusOne - iMin));

	oPoint = aoFreeCells.at(iIndex);
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
