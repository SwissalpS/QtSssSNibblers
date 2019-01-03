#include "SurfaceGame.h"
#include "ui_SurfaceGame.h"

#include "definitions.h"
#include "IconEngine.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>



namespace SwissalpS { namespace QtNibblers {



SurfaceGame::SurfaceGame(QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::SurfaceGame),
	pAS(AppSettings::pAppSettings()),
	ubCurrentLevel(0xFFu) {

	this->pUi->setupUi(this);

	this->aRows.clear();
	this->aWorms.clear();
	this->aSpawnPoints.clear();
	this->hTeleporterEntrances.clear();
	this->hTeleporterExits.clear();

	this->ubCurrentLevel = this->pAS->get(
							   AppSettings::sSettingGameStartLevel).toUInt();

} // construct


SurfaceGame::~SurfaceGame() {

	this->aRows.clear();
	this->aSpawnPoints.clear();
	this->hTeleporterEntrances.clear();
	this->hTeleporterExits.clear();

	delete this->pUi;

} // dealloc


void SurfaceGame::changeEvent(QEvent *pEvent) {

	QFrame::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		default:
		break;

	} // switch

} // changeEvent


void SurfaceGame::clearSurface() {

	this->aSpawnPoints.clear();
	this->hTeleporterEntrances.clear();
	this->hTeleporterExits.clear();

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
			this->setCellState(pCell, 0u, false);

		} // loop columns

	} // loop rows

	this->update();

} // clearSurface


void SurfaceGame::clearSurfaceOf(const quint8 ubState) {

	QVector<quint8> aStates;
	aStates.append(ubState);

	this->clearSurfaceOf(aStates);

} // clearSurfaceOf(single)


void SurfaceGame::clearSurfaceOf(const QVector<quint8> aStates) {

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

				this->setCellState(pCell, 0u, false);

			} // if one to clear

		} // loop columns

	} // loop rows

	this->update();

} // clearSurfaceOf(vector)


void SurfaceGame::init() {

	this->initCells();

	this->loadCurrentLevel();

	this->initWorms();

} // init


void SurfaceGame::initCells() {

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

			pCell = new SurfaceCell(false, ubState, ubColumns, ubRows, this);

			aRow.append(pCell);
			pHBox->addWidget(pCell);

			connect(pCell, SIGNAL(debugMessage(QString)),
					this, SLOT(onDebugMessage(QString)));

		} // loop columns

		this->aRows.append(aRow);
		this->pUi->surfaceRows->addItem(pHBox);

	} // loop rows

} // initCells


void SurfaceGame::initWorms() {

	this->aWorms.clear();
	this->clearSurfaceOf(IconEngine::statesSnakes());

	quint8 ubCount;
	quint8 ubCountAll = 0u;
	quint8 ubCountAIs = this->pAS->get(AppSettings::sSettingGameCountAIs).toUInt();
	quint8 ubCountHumans = this->pAS->get(AppSettings::sSettingGameCountHumans).toUInt();

	QVector<SurfaceCell *> aUsedSpawns;
	SurfaceCell *pCell;

	// check that there are enough spawn points
	if ((ubCountAIs + ubCountHumans) > this->aSpawnPoints.length()) {

		Q_EMIT this->statusMessage(tr("Level does not have sufficient spawn-points. Bailling."));
		return;

	} // if not enough start points

	for (ubCount = 0u; ubCount < ubCountHumans; ++ubCount) {

		// TODO: random spawn point distribution
		pCell = this->aSpawnPoints.at(ubCountAll);

		Worm *pWorm = new Worm(pCell, this->pAS->getPlayerColour(ubCountAll),
							   false, this);

		connect(pWorm, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		aUsedSpawns.append(pCell);
		this->aWorms.append(pWorm);

		ubCountAll++;

	} // loop humans

	for (ubCount = 0u; ubCount < ubCountAIs; ++ubCount) {

		// TODO: random spawn point distribution
		pCell = this->aSpawnPoints.at(ubCountAll);

		Worm *pWorm = new Worm(pCell, this->pAS->getPlayerColour(ubCountAll),
							   true, this);

		connect(pWorm, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		aUsedSpawns.append(pCell);
		this->aWorms.append(pWorm);

		ubCountAll++;

	} // loop AIs

} // initWorms


void SurfaceGame::loadCurrentLevel() {

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


void SurfaceGame::on_buttonPP_toggled(bool bChecked) {

	this->pUi->buttonPP->setText(bChecked ? tr("Pause") : tr("Play"));

} // on_buttonPP_toggled


void SurfaceGame::setCellState(SurfaceCell *pCell, const quint8 ubState,
								  const bool bUpdate) {

	quint8 ubStateOld = pCell->getState();

	// nothing to do?
	if (ubStateOld == ubState) return;

	static QVector<quint8> aStatesSpawns = IconEngine::statesSpawns();
	static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();

	// check old state first
	bool bUpdateStatus = false;
	SurfaceCell *pCell2;
	quint8 ubStatePartner;

	if (aStatesSpawns.contains(ubStateOld)) {

		if (this->aSpawnPoints.contains(pCell)) this->aSpawnPoints.removeOne(pCell);
		bUpdateStatus = true;

	} else if (aStatesTeleporterEntrances.contains(ubStateOld)) {

		// overwriting an entrance -> delete the exit too
		if (this->hTeleporterEntrances.contains(ubStateOld)) {
			this->hTeleporterEntrances.remove(ubStateOld);
		}
		ubStatePartner = ubStateOld + 1u;
		if (this->hTeleporterExits.contains(ubStatePartner)) {
			pCell2 = this->hTeleporterExits.value(ubStatePartner);
			pCell2->setState(0u);
			if (bUpdate) pCell2->update();
			this->hTeleporterExits.remove(ubStatePartner);
		}
		bUpdateStatus = true;

	} else if (aStatesTeleporterExits.contains(ubStateOld)) {

		// overwriting an exit -> delete the entrance too
		if (this->hTeleporterExits.contains(ubStateOld)) {
			this->hTeleporterExits.remove(ubStateOld);
		}
		ubStatePartner = ubStateOld - 1u;
		if (this->hTeleporterEntrances.contains(ubStatePartner)) {
			pCell2 = this->hTeleporterEntrances.value(ubStatePartner);
			pCell2->setState(0u);
			if (bUpdate) pCell2->update();
			this->hTeleporterEntrances.remove(ubStatePartner);
		}
		bUpdateStatus = true;

	} // if special state we need to keep track of (old state)

	// now keep track of new one

	if (aStatesSpawns.contains(ubState)) {

		this->aSpawnPoints.append(pCell);
		bUpdateStatus = true;

	} else if (aStatesTeleporterEntrances.contains(ubState)) {

		this->hTeleporterEntrances.insert(ubState, pCell);
		bUpdateStatus = true;

	} else if (aStatesTeleporterExits.contains(ubState)) {

		this->hTeleporterExits.insert(ubState, pCell);
		bUpdateStatus = true;

	} // if special state we need to keep track of (new state)

	// finally change the cell's state and update if requested

	pCell->setState(ubState);
	if (bUpdate) pCell->update();
	//if (bUpdateStatus) this->currentBrushState();

} // setCellState


void SurfaceGame::setCellState(const quint8 ubColumn, const quint8 ubRow,
								  const quint8 ubState, const bool bUpdate) {

	// check limits
	if (ubRow >= this->aRows.count()) return;
	if (ubColumn >= this->aRows.first().count()) return;

	QList<SurfaceCell *> aRow = this->aRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	this->setCellState(pCell, ubState, bUpdate);

} // setCellState



}	} // namespace SwissalpS::QtNibblers
