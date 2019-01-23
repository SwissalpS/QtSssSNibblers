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
#include "SurfaceBuilder.h"
#include "ui_SurfaceBuilder.h"

#include "definitions.h"
#include "IconEngine.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>



namespace SwissalpS { namespace QtNibblers {



SurfaceBuilder::SurfaceBuilder(QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::SurfaceBuilder),
	pAS(AppSettings::pAppSettings()),
	pDialogLoad(nullptr),
	pDialogSave(nullptr),
	ubCurrentLevel(0xFFu),
	ubLastColumn(0xFFu),
	ubLastRow(0xFFu) {

	this->pUi->setupUi(this);

	this->aopRows.clear();
	this->apSpawnPoints.clear();
	this->hpTeleporterEntrances.clear();
	this->hpTeleporterExits.clear();

	this->ubCurrentLevel = this->pAS->get(AppSettings::sSettingBuilderLastLevel).toUInt();

} // construct


SurfaceBuilder::~SurfaceBuilder() {

	this->aopRows.clear();
	this->apSpawnPoints.clear();
	this->hpTeleporterEntrances.clear();
	this->hpTeleporterExits.clear();

	delete this->pUi;

} // dealloc


void SurfaceBuilder::changeEvent(QEvent *pEvent) {

	QFrame::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		default:
		break;

	} // switch

} // changeEvent


void SurfaceBuilder::clearSurface() {

	this->apSpawnPoints.clear();
	this->hpTeleporterEntrances.clear();
	this->hpTeleporterExits.clear();

	quint8 ubRows = this->aopRows.count();
	quint8 ubColumns = this->aopRows.first().count();
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aopRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pCell = aRow.at(ubX);
			this->setCellState(pCell, L::FloorClean, false);

		} // loop columns

	} // loop rows

	this->update();

} // clearSurface


void SurfaceBuilder::clearSurfaceOf(const quint8 ubState) {

	QVector<quint8> aStates;
	aStates.append(ubState);

	this->clearSurfaceOf(aStates);

} // clearSurfaceOf(single)


void SurfaceBuilder::clearSurfaceOf(const QVector<quint8> aStates) {

	quint8 ubRows = this->aopRows.count();
	quint8 ubColumns = this->aopRows.first().count();
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aopRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pCell = aRow.at(ubX);

			if (aStates.contains(pCell->getState())) {

				this->setCellState(pCell, L::FloorClean, false);

			} // if one to clear

		} // loop columns

	} // loop rows

	this->update();

} // clearSurfaceOf(vector)


QIcon SurfaceBuilder::currentBrushIcon() const {

	return IconEngine::cell(this->currentBrushState(), true);

} // currentBrushIcon


quint8 SurfaceBuilder::currentBrushState() const {

	QList<quint8> aExitKeys = this->hpTeleporterExits.keys();
	QList<quint8> aEntranceKeys = this->hpTeleporterEntrances.keys();

	static QVector<quint8> aEntrances = IconEngine::statesTeleporterEntrances();
	static QVector<quint8> aExits = IconEngine::statesTeleporterExits();
	quint8 ubState;

	int iCurrent = this->pUi->selectTool->currentIndex();

	switch (iCurrent) {

		 // walls
		case 1: return L::WallCornerNE;
		case 2: return L::WallCornerNW;
		case 3: return L::WallCornerSE;
		case 4: return L::WallCornerSW;
		case 5: return L::WallCross;
		case 6: return L::WallHorizontal;
			// wall Ts
		case 7: return L::WallTeast;
		case 8: return L::WallTnorth;
		case 9: return L::WallTsouth;
		case 10: return L::WallTwest;
		case 11: return L::WallVertical;
		// spawn points
		case 12:
		case 13:
		case 14:
		case 15:

			if (SssS_Nibblers_Max_Players <= this->apSpawnPoints.length()) {

				Q_EMIT this->statusMessage(tr("Maximum amount of spawn-points reached. Click on one to remove it."));

				return L::FloorClean;

			} // if already full

			Q_EMIT this->statusMessage(tr("Click on a cell to set spawn-point."));

			return quint8(iCurrent) - 12 + L::SpawnHeadingNorth;

		// Teleporter
		case 16:

			if (aEntranceKeys.length() > aExitKeys.length()) {
				// placing an exit

				for (int iPos = 0; iPos < aExits.length(); ++iPos) {

					ubState = aExits.at(iPos);
					if (aExitKeys.contains(ubState)) continue;

					Q_EMIT this->statusMessage(tr("Click on a cell to place teleporter exit."));
					return ubState;

				} // loop

			} else {

				// placing an entrance or full?
				if (SssS_Nibblers_Max_Teleporters <= aEntranceKeys.length()) {

					Q_EMIT this->statusMessage(tr("Maximum number of teleporters reached. Click on an existing one to remove it and it's partner."));
					return 0u;

				} // if max reached

				for (int iPos = 0; iPos < aEntrances.length(); ++iPos) {

					ubState = aEntrances.at(iPos);
					if (aEntranceKeys.contains(ubState)) continue;

					Q_EMIT this->statusMessage(tr("Click on a cell to place a teleporter Entrance."));
					return ubState;

				} // loop

			} // if placing exit or entrance
		break;

		// floor
		case 0:
		default:
			return L::FloorClean;
		break;

	} // switch this->pUi->selectTool->currentIndex()

	// just to silence compiler
	return L::FloorClean;

} // currentBrushState


