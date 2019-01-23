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
#include "IconEngine.h"
#include "definitions.h"
#include "AppSettings.h"
#include "Lingo.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QIcon>
#include <QMutex>
#include <QPixmap>
#include <QPainter>


/* states 0-255 in a nutshell
 *
 * 0: empty floor tile
 * 1..9: other floor tiles (such as slime marks or RIPs)
 * 10..19: Player 1
 * 20..29: Player 2
 * 30..39: Player 3
 * 40..49: Player 4
 * 50..59: Player 5
 * 60..69: Player 6
 * 70..79: Player 7
 * 80..89: Player 8
 * 90: Spawn Point headed North
 * 91: Spawn Point headed West
 * 92: Spawn Point headed South
 * 93: Spawn Point headed East
 * 94..99: Reserved tombstones?
 * 100..103: Bonus Apples
 * 104..109: Reserved decaying Bonus
 * 110..113: Bonus Cherries
 * 114..119: Reserved decaying Bonus
 * 120..123: Bonus Bananas
 * 124..129: Reserved decaying Bonus
 * 130..133: Bonus Hearts
 * 134..139: Reserved decaying Bonus
 * 140..143: Bonus Diamonds
 * 144..149: Reserved decaying Bonus
 * 150..199: Reserved for other bonuses
 * 200u: Wall Vertical
 * 201u: Wall Horizontal
 * 202u: Wall Corner SW
 * 203u: Wall Corner SE
 * 204u: Wall Corner NW
 * 205u: Wall Corner NE
 * 206u: Wall T North (upside down T)
 * 207u: Wall T East
 * 208u: Wall T West
 * 209u: Wall T South
 * 210u: Wall Cross
 * 211..219: Reserved for other walls
 *
 *
 *
 *
 *
 *
 *
 *
 * 220: Teleporter Entrance A
 * 221: Teleporter Exit A
 * 222: Teleporter Entrance B
 * 223: Teleporter Exit B
 * 224: Teleporter Entrance C
 * 225: Teleporter Exit C
 * 226: Teleporter Entrance D
 * 227: Teleporter Exit D
 * 228: Teleporter Entrance E
 * 229: Teleporter Exit E
 * 230: Teleporter Entrance F
 * 221: Teleporter Exit F
 * 232: Teleporter Entrance G
 * 223: Teleporter Exit G
 * 234: Teleporter Entrance H
 * 225: Teleporter Exit H
 * 236: Teleporter Entrance I
 * 227: Teleporter Exit I
 * 238: Teleporter Entrance J
 * 229: Teleporter Exit J
 * 240..249: Reserved for some future do-dad
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * 250..254: Reserved for some extra stuff
 *
 *
 *
 *
 * 255: Null Cell
 */


