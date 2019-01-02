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
	ubDrawMode(0x00u),
	ubLastColumn(0xFFu),
	ubLastRow(0xFFu) {

	this->pUi->setupUi(this);

	this->aRows.clear();

	this->ubCurrentLevel = this->pAS->get(AppSettings::sSettingBuilderLastLevel).toUInt();

} // construct


SurfaceBuilder::~SurfaceBuilder() {

	this->aRows.clear();

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

	quint8 ubRows = this->aRows.count();
	quint8 ubColumns = this->aRows.first().count();
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pCell = aRow.at(ubX);
			pCell->setState(0u);
			//pCell->update();

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

	quint8 ubRows = this->aRows.count();
	quint8 ubColumns = this->aRows.first().count();
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pCell = aRow.at(ubX);

			if (aStates.contains(pCell->getState())) {

				pCell->setState(0u);
				//pCell->update();

			} // if one to clear

		} // loop columns

	} // loop rows

	this->update();

} // clearSurfaceOf(vector)


QIcon SurfaceBuilder::currentBrushIcon() const {

	return IconEngine::cell(this->currentBrushState(), true);

} // currentBrushIcon


quint8 SurfaceBuilder::currentBrushState() const {

	switch (this->pUi->selectTool->currentIndex()) {

		 // walls
		case 1: return 205u;
		case 2: return 204u;
		case 3: return 203u;
		case 4: return 202u;
		case 5: return 210u;
		case 6: return 201u;
		case 7: return 207u;
			// wall Ts
		case 8: return 206u;
		case 9: return 209u;
		case 10: return 208u;
		case 11: return 200u;
			// spawn points
		case 12: return 90u;
		case 13: return 91u;
		case 14: return 92u;
		case 15: return 93u;
			// Teleporter
			// TODO: detect which one
		case 16: return 220u;

			// floor
		case 0:
		default:
			return 0u;
		break;

	} // switch this->pUi->selectTool->currentIndex()

} // currentBrushState


void SurfaceBuilder::dialogLoadFinished(const int iResult) {

	// canceled
	if (0 == iResult) return;

	this->ubCurrentLevel = this->pDialogLoad->getSelected();
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
		return;

	} // if failed to create path or path not existing

	sPath += "Level_" + QString::number(this->ubCurrentLevel);

	QByteArray aOut;
	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	quint8 ubState = 0u;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow = this->aRows.at(ubRows);

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

	pCB->addItem(IconEngine::cell(0u, true), tr("Floor / Eraser"));
	pCB->addItem(IconEngine::cell(205u, true), tr("Wall corner NE"));
	pCB->addItem(IconEngine::cell(204u, true), tr("Wall corner NW"));
	pCB->addItem(IconEngine::cell(203u, true), tr("Wall corner SE"));
	pCB->addItem(IconEngine::cell(202u, true), tr("Wall corner SW"));
	pCB->addItem(IconEngine::cell(210u, true), tr("Wall cross"));
	pCB->addItem(IconEngine::cell(201u, true), tr("Wall horizontal"));
	pCB->addItem(IconEngine::cell(207u, true), tr("Wall T east"));
	pCB->addItem(IconEngine::cell(206u, true), tr("Wall T north"));
	pCB->addItem(IconEngine::cell(209u, true), tr("Wall T south"));
	pCB->addItem(IconEngine::cell(208u, true), tr("Wall T west"));
	pCB->addItem(IconEngine::cell(200u, true), tr("Wall vertical"));

	pCB->addItem(IconEngine::cell(90u, true), tr("Spawn heading north"));
	pCB->addItem(IconEngine::cell(91u, true), tr("Spawn heading west"));
	pCB->addItem(IconEngine::cell(92u, true), tr("Spawn south"));
	pCB->addItem(IconEngine::cell(93u, true), tr("Spawn east"));

	pCB->addItem(IconEngine::makeTeleporter(""), tr("Teleporter"));

	pCB->setCurrentIndex(this->pAS->get(
							 AppSettings::sSettingBuilderLastBrushIndex).toInt());

} // initBrushes


void SurfaceBuilder::initCells() {

	quint8 ubRows = 0u;
	quint8 ubColumns = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	QHBoxLayout *pHBox;
	quint8 ubState = 0u;

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

		this->aRows.append(aRow);
		this->pUi->surfaceRows->addItem(pHBox);

	} // loop rows

	Q_EMIT this->statusMessage(tr("Click on walls or empty space to toggle walls. Hold Shift for lines."));

} // initCells


