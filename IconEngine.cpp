#include "IconEngine.h"
#include "definitions.h"
#include "AppSettings.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QIcon>
#include <QMutex>
#include <QPixmap>
#include <QPainter>



namespace SwissalpS { namespace QtNibblers {



IconEngine *IconEngine::pSingelton = 0;


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
		case 200u: // vertical
		case 201u: // horizontal
		case 202u: // corner sw
		case 203u: // corner se
		case 204u: // corner nw
		case 205u: // corner ne
		case 206u: // T north
		case 207u: // T east
		case 208u: // T west
		case 209u: // T south
		case 210u: // cross

			oIcon = this->getCellForBuilder(ubState);

		break;

//		// players
//		case 10u:
//		case 11u:
////		case 12u: return QColor(Qt::green);	break;

//		case 20u:
//		case 21u:
//		case 22u: return QColor(Qt::red); break;

//		case 30u:
//		case 31u:
//		case 32u: return QColor(Qt::blue); break;

//		case 40u:
//		case 41u:
//		case 42u: return QColor(Qt::yellow); break;

//		case 50u:
//		case 51u:
//		case 52u: return QColor(Qt::darkGreen); break;

//		case 60u:
//		case 61u:
//		case 62u: return QColor(Qt::darkRed); break;

//		case 70u:
//		case 71u:
//		case 72u: return QColor(Qt::darkBlue); break;

//		case 80u:
//		case 81u:
//		case 82u: return QColor(Qt::darkYellow); break;

		// teleporter entrances
		case 220: // entrance A
		case 222: // entrance B
		case 224: // entrance C
		case 226: // entrance D
		case 228: // entrance E
		case 230: // entrance F
		case 232: // entrance G
		case 234: // entrance H
		case 236: // entrance I
		case 238: // entrance J

			oIcon = IconEngine::makeTeleporter("");

		break;

		// bonuses
		// apples -> add length and advance level
		case 100u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus1.png"), 0u); break;
		case 101u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus1.png"), 1u); break;
		case 102u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus1.png"), 2u); break;
		case 103u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus1.png"), 3u); break;

		// cherries -> shorten
		case 110u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus2.png"), 0u); break;
		case 111u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus2.png"), 1u); break;
		case 112u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus2.png"), 2u); break;
		case 113u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus2.png"), 3u); break;

		// bananas -> add length
		case 120u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus3.png"), 0u); break;
		case 121u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus3.png"), 1u); break;
		case 122u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus3.png"), 2u); break;
		case 123u: oIcon = IconEngine::makeQuart(QIcon(":/Images/bonus3.png"), 3u); break;

		// hearts -> add life
		case 130u: oIcon = IconEngine::makeQuart(QIcon(":/Images/life.png"), 0u); break;
		case 131u: oIcon = IconEngine::makeQuart(QIcon(":/Images/life.png"), 1u); break;
		case 132u: oIcon = IconEngine::makeQuart(QIcon(":/Images/life.png"), 2u); break;
		case 133u: oIcon = IconEngine::makeQuart(QIcon(":/Images/life.png"), 3u); break;

		// diamonds -> reverse direction
		case 140u: oIcon = IconEngine::makeQuart(QIcon(":/Images/diamond.png"), 0u); break;
		case 141u: oIcon = IconEngine::makeQuart(QIcon(":/Images/diamond.png"), 1u); break;
		case 142u: oIcon = IconEngine::makeQuart(QIcon(":/Images/diamond.png"), 2u); break;
		case 143u: oIcon = IconEngine::makeQuart(QIcon(":/Images/diamond.png"), 3u); break;

		// most common -> empty space
		case 0u: // 0-9 floor tiles
		// spawn points
		case 90u: // headed north
		case 91u: // headed west
		case 92u: // headed south
		case 93u: // headed east
		// teleporter exits
		case 221: // exit A
		case 223: // exit B
		case 225: // exit C
		case 227: // exit D
		case 229: // exit E
		case 231: // exit F
		case 233: // exit G
		case 235: // exit H
		case 237: // exit I
		case 239: // exit J
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
		case 0u:
		case 1u:
		case 2u:
		case 3u:
		case 4u:
		case 5u:
		case 6u:
		case 7u:
		case 8u:
		case 9u:
			oIcon = IconEngine::makeFloor(); break;

		// spawn points
		case 90u: // headed north
			oIcon = QIcon(":/Images/arrow.png"); break;
		case 91u: // headed west
			oIcon = QIcon(":/Images/arrow.png").pixmap(32, 32).transformed(QTransform().rotate(-90.0)); break;
		case 92u: // headed south
			oIcon = QIcon(":/Images/arrow.png").pixmap(32, 32).transformed(QTransform().rotate(180.0)); break;
		case 93u: // headed east
			oIcon = QIcon(":/Images/arrow.png").pixmap(64, 64).transformed(QTransform().rotate(90.0)); break;