void SurfaceBuilder::dialogLoadFinished(const int iResult) {

	// canceled
	if (0 == iResult) return;

	this->ubCurrentLevel = quint8(this->pDialogLoad->getSelected());
	this->pAS->setValue(AppSettings::sSettingBuilderLastLevel,
						this->ubCurrentLevel);

	this->loadCurrentLevel();

} // dialogLoadFinished


void SurfaceBuilder::dialogSaveFinished(const int iResult) {

	// canceled
	if (0 == iResult) return;

	this->ubCurrentLevel = this->pDialogSave->getSelected();
	this->pAS->setValue(AppSettings::sSettingBuilderLastLevel,
						this->ubCurrentLevel);

	QString sPath = this->pAS->getDataPath();
	QDir oDir(sPath);
	if (!oDir.mkpath(sPath)) {

		this->onDebugMessage("KO: failed to create path: " + sPath);
		Q_EMIT this->statusMessage(tr("KO: failed to create path: ") + sPath);
		return;

	} // if failed to create path or path not existing

	sPath = this->pAS->getDataPathLevelFile(this->ubCurrentLevel);

	QByteArray aOut;
	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	quint8 ubState = 0u;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow = this->aopRows.at(ubRows);

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = aRow.at(ubColumns);

			ubState = pCell->getState();

			aOut.append(char(ubState));

		} // loop columns

	} // loop rows

	QFile oFile(sPath);
	if (oFile.open(QFile::WriteOnly)) {

		int iLen = oFile.write(aOut);

		if (iLen < aOut.length()) {

			this->onDebugMessage("KO: failed to write all. Only: "
								 + QString::number(iLen));

		} else {

			IconEngine::pIconEngine()->removeCacheOfLevel(this->ubCurrentLevel);

		} // if didn't write all or OK

	} else {

		this->onDebugMessage("KO: failed to write to: " + sPath);

	} // if opened or not

} // dialogSaveFinished


void SurfaceBuilder::init() {

	this->initBrushes();

	this->initCells();

	this->loadCurrentLevel();

} // init