namespace SwissalpS { namespace QtNibblers {



IconEngine *IconEngine::pSingelton = nullptr;


IconEngine::IconEngine(QObject *pParent) :
	QObject(pParent) {

	// init icons caches
	this->hCells.clear();
	this->hCellsForBuilder.clear();
	this->hLevels.clear();

} // construct


IconEngine::~IconEngine() {

} // dealloc


// static
IconEngine *IconEngine::pIconEngine() {

	///
	/// \brief oMutex
	///
	static QMutex oMutex;

	///////////////////////////////
	/* NOTE from: http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	 * Be aware that you should check for a std::bad_alloc exception
	 * to unlock the mutex in case new fails. We are also using
	 * double-checked locking as locking and unlocking a mutex
	 * is expensive and would significantly slow down a call to
	 *  Singleton::instance().
	 */
	if (!IconEngine::pSingelton) {

		oMutex.lock();

		if (!pSingelton) {

			pSingelton = new IconEngine();

		} // if first call

		oMutex.unlock();

	} // if first call

	return pSingelton;

} // singelton access


// static
void IconEngine::drop() {

	static QMutex oMutex;

	oMutex.lock();

	delete pSingelton;
	pSingelton = 0;

	oMutex.unlock();

} // drop singelton


// static
QIcon IconEngine::cell(const quint8 ubState, const bool bForBuilder) {

	static IconEngine *pIE = IconEngine::pIconEngine();

	if (bForBuilder) {

		return pIE->getCellForBuilder(ubState);

	} else {

		return pIE->getCell(ubState);

	} // if for builder or not

} // cell


QIcon IconEngine::getCell(const quint8 ubState) {

	if (this->hCells.contains(ubState)) return this->hCells.value(ubState);

	// need to create cache for this one
	QIcon oIcon;

	switch (ubState) {

		// also common -> walls
		case L::WallVertical: // vertical
		case L::WallHorizontal: // horizontal
		case L::WallCornerSW: // corner sw
		case L::WallCornerSE: // corner se
		case L::WallCornerNW: // corner nw
		case L::WallCornerNE: // corner ne
		case L::WallTnorth: // T north
		case L::WallTeast: // T east
		case L::WallTwest: // T west
		case L::WallTsouth: // T south
		case L::WallCross: // cross

			oIcon = this->getCellForBuilder(ubState);

		break;

		// teleporter entrances
		case L::TeleporterInA: // entrance A
		case L::TeleporterInB: // entrance B
		case L::TeleporterInC: // entrance C
		case L::TeleporterInD: // entrance D
		case L::TeleporterInE: // entrance E
		case L::TeleporterInF: // entrance F
		case L::TeleporterInG: // entrance G
		case L::TeleporterInH: // entrance H
		case L::TeleporterInI: // entrance I
		case L::TeleporterInJ: // entrance J

			oIcon = IconEngine::makeTeleporter("");

		break;

		// bonuses
		// apples -> add length and advance level
		case L::BonusAppleNW: oIcon = QIcon(":/Images/bonus1_NW.png"); break;
		case L::BonusAppleNE: oIcon = QIcon(":/Images/bonus1_NE.png"); break;
		case L::BonusAppleSE: oIcon = QIcon(":/Images/bonus1_SE.png"); break;
		case L::BonusAppleSW: oIcon = QIcon(":/Images/bonus1_SW.png"); break;

		// cherries -> shorten
		case L::BonusCherryNW: oIcon = QIcon(":/Images/bonus2_NW.png"); break;
		case L::BonusCherryNE: oIcon = QIcon(":/Images/bonus2_NE.png"); break;
		case L::BonusCherrySE: oIcon = QIcon(":/Images/bonus2_SE.png"); break;
		case L::BonusCherrySW: oIcon = QIcon(":/Images/bonus2_SW.png"); break;

		// bananas -> add length
		case L::BonusBananaNW: oIcon = QIcon(":/Images/bonus3_NW.png"); break;
		case L::BonusBananaNE: oIcon = QIcon(":/Images/bonus3_NE.png"); break;
		case L::BonusBananaSE: oIcon = QIcon(":/Images/bonus3_SE.png"); break;
		case L::BonusBananaSW: oIcon = QIcon(":/Images/bonus3_SW.png"); break;

		// hearts -> add life
		case L::BonusHeartNW: oIcon = QIcon(":/Images/life_NW.png"); break;
		case L::BonusHeartNE: oIcon = QIcon(":/Images/life_NE.png"); break;
		case L::BonusHeartSE: oIcon = QIcon(":/Images/life_SE.png"); break;
		case L::BonusHeartSW: oIcon = QIcon(":/Images/life_SW.png"); break;

		// diamonds -> reverse direction
		case L::BonusDiamondNW: oIcon = QIcon(":/Images/diamond_NW.png"); break;
		case L::BonusDiamondNE: oIcon = QIcon(":/Images/diamond_NE.png"); break;
		case L::BonusDiamondSE: oIcon = QIcon(":/Images/diamond_SE.png"); break;
		case L::BonusDiamondSW: oIcon = QIcon(":/Images/diamond_SW.png"); break;

		// most common -> empty space
			// 0-9 floor tiles
		// spawn points
		// teleporter exits
		default:
			oIcon = QIcon();
		break;

	} // switch ubState

	this->hCells.insert(ubState, oIcon);

	return this->hCells.value(ubState);

} // getCell


QIcon IconEngine::getCellForBuilder(const quint8 ubState) {

	if (this->hCellsForBuilder.contains(ubState)) return this->hCellsForBuilder.value(ubState);

	// need to create cache for this one
	QIcon oIcon;

	switch (ubState) {

		// most common -> empty space
		case L::FloorClean:
		case L::FloorWet1:
		case L::FloorWet2:
		case L::FloorWet3:
		case L::FloorWet4:
		case L::FloorWet5:
		case L::FloorWet6:
		case L::FloorWet7:
		case L::FloorWet8:
		case L::FloorWet9:
			oIcon = IconEngine::makeFloor();
		break;

		// spawn points
		case L::SpawnHeadingNorth: oIcon = QIcon(":/Images/arrow.png"); break;
		case L::SpawnHeadingWest: oIcon = QIcon(":/Images/arrow.png").pixmap(32, 32).transformed(QTransform().rotate(-90.0)); break;
		case L::SpawnHeadingSouth: oIcon = QIcon(":/Images/arrow.png").pixmap(32, 32).transformed(QTransform().rotate(180.0)); break;
		case L::SpawnHeadingEast: oIcon = QIcon(":/Images/arrow.png").pixmap(64, 64).transformed(QTransform().rotate(90.0));
		break;

		// also common -> walls
		case L::WallVertical: oIcon = QIcon(":/Images/wall_vertical.svg"); break;
		case L::WallHorizontal: oIcon = QIcon(":/Images/wall_horizontal.svg"); break;
		case L::WallCornerSW: oIcon = QIcon(":/Images/wall_corner_sw.svg"); break;
		case L::WallCornerSE: oIcon = QIcon(":/Images/wall_corner_se.svg"); break;
		case L::WallCornerNW: oIcon = QIcon(":/Images/wall_corner_nw.svg"); break;
		case L::WallCornerNE: oIcon = QIcon(":/Images/wall_corner_ne.svg"); break;
		case L::WallTnorth: oIcon = QIcon(":/Images/wall_T_north.svg"); break;
		case L::WallTeast: oIcon = QIcon(":/Images/wall_T_east.svg"); break;
		case L::WallTwest: oIcon = QIcon(":/Images/wall_T_west.svg"); break;
		case L::WallTsouth: oIcon = QIcon(":/Images/wall_T_south.svg"); break;
		case L::WallCross: oIcon = QIcon(":/Images/wall_cross.svg");
		break;

		// teleporters
		case L::TeleporterInA: oIcon = IconEngine::makeTeleporter("A"); break;
		case L::TeleporterInB: oIcon = IconEngine::makeTeleporter("B"); break;
		case L::TeleporterInC: oIcon = IconEngine::makeTeleporter("C"); break;
		case L::TeleporterInD: oIcon = IconEngine::makeTeleporter("D"); break;
		case L::TeleporterInE: oIcon = IconEngine::makeTeleporter("E"); break;
		case L::TeleporterInF: oIcon = IconEngine::makeTeleporter("F"); break;
		case L::TeleporterInG: oIcon = IconEngine::makeTeleporter("G"); break;
		case L::TeleporterInH: oIcon = IconEngine::makeTeleporter("H"); break;
		case L::TeleporterInI: oIcon = IconEngine::makeTeleporter("I"); break;
		case L::TeleporterInJ: oIcon = IconEngine::makeTeleporter("J"); break;

		case L::TeleporterOutA: oIcon = IconEngine::makeTeleporter("a"); break;
		case L::TeleporterOutB: oIcon = IconEngine::makeTeleporter("b"); break;
		case L::TeleporterOutC: oIcon = IconEngine::makeTeleporter("c"); break;
		case L::TeleporterOutD: oIcon = IconEngine::makeTeleporter("d"); break;
		case L::TeleporterOutE: oIcon = IconEngine::makeTeleporter("e"); break;
		case L::TeleporterOutF: oIcon = IconEngine::makeTeleporter("f"); break;
		case L::TeleporterOutG: oIcon = IconEngine::makeTeleporter("g"); break;
		case L::TeleporterOutH: oIcon = IconEngine::makeTeleporter("h"); break;
		case L::TeleporterOutI: oIcon = IconEngine::makeTeleporter("i"); break;
		case L::TeleporterOutJ: oIcon = IconEngine::makeTeleporter("j"); break;

		// players
		// bonuses
		// apples -> add length and advance level
			// cherries -> shorten
			// bananas -> add length
			// hearts -> add life
			// diamonds -> reverse direction

		default: oIcon = this->getCell(ubState); break;

	} // switch ubState

	this->hCellsForBuilder.insert(ubState, oIcon);

	return this->hCellsForBuilder.value(ubState);

} // getCellForBuilder


QIcon IconEngine::getLevel(const quint8 ubLevel) {

	if (this->hLevels.contains(ubLevel)) return this->hLevels.value(ubLevel);

	// need to create cache for this one
	QPixmap oPixmap(SssS_Nibblers_Surface_Width, SssS_Nibblers_Surface_Height);
	QPainter oP(&oPixmap);

	oP.setBackground(Qt::black);
	oP.fillRect(oPixmap.rect(), Qt::black);
	oP.setPen(Qt::white);

	QString sPath = AppSettings::pAppSettings()->getDataPathLevelFile(ubLevel);

	QFileInfo oFI = QFileInfo(sPath);
	if (!(oFI.exists() && oFI.isFile())) {

		this->onDebugMessage("KO: failed to find: " + sPath);
		return QIcon(oPixmap);

	} // if file does not exist

	QFile oFile(sPath);
	if (!oFile.open(QFile::ReadOnly)) {

		this->onDebugMessage("KO: failed to open: " + sPath);
		return QIcon(oPixmap);

	} // if failed to open

	QByteArray aFile = oFile.readAll();
	oFile.close();

	if ((SssS_Nibblers_Surface_Height * SssS_Nibblers_Surface_Width)
			> aFile.length()) {

		this->onDebugMessage("invalid length (too short)");
		return QIcon(oPixmap);

	} // if invalid length

	// prepare vector of states we react to
	quint8 ubState;
	QVector<quint8> aCOIs;

	for (ubState = L::WallVertical; ubState < 211u; ubState++) aCOIs.append(ubState);

	int iPos = 0u;
	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			ubState = quint8(aFile.at(iPos));

			if (aCOIs.contains(ubState)) oP.drawPoint(ubColumns, ubRows);

			iPos++;

		} // loop columns

	} // loop rows