void SurfaceBuilder::loadCurrentLevel() {

	QString sPath = this->pAS->getDataPath() + "Level_"
					+ QString::number(this->ubCurrentLevel);

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
	quint8 ubState = 0u;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow = this->aRows.at(ubRows);

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
	quint8 ubCount;

	switch (this->pUi->selectClear->currentIndex()) {

		case 1: // Walls
			for (ubCount = 200u; ubCount < 211u; ++ubCount)
				aStates.append(ubCount);

			this->clearSurfaceOf(aStates);
		break;

		case 2: // Spawns
			for (ubCount = 90u; ubCount < 94u; ++ubCount)
				aStates.append(ubCount);

			this->clearSurfaceOf(aStates);
		break;

		case 3: // Teleporters
			for (ubCount = 220u; ubCount < 240u; ++ubCount)
				aStates.append(ubCount);

			this->clearSurfaceOf(aStates);
		break;

		case 4: // Snakes
			aStates.append(10u);
			aStates.append(11u);
			aStates.append(12u);
			aStates.append(20u);
			aStates.append(21u);
			aStates.append(22u);
			aStates.append(30u);
			aStates.append(31u);
			aStates.append(32u);
			aStates.append(40u);
			aStates.append(41u);
			aStates.append(42u);
			aStates.append(50u);
			aStates.append(51u);
			aStates.append(52u);
			aStates.append(60u);
			aStates.append(61u);
			aStates.append(62u);
			aStates.append(70u);
			aStates.append(71u);
			aStates.append(72u);
			aStates.append(80u);
			aStates.append(81u);
			aStates.append(82u);

			this->clearSurfaceOf(aStates);
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

	this->pDialogSave->exec();

} // on_buttonSave_clicked


void SurfaceBuilder::on_buttonSetStarts_clicked() {

	if (this->ubDrawMode) {

		// currently setting start points -> cancel

		this->ubDrawMode = 0u;
		this->pUi->buttonSetStarts->setText(tr("Set Start Positions"));

		Q_EMIT this->statusMessage(tr("Click on cells to toggle walls. Hold shift for lines."));

	} else {

		// start fresh
		this->ubDrawMode = 10u;
		this->pUi->buttonSetStarts->setText(tr("Cancel Setting Starts"));

		Q_EMIT this->statusMessage(tr("Click on a free cell to set the head-point for player 1."));

		// remove existing one
		QVector<quint8> aStates;
		aStates.append(this->ubDrawMode);
		aStates.append(this->ubDrawMode + 1u);
		aStates.append(this->ubDrawMode + 2u);
		this->clearSurfaceOf(aStates);

	} // if already in draw mode or starting

} // on_buttonSetStarts_clicked


void SurfaceBuilder::onCellClicked(const quint8 ubColumn, const quint8 ubRow,
								   bool bShift, SurfaceCell *pCell) {

	// are we setting start points?
	if (this->ubDrawMode) {

		this->onCellClickedForStartPoints(ubColumn, ubRow, pCell);

		return;

	} // if setting start points

	// has a click already occured?
	// or no shift held anyway?
	// or non-line-tool active?
	if ((0xFFu == this->ubLastColumn)
			|| (!bShift)
			|| (11 < this->pUi->selectTool->currentIndex())) {

		// no shift, first click or non-line-tool active

		this->setCellState(pCell, this->currentBrushState());
		//this->toggleCell(pCell);

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
			//this->toggleCell(iY, iX);

		} else {

			this->setCellState(iX, iY, this->currentBrushState());
			//this->toggleCell(iX, iY);

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


void SurfaceBuilder::onCellClickedForStartPoints(const quint8 ubColumn,
												 const quint8 ubRow,
												 SurfaceCell *pCell) {

	// if last bit is 1 -> we are setting a tail after having set head already
	bool bTail = this->ubDrawMode & 1u;

	// check if available (not wall and not other player)
	// this also asures that tail is not placed on head
	if (0u != pCell->getState()) {

		Q_EMIT this->statusMessage(tr("Choose a free spot for the head."));

		return;

	} // if not free cell

	if (bTail) {

		// check if orthogonal

		if ((ubColumn == this->ubLastColumn)
				|| (ubRow == this->ubLastRow)) {

			// ok, place it
			this->setCellState(pCell, this->ubDrawMode);

			// fill in the space in between
			quint8 ubMidState = this->ubDrawMode + 1u;
			if (0u == ubColumn - this->ubLastColumn) {

				// same column -> vertical

				// orient direction
				quint8 ubY1 = qMin(ubRow, this->ubLastRow);
				quint8 ubY2 = qMax(ubRow, this->ubLastRow);

				// avoid the head
				if (ubY1 == this->ubLastRow) ubY1++;
				else if (0u != ubY2) ubY2--;

				for (; ubY1 <= ubY2; ubY1++) {

					this->setCellState(ubColumn, ubY1, ubMidState);

				} // loop mid section

			} else {

				// same row -> horizontal

				// orient direction
				quint8 ubX1 = qMin(ubColumn, this->ubLastColumn);
				quint8 ubX2 = qMax(ubColumn, this->ubLastColumn);

				// avoid the head
				if (ubX1 == this->ubLastColumn) ubX1++;
				else if (0u != ubX2) ubX2--;

				for (; ubX1 <= ubX2; ubX1++) {

					this->setCellState(ubX1, ubRow, ubMidState);

				} // loop mid section

			} // if horizontal or vertical

		} else {

			// not orthogonal -> bad skip
			Q_EMIT this->statusMessage(tr("Tail must be orthogonal to head. Try again to place tail."));

			return;

		} // if orthogonal or not

	} else {

		// TODO: check if there are any valid free cells adjacent

		// ok, place it
		this->setCellState(pCell, this->ubDrawMode);

	} // if tail or head

	// calculate next step

	if (bTail) {

		// next click is head of next player
		this->ubDrawMode += 9u;

		// are we done?
		if (90u == this->ubDrawMode) {

			this->ubDrawMode = 0u;

			// reset button and status
			this->pUi->buttonSetStarts->setText(tr("Set Start Positions"));
			Q_EMIT this->statusMessage(tr("Click on walls or empty space to toggle walls. Hold Shift for lines."));

			this->ubLastColumn = 0xFFu;
			this->ubLastRow = 0xFFu;

			return;

		} // if done

		// remove existing one
		QVector<quint8> aStates;
		aStates.append(this->ubDrawMode);
		aStates.append(this->ubDrawMode + 1u);
		aStates.append(this->ubDrawMode + 2u);
		this->clearSurfaceOf(aStates);

		Q_EMIT this->statusMessage(tr("Click on a free cell to set the head-point for player ")
								   + QString::number(quint8(qreal(this->ubDrawMode) * 0.1f))
								   + ".");


	} else {

		// set to tail of same player
		this->ubDrawMode += 1u;

		// update status
		Q_EMIT this->statusMessage(tr("Click on a free cell to set the tail-point for player ")
								   + QString::number(quint8(qreal(this->ubDrawMode) * 0.1f))
								   + ". " + tr("The tail needs to be in the same column or row as the head."));

	} // if tail or head

	this->ubLastColumn = ubColumn;
	this->ubLastRow = ubRow;

} // onCellClickedForStartPoints


void SurfaceBuilder::on_selectTool_currentIndexChanged(int iIndex) {

	if (17 > this->pUi->selectTool->count()) return;

	this->pAS->setValue(AppSettings::sSettingBuilderLastBrushIndex, iIndex);

} // on_selectTool_currentIndexChanged


void SurfaceBuilder::setCellState(SurfaceCell *pCell, const quint8 ubState) {

	pCell->setState(ubState);
	pCell->update();

} // setCellState


void SurfaceBuilder::setCellState(const quint8 ubColumn, const quint8 ubRow,
								  const quint8 ubState) {

	// check limits
	if (ubRow >= this->aRows.count()) return;
	if (ubColumn >= this->aRows.first().count()) return;

	QList<SurfaceCell *> aRow = this->aRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	this->setCellState(pCell, ubState);

} // setCellState


void SurfaceBuilder::toggleCell(SurfaceCell *pCell) {

	quint8 ubState = pCell->getState();

	if (0u == ubState) ubState = 200u;
	else if (200u == ubState) ubState = 0u;
	else return;

	pCell->setState(ubState);
	pCell->update();

} // toggleCell


void SurfaceBuilder::toggleCell(const quint8 ubColumn, const quint8 ubRow) {

	// check limits
	if (ubRow >= this->aRows.count()) return;
	if (ubColumn >= this->aRows.first().count()) return;

	QList<SurfaceCell *> aRow = this->aRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	this->toggleCell(pCell);

} // toggleCell



}	} // namespace SwissalpS::QtNibblers