void SurfaceBuilder::initBrushes() {

	QComboBox *pCB = this->pUi->selectTool;

	pCB->addItem(IconEngine::cell(L::FloorClean, true), tr("Floor / Eraser"));
	pCB->addItem(IconEngine::cell(L::WallCornerNE, true), tr("Wall Corner NE"));
	pCB->addItem(IconEngine::cell(L::WallCornerNW, true), tr("Wall Corner NW"));
	pCB->addItem(IconEngine::cell(L::WallCornerSE, true), tr("Wall Corner SE"));
	pCB->addItem(IconEngine::cell(L::WallCornerSW, true), tr("Wall Corner SW"));
	pCB->addItem(IconEngine::cell(L::WallCross, true), tr("Wall Cross"));
	pCB->addItem(IconEngine::cell(L::WallHorizontal, true), tr("Wall Horizontal"));
	pCB->addItem(IconEngine::cell(L::WallTeast, true), tr("Wall T East (Right)"));
	pCB->addItem(IconEngine::cell(L::WallTnorth, true), tr("Wall T North (Up)"));
	pCB->addItem(IconEngine::cell(L::WallTsouth, true), tr("Wall T South (Down)"));
	pCB->addItem(IconEngine::cell(L::WallTleft, true), tr("Wall T West (Left)"));
	pCB->addItem(IconEngine::cell(L::WallVertical, true), tr("Wall Vertical"));

	pCB->addItem(IconEngine::cell(L::SpawnHeadingNorth, true), tr("Spawn heading north"));
	pCB->addItem(IconEngine::cell(L::SpawnHeadingWest, true), tr("Spawn heading west"));
	pCB->addItem(IconEngine::cell(L::SpawnHeadingSouth, true), tr("Spawn heading south"));
	pCB->addItem(IconEngine::cell(L::SpawnHeadingEast, true), tr("Spawn heading east"));

	pCB->addItem(IconEngine::makeTeleporter(""), tr("Teleporter"));

	pCB->setCurrentIndex(this->pAS->get(
							 AppSettings::sSettingBuilderLastBrushIndex).toInt());

	// works, but needs to adapt to cell size
	//this->setCursor(QCursor(this->currentBrushIcon().pixmap(32, 32)));

} // initBrushes


void SurfaceBuilder::initCells() {

	quint8 ubRows = 0u;
	quint8 ubColumns = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	QHBoxLayout *pHBox;
	quint8 ubState = L::FloorClean;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow.clear();
		pHBox = new QHBoxLayout();

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = new SurfaceCell(true, ubState, ubColumns, ubRows, this);

			aRow.append(pCell);
			pHBox->addWidget(pCell);

			connect(pCell, SIGNAL(debugMessage(QString)),
					this, SLOT(onDebugMessage(QString)));

			connect(pCell, SIGNAL(clicked(quint8,quint8,bool,SurfaceCell*)),
					this, SLOT(onCellClicked(quint8,quint8,bool,SurfaceCell*)));

		} // loop columns

		this->aopRows.append(aRow);
		this->pUi->surfaceRows->addItem(pHBox);

	} // loop rows

} // initCells


void SurfaceBuilder::loadCurrentLevel() {

	QString sPath = this->pAS->getDataPathLevelFile(this->ubCurrentLevel);

	this->clearSurface();

	QFileInfo oFI = QFileInfo(sPath);
	if (!(oFI.exists() && oFI.isFile())) {

		this->onDebugMessage("KO: failed to find: " + sPath);
		return;

	} // if file does not exist

	QFile oFile(sPath);
	if (!oFile.open(QFile::ReadOnly)) {

		this->onDebugMessage("KO: failed to open: " + sPath);
		return;

	} // if failed to open

	QByteArray aFile = oFile.readAll();
	oFile.close();

	if ((SssS_Nibblers_Surface_Height * SssS_Nibblers_Surface_Width)
			> aFile.length()) {

		this->onDebugMessage("invalid length (too short)");
		return;

	} // if invalid length

	int iPos = 0u;
	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	quint8 ubState = L::FloorClean;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow = this->aopRows.at(ubRows);

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = aRow.at(ubColumns);

			ubState = quint8(aFile.at(iPos));

			this->setCellState(pCell, ubState);

			iPos++;

		} // loop columns

	} // loop rows

} // loadCurrentLevel


void SurfaceBuilder::on_buttonClear_clicked() {

	QVector<quint8> aStates;

	switch (this->pUi->selectClear->currentIndex()) {

		case 1: // Walls
			this->clearSurfaceOf(IconEngine::statesWalls());
		break;

		case 2: // Spawns
			this->clearSurfaceOf(IconEngine::statesSpawns());
		break;

		case 3: // Teleporters
			aStates = IconEngine::statesTeleporterEntrances();
			aStates.append(IconEngine::statesTeleporterExits());
			this->clearSurfaceOf(aStates);
		break;

		case 4: // Snakes
			this->clearSurfaceOf(IconEngine::statesSnakes());
		break;

		case 0:
		default:
			this->clearSurface();
		break;

	} // switch what to clear

} // on_buttonClear_clicked


void SurfaceBuilder::on_buttonLoad_clicked() {

	if (nullptr == this->pDialogLoad) {

		this->pDialogLoad = new DialogLoad(this);

		connect(this->pDialogLoad, SIGNAL(finished(int)),
			this, SLOT(dialogLoadFinished(int)));

	} // if first time

	this->pDialogLoad->setSelected(this->ubCurrentLevel);

	this->pDialogLoad->exec();

} // on_buttonLoad_clicked