	this->hLevels.insert(ubLevel, QIcon(oPixmap));

	return this->hLevels.value(ubLevel);

} // getLevel


// static
QIcon IconEngine::level(const quint8 ubLevel) {

	static IconEngine *pIE = IconEngine::pIconEngine();

	return pIE->getLevel(ubLevel);

} // level


// static
QIcon IconEngine::makeFloor() {

	QPixmap oPixmap(64, 64);
	QPainter oP(&oPixmap);
	QRect oRect = oPixmap.rect();

	oP.setBackground(Qt::black);

	oP.setBrush(Qt::lightGray);
	oP.setPen(Qt::darkGray);
	oP.drawRoundedRect(oRect, oRect.width() * 0.32,
					   oRect.height() * 0.32);

	return QIcon(oPixmap);

} // makeFloor


// static
QIcon IconEngine::makeQuart(QIcon oIcon, const quint8 ubQuart) {

	QPixmap oPixmap(64, 64);
	QPainter oP(&oPixmap);
	QRect oRect = oPixmap.rect();
	QSize oSize = oIcon.actualSize(QSize(64, 64));
	int iW = oSize.width() * 0.5;
	int iH = oSize.height() * 0.5;

	oP.setBackground(Qt::transparent);

	if (L::NorthWest == ubQuart) {
		oP.drawPixmap(oRect, oIcon.pixmap(oSize), QRect(0, 0, iW, iH));

	} else if (L::NorthEast == ubQuart) {
		oP.drawPixmap(oRect, oIcon.pixmap(oSize), QRect(iW, 0, iW, iH));

	} else if (L::SouthEast == ubQuart) {
		oP.drawPixmap(oRect, oIcon.pixmap(oSize), QRect(iW, iH, iW, iH));

	} else {
		// sw
		oP.drawPixmap(oRect, oIcon.pixmap(oSize), QRect(0, iH, iW, iH));

	} // switch corner

	return QIcon(oPixmap);

} // makeQuart