		// also common -> walls
		case 200u: // vertical
			oIcon = QIcon(":/Images/wall_vertical.svg"); break;
		case 201u: // horizontal
			oIcon = QIcon(":/Images/wall_horizontal.svg"); break;
		case 202u: // corner sw
			oIcon = QIcon(":/Images/wall_corner_sw.svg"); break;
		case 203u: // corner se
			oIcon = QIcon(":/Images/wall_corner_se.svg"); break;
		case 204u: // corner nw
			oIcon = QIcon(":/Images/wall_corner_nw.svg"); break;
		case 205u: // corner ne
			oIcon = QIcon(":/Images/wall_corner_ne.svg"); break;
		case 206u: // T north
			oIcon = QIcon(":/Images/wall_T_north.svg"); break;
		case 207u: // T east
			oIcon = QIcon(":/Images/wall_T_east.svg"); break;
		case 208u: // T west
			oIcon = QIcon(":/Images/wall_T_west.svg"); break;
		case 209u: // T south
			oIcon = QIcon(":/Images/wall_T_south.svg"); break;
		case 210u: // cross
			oIcon = QIcon(":/Images/wall_cross.svg"); break;

		// teleporters
		case 220: // entrance A
			oIcon = IconEngine::makeTeleporter("A"); break;
		case 222: // entrance B
			oIcon = IconEngine::makeTeleporter("B"); break;
		case 224: // entrance C
			oIcon = IconEngine::makeTeleporter("C"); break;
		case 226: // entrance D
			oIcon = IconEngine::makeTeleporter("D"); break;
		case 228: // entrance E
			oIcon = IconEngine::makeTeleporter("E"); break;
		case 230: // entrance F
			oIcon = IconEngine::makeTeleporter("F"); break;
		case 232: // entrance G
			oIcon = IconEngine::makeTeleporter("G"); break;
		case 234: // entrance H
			oIcon = IconEngine::makeTeleporter("H"); break;
		case 236: // entrance I
			oIcon = IconEngine::makeTeleporter("I"); break;
		case 238: // entrance J
			oIcon = IconEngine::makeTeleporter("J"); break;

		case 221: // exit A
			oIcon = IconEngine::makeTeleporter("a"); break;
		case 223: // exit B
			oIcon = IconEngine::makeTeleporter("b"); break;
		case 225: // exit C
			oIcon = IconEngine::makeTeleporter("c"); break;
		case 227: // exit D
			oIcon = IconEngine::makeTeleporter("d"); break;
		case 229: // exit E
			oIcon = IconEngine::makeTeleporter("e"); break;
		case 231: // exit F
			oIcon = IconEngine::makeTeleporter("f"); break;
		case 233: // exit G
			oIcon = IconEngine::makeTeleporter("g"); break;
		case 235: // exit H
			oIcon = IconEngine::makeTeleporter("h"); break;
		case 237: // exit I
			oIcon = IconEngine::makeTeleporter("i"); break;
		case 239: // exit J
			oIcon = IconEngine::makeTeleporter("j"); break;

		// players
		case 10u: // head
		case 11u: // tail
		case 12u: // mid-sections

		case 20u:
		case 21u:
		case 22u:

		case 30u:
		case 31u:
		case 32u:

		case 40u:
		case 41u:
		case 42u:

		case 50u:
		case 51u:
		case 52u:

		case 60u:
		case 61u:
		case 62u:

		case 70u:
		case 71u:
		case 72u:

		case 80u:
		case 81u:
		case 82u:

		// bonuses
		// apples -> add length and advance level
		case 100u:
		case 101u:
		case 102u:
		case 103u:
			// cherries -> shorten
		case 110u:
		case 111u:
		case 112u:
		case 113u:
			// bananas -> add length
		case 120u:
		case 121u:
		case 122u:
		case 123u:
			// hearts -> add life
		case 130u:
		case 131u:
		case 132u:
		case 133u:
			// diamonds -> reverse direction
		case 140u:
		case 141u:
		case 142u:
		case 143u:

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
	for (ubState = 200u; ubState < 211u; ubState++) aCOIs.append(ubState);

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

	if (0u == ubQuart) {
		// nw
		oP.drawPixmap(oRect, oIcon.pixmap(oSize), QRect(0, 0, iW, iH));

	} else if (1u == ubQuart) {
		// ne
		oP.drawPixmap(oRect, oIcon.pixmap(oSize), QRect(iW, 0, iW, iH));

	} else if (2u == ubQuart) {
		// se
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
	for (ubCount = 220u; ubCount < 240u; ubCount += 2u)
		aStates.append(ubCount);

	return aStates;

} // statesTeleporterEntrances


// static
QVector<quint8> IconEngine::statesTeleporterExits() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = 221u; ubCount < 240u; ubCount += 2u)
		aStates.append(ubCount);

	return aStates;

} // statesTeleporterExits


// static
QVector<quint8> IconEngine::statesFloors() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = 0u; ubCount < 10u; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesFloors


// static
QVector<quint8> IconEngine::statesPickups() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = 100u; ubCount < 150u; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesPickups


// static
QVector<quint8> IconEngine::statesSnake(const quint8 ubSnake) {

	quint8 ubState = 10u + (10u * ubSnake);
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

	quint8 ubState = 10u;
	quint8 ubLast = 89u;

	for (; ubState <= ubLast; ++ubState)
		aStates.append(ubState);

	return aStates;

} // statesSnakes


// static
QVector<quint8> IconEngine::statesSpawns() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = 90u; ubCount < 94u; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesSpawns


// static
QVector<quint8> IconEngine::statesWalls() {

	static QVector<quint8> aStates;

	if (!aStates.empty()) return aStates;

	quint8 ubCount;
	for (ubCount = 200u; ubCount < 211u; ++ubCount)
		aStates.append(ubCount);

	return aStates;

} // statesWalls



}	} // namespace SwissalpS::QtNibblers