void SurfaceBuilder::on_buttonSave_clicked() {

	if (nullptr == this->pDialogSave) {

		this->pDialogSave = new DialogSave(this);

		connect(this->pDialogSave, SIGNAL(finished(int)),
			this, SLOT(dialogSaveFinished(int)));

	} // if first time

	this->pDialogSave->setSelected(this->ubCurrentLevel);

	QString sMessage;
	// check for teleporter pairs
	QList<quint8> aKeysEntrances = this->hpTeleporterEntrances.keys();
	QList<quint8> aKeysExits = this->hpTeleporterExits.keys();
	quint8 ubState;
	quint8 ubMissing = 0u;
	int iPos = 0;
	for (; iPos < aKeysEntrances.length(); ++iPos) {

		ubState = aKeysEntrances.at(iPos);
		if (!aKeysExits.contains(ubState + 1u)) ubMissing++;

	} // loop
	if (ubMissing) {

		sMessage += tr(" Warning: ") + QString::number(ubMissing)
					+ tr(" teleporter entrances are missing their exits.");

	} // if found missing exits

	ubMissing = 0u;
	iPos = 0;
	for (; iPos < aKeysExits.length(); ++iPos) {

		ubState = aKeysExits.at(iPos);
		if (!aKeysEntrances.contains(ubState - 1u)) ubMissing++;

	} // loop
	if (ubMissing) {

		sMessage += tr(" Warning: ") + QString::number(ubMissing)
					+ tr(" teleporter exits are missing their entrances.");

	} // if found missing entrances

	// check for spawn points
	int iDiff = SssS_Nibblers_Max_Players - this->apSpawnPoints.length();
	if (0 < iDiff) {

		sMessage += tr(" Warning: missing ") + QString::number(iDiff)
					+ tr(" spawn-points. May not be playable!");

	} else if (0 > iDiff) {

		sMessage += tr("Notice: too many spawn-points set.");

	} // if need to alert about spawn-points

	this->pDialogSave->setWarning(sMessage);

	this->pDialogSave->exec();

} // on_buttonSave_clicked


void SurfaceBuilder::onCellClicked(const quint8 ubColumn, const quint8 ubRow,
								   bool bShift, SurfaceCell *pCell) {

	// has a click already occured?
	// or no shift held anyway?
	// or non-line-tool active?
	if ((0xFFu == this->ubLastColumn)
			|| (!bShift)
			|| (11 < this->pUi->selectTool->currentIndex())) {

		// no shift, first click or non-line-tool active

		this->setCellState(pCell, this->currentBrushState());

		this->ubLastColumn = ubColumn;
		this->ubLastRow = ubRow;

		return;

	} // if first click

	// Bresenham's line algorithm
	// adopted from: https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.2B.2B
	qreal fX1 = this->ubLastColumn;
	qreal fX2 = ubColumn;
	qreal fY1 = this->ubLastRow;
	qreal fY2 = ubRow;
	const bool bSteep = (qAbs(fY2 - fY1) > qAbs(fX2 - fX1));

	if (bSteep) {

		qSwap(fX1, fY1);
		qSwap(fX2, fY2);

	} // if 'steep'

	if (fX1 > fX2) {

		std::swap(fX1, fX2);
		std::swap(fY1, fY2);

	} // if reversed

	const float fDx = fX2 - fX1;
	const float fDy = qAbs(fY2 - fY1);

	float fError = fDx / 2.0f;

	const int iStepY = (fY1 < fY2) ? 1 : -1;
	int iY = int(fY1);

	const int iMaxX = int(fX2);

	for (int iX = int(fX1); iX < iMaxX; iX++) {

		if (bSteep) {

			this->setCellState(iY, iX, this->currentBrushState());

		} else {

			this->setCellState(iX, iY, this->currentBrushState());

		} // if steep

		fError -= fDy;
		if (fError < 0) {

			iY += iStepY;
			fError += fDx;

		} // if error margin negative

	} // loop

	this->ubLastColumn = ubColumn;
	this->ubLastRow = ubRow;

} // onCellClicked