// static
QIcon IconEngine::makeTeleporter(const QString sChar) {

	QPixmap oPixmap(64, 64);
	QPainter oP(&oPixmap);
	QRect oRect = oPixmap.rect();

	QFont oFont = oP.font();
	oFont.setPixelSize(44);
	oFont.setBold(true);
	oP.setFont(oFont);

	oP.setBackground(Qt::black);

	oP.setBrush(Qt::lightGray);
	oP.drawRect(oRect.adjusted(3, 3, -3, -3));

	oP.setBrush(Qt::blue);
	oP.drawRect(oRect.adjusted(7, 7, -7, -7));

	oP.setBrush(Qt::lightGray);
	oP.drawRect(oRect.adjusted(9, 9, -9, -9));

	oP.drawText(oPixmap.rect().adjusted(10, 10, -10, -10),
				Qt::AlignCenter, sChar);

	return QIcon(oPixmap);

} // makeTeleporter


void IconEngine::removeCacheOfLevel(const quint8 ubLevel) {

	this->hLevels.remove(ubLevel);

} // removeCacheOfLevel


// static
QVector<quint8> IconEngine::statesTeleporterEntrances() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = L::TeleporterInA; ubCount <= L::TeleporterInJ; ubCount += 2u)
		aStates.append(ubCount);

	return aStates;

} // statesTeleporterEntrances


// static
QVector<quint8> IconEngine::statesTeleporterExits() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = L::TeleporterOutA; ubCount <= L::TeleporterOutJ; ubCount += 2u)
		aStates.append(ubCount);

	return aStates;

} // statesTeleporterExits


// static
QVector<quint8> IconEngine::statesFloors() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = L::FloorClean; ubCount <= L::FloorWet9; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesFloors


// static
QVector<quint8> IconEngine::statesFloorsWet() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = L::FloorWet1; ubCount <= L::FloorWet9; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesFloorsWet


// static
QVector<quint8> IconEngine::statesPickups() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = L::BonusApple; ubCount < 150u; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesPickups


// static
QVector<quint8> IconEngine::statesSnake(const quint8 ubSnake) {

	quint8 ubState = L::Player_1_Head + (10u * ubSnake);
	quint8 ubLast = ubState + 9u;
	QVector<quint8> aStates;

	for (; ubState <= ubLast; ++ubState)
		aStates.append(ubState);

	return aStates;

} // statesSnake


// static
QVector<quint8> IconEngine::statesSnakes() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubState = L::Player_1_Head;
	quint8 ubLast = L::Player_8_Tail;

	for (; ubState <= ubLast; ++ubState)
		aStates.append(ubState);

	return aStates;

} // statesSnakes


// static
QVector<quint8> IconEngine::statesSpawns() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = L::SpawnHeadingNorth; ubCount <= L::SpawnHeadingEast; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesSpawns


// static
QVector<quint8> IconEngine::statesWalls() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = L::WallVertical; ubCount <= L::WallCross; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesWalls



}	} // namespace SwissalpS::QtNibblers