void SurfaceBuilder::on_selectTool_currentIndexChanged(int iIndex) {

	// avoid while adding elements on init
	if (17 > this->pUi->selectTool->count()) return;

	// update status message
	if (0 == iIndex) {

		Q_EMIT this->statusMessage(tr("Click on cells to replace them with floor-tiles. Hold Shift for lines."));

	} else if (12 > iIndex) {

		Q_EMIT this->statusMessage(tr("Click on cells to replace them with walls. Hold Shift for lines."));

	} else this->currentBrushState();

	// save for next session
	this->pAS->setValue(AppSettings::sSettingBuilderLastBrushIndex, iIndex);

	// works, but needs to adapt to cell size
	//this->setCursor(QCursor(this->currentBrushIcon().pixmap(32, 32)));

} // on_selectTool_currentIndexChanged


void SurfaceBuilder::setCellState(SurfaceCell *pCell, const quint8 ubState,
								  const bool bUpdate) {

	quint8 ubStateOld = pCell->getState();

	// nothing to do?
	if (ubStateOld == ubState) {

		if (L::FloorClean == ubState) return;

		if (11 >= this->pUi->selectTool->currentIndex()) {

			pCell->setState(L::FloorClean);
			if (bUpdate) pCell->update();

		} // if wall-tool toggle between wall and floor

		return;

	} // if same state

	static QVector<quint8> aStatesSpawns = IconEngine::statesSpawns();
	static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();

	// check old state first
	bool bUpdateStatus = false;
	SurfaceCell *pCell2;
	quint8 ubStatePartner;

	if (aStatesSpawns.contains(ubStateOld)) {

		if (this->apSpawnPoints.contains(pCell)) this->apSpawnPoints.removeOne(pCell);
		bUpdateStatus = true;

	} else if (aStatesTeleporterEntrances.contains(ubStateOld)) {

		// overwriting an entrance -> delete the exit too
		if (this->hpTeleporterEntrances.contains(ubStateOld)) {
			this->hpTeleporterEntrances.remove(ubStateOld);
		}
		ubStatePartner = ubStateOld + 1u;
		if (this->hpTeleporterExits.contains(ubStatePartner)) {
			pCell2 = this->hpTeleporterExits.value(ubStatePartner);
			pCell2->setState(0u);
			if (bUpdate) pCell2->update();
			this->hpTeleporterExits.remove(ubStatePartner);
		}
		bUpdateStatus = true;

	} else if (aStatesTeleporterExits.contains(ubStateOld)) {

		// overwriting an exit -> delete the entrance too
		if (this->hpTeleporterExits.contains(ubStateOld)) {
			this->hpTeleporterExits.remove(ubStateOld);
		}
		ubStatePartner = ubStateOld - 1u;
		if (this->hpTeleporterEntrances.contains(ubStatePartner)) {
			pCell2 = this->hpTeleporterEntrances.value(ubStatePartner);
			pCell2->setState(0u);
			if (bUpdate) pCell2->update();
			this->hpTeleporterEntrances.remove(ubStatePartner);
		}
		bUpdateStatus = true;

	} // if special state we need to keep track of (old state)

	// now keep track of new one

	if (aStatesSpawns.contains(ubState)) {

		this->apSpawnPoints.append(pCell);
		bUpdateStatus = true;

	} else if (aStatesTeleporterEntrances.contains(ubState)) {

		this->hpTeleporterEntrances.insert(ubState, pCell);
		bUpdateStatus = true;

	} else if (aStatesTeleporterExits.contains(ubState)) {

		this->hpTeleporterExits.insert(ubState, pCell);
		bUpdateStatus = true;

	} // if special state we need to keep track of (new state)

	// finally change the cell's state and update if requested

	pCell->setState(ubState);
	if (bUpdate) pCell->update();
	if (bUpdateStatus) this->currentBrushState();

} // setCellState


void SurfaceBuilder::setCellState(const quint8 ubColumn, const quint8 ubRow,
								  const quint8 ubState, const bool bUpdate) {

	// check limits
	if (ubRow >= this->aopRows.count()) return;
	if (ubColumn >= this->aopRows.first().count()) return;

	QList<SurfaceCell *> aRow = this->aopRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	this->setCellState(pCell, ubState, bUpdate);

} // setCellState



}	} // namespace SwissalpS::QtNibblers
