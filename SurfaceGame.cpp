#include "SurfaceGame.h"
#include "ui_SurfaceGame.h"

#include "definitions.h"
#include "IconEngine.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>



namespace SwissalpS { namespace QtNibblers {



SurfaceGame::SurfaceGame(QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::SurfaceGame),
	pAS(AppSettings::pAppSettings()),
	ubCurrentLevel(0xFFu) {

	this->pUi->setupUi(this);

	this->aopRows.clear();
	this->apScoreBoards.clear();
	this->apWorms.clear();
	this->apSpawnPoints.clear();
	this->hpTeleporterEntrances.clear();
	this->hpTeleporterExits.clear();

	this->ubCurrentLevel = this->pAS->get(
							   AppSettings::sSettingGameStartLevel).toUInt();

} // construct


SurfaceGame::~SurfaceGame() {

	this->aopRows.clear();
	this->apScoreBoards.clear();
	this->apWorms.clear();
	this->apSpawnPoints.clear();
	this->hpTeleporterEntrances.clear();
	this->hpTeleporterExits.clear();

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

				this->setCellState(pCell, 0u, false);

			} // if one to clear

		} // loop columns

	} // loop rows

	this->update();

} // clearSurfaceOf(vector)


void SurfaceGame::clearSurfaceOfWorm(const quint8 ubWormColourIndex) {

	this->clearSurfaceOf(IconEngine::statesSnake(ubWormColourIndex));

} // clearSurfaceOfWorm


void SurfaceGame::clearSurfaceOfWorms() {

	this->clearSurfaceOf(IconEngine::statesSnakes());

} // clearSurfaceOfWorms


void SurfaceGame::countdownTick() {

	int iTick = this->pUi->buttonPP->text().toInt() - 1;

	if (0 < iTick) {

		QString sMessage = QString::number(iTick);
		this->pUi->buttonPP->setText(sMessage);
		Q_EMIT this->statusMessage(sMessage);

		QTimer::singleShot(1000, this, SLOT(countdownTick()));

		return;

	} // if still got ticks to go

	this->pUi->buttonPP->setChecked(true);
	this->pUi->buttonPP->setEnabled(true);
	this->pUi->buttonSR->setEnabled(true);
	this->pUi->buttonPP->setText(tr("Pause"));
	Q_EMIT this->statusMessage(tr("Go, Go, Goooooh!"));

	Q_EMIT this->pauseResumeToggled();

} // countdownTick


SurfaceCell *SurfaceGame::getCell(const QPoint oPoint) {

	return this->getCell(oPoint.x(), oPoint.y());

} // getCell(point)


SurfaceCell *SurfaceGame::getCell(const quint8 ubColumn, const quint8 ubRow) {

	// check limits
	if (ubRow >= this->aopRows.count()) return new SurfaceCell();
	if (ubColumn >= this->aopRows.first().count()) return new SurfaceCell();

	QList<SurfaceCell *> aRow = this->aopRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	return pCell;

} // getCell(x,y)


void SurfaceGame::init() {

	this->initCells();

	this->loadCurrentLevel();

	this->initWorms();

	this->pUi->buttonPP->setChecked(false);
	this->pUi->buttonPP->setEnabled(true);
	this->pUi->buttonPP->setText(tr("Start"));

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

		this->aopRows.append(aRow);
		this->pUi->surfaceRows->addItem(pHBox);

	} // loop rows

} // initCells


void SurfaceGame::initWorms() {

	this->apWorms.clear();
	this->clearSurfaceOfWorms();

	quint8 ubColour;
	quint8 ubCount;
	quint8 ubCountAIs = this->pAS->get(AppSettings::sSettingGameCountAIs).toUInt();
	quint8 ubCountHumans = this->pAS->get(AppSettings::sSettingGameCountHumans).toUInt();
	quint8 ubLives = this->pAS->get(AppSettings::sSettingGameStartLives).toUInt();

	QVector<SurfaceCell *> aUsedSpawns;
	SurfaceCell *pCell;
	Worm *pWorm;
	ScoreBoard *pSB;
	quint8 ubCountAll = ubCountAIs + ubCountHumans;

	// check that there are enough spawn points
	if (ubCountAll > this->apSpawnPoints.length()) {

		this->pUi->buttonPP->setEnabled(false);
		Q_EMIT this->statusMessage(tr("Level does not have sufficient spawn-points. Bailling."));
		return;

	} // if not enough start points

	for (ubCount = 0u; ubCount < ubCountAll; ++ubCount) {

		// TODO: random spawn point distribution
		pCell = this->apSpawnPoints.at(ubCount);

		ubColour = this->pAS->getPlayerColour(ubCount);

		pWorm = new Worm(pCell, ubColour, (ubCount >= ubCountHumans), this);

		connect(pWorm, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		pSB = new ScoreBoard(ubColour, this);
		this->pUi->frameScore->layout()->addWidget(pSB);

		connect(pSB, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		// connect worm with score board
		connect(pWorm, SIGNAL(updateLives(quint8)),
				pSB, SLOT(setLives(quint8)));

		connect(pWorm, SIGNAL(updateName(QString)),
				pSB, SLOT(setName(QString)));

		connect(pWorm, SIGNAL(updateScore(quint32)),
				pSB, SLOT(setScore(quint32)));

		aUsedSpawns.append(pCell);
		this->apWorms.append(pWorm);
		this->apScoreBoards.append(pSB);

		Q_EMIT this->wormCreated(pWorm);

		pWorm->onSetLives(ubLives);
		pWorm->onSetName(tr("Player ") + QString::number(ubCount + 1u));

	} // loop worms

} // initWorms


void SurfaceGame::loadCurrentLevel() {

	QString sPath = this->pAS->getDataPathLevelFile(this->ubCurrentLevel);

	this->pUi->labelLevel->setText(tr("Level ") + QString::number(this->ubCurrentLevel));

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

	static QVector<quint8> aStatesSpawns = IconEngine::statesSpawns();
	static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();

	int iPos = 0u;
	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	quint8 ubState = 0u;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow = this->aopRows.at(ubRows);

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = aRow.at(ubColumns);

			ubState = quint8(aFile.at(iPos));

			// keep track of speciol ones

			if (aStatesSpawns.contains(ubState)) {

				this->apSpawnPoints.append(pCell);

			} else if (aStatesTeleporterEntrances.contains(ubState)) {

				this->hpTeleporterEntrances.insert(ubState, pCell);

			} else if (aStatesTeleporterExits.contains(ubState)) {

				this->hpTeleporterExits.insert(ubState, pCell);

			} // if special state we need to keep track of (new state)

			this->setCellState(pCell, ubState);
			pCell->freezeState();

			iPos++;

		} // loop columns

	} // loop rows

} // loadCurrentLevel


void SurfaceGame::on_buttonPP_toggled(bool bChecked) {

	this->pUi->buttonPP->setText(bChecked ? tr("Pause") : tr("Play"));

	Q_EMIT this->pauseResumeToggled();

} // on_buttonPP_toggled


void SurfaceGame::on_buttonSR_clicked() {

} // on_buttonSR_clicked


void SurfaceGame::onDoLevelStartCountdown() {

	this->onMove();
	this->onMove();
	this->onMove();
	this->onMove();
	this->onMove();

	this->pUi->buttonPP->setText("3");
	this->pUi->buttonPP->setEnabled(false);
	this->pUi->buttonSR->setEnabled(false);
	QTimer::singleShot(1000, this, SLOT(countdownTick()));
	Q_EMIT this->statusMessage(tr("Get Ready: 3..."));

} // onDoLevelStartCountdown


void SurfaceGame::onMove() {

	static QVector<quint8> aStatesSnakes = IconEngine::statesSnakes();
	static QVector<quint8> aStatesSpawns = IconEngine::statesSpawns();
	static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();
	static QVector<quint8> aStatesWalls = IconEngine::statesWalls();

	Worm *pWorm;
	SurfaceCell *pCell;
	quint8 ubState;
	QHash<SurfaceCell *, Worm *> hppCrashPotential;
	QVector<Worm *> apCrashedWorms;
	hppCrashPotential.clear();
	apCrashedWorms.clear();

	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWorm = this->apWorms.at(i);

		if (pWorm->isDead()) continue;

		pCell = this->getCell(pWorm->nextPoint());

		if (pCell->isNull()) continue;

		if (pWorm->isImmune()) {

			pWorm->advanceTo(pCell);
			continue;

		} //

		ubState = pCell->getState();

		if (aStatesSnakes.contains(ubState)
				|| aStatesWalls.contains(ubState)) {

			// crash
			apCrashedWorms.append(pWorm);
			continue;

		} else if (aStatesTeleporterEntrances.contains(ubState)) {

			// enter teleporter
			// TODO: find matching exit
			pWorm->advanceTo(pCell);

		} // if not empty


		pWorm->advanceTo(pCell);
		if (hppCrashPotential.keys().contains(pCell)) {
			// head-on-colision
			apCrashedWorms.append(pWorm);
			if (!apCrashedWorms.contains(hppCrashPotential.value(pCell))) {
				apCrashedWorms.append(hppCrashPotential.value(pCell));
			}
		} else hppCrashPotential.insert(pCell, pWorm);

	} // loop

	for (int i = 0; i < apCrashedWorms.length(); ++i) {

		pWorm = apCrashedWorms.at(i);
		Q_EMIT this->wormCrashed(pWorm);
		this->clearSurfaceOfWorm(pWorm->colourIndex());

	} // loop


} // onMove


void SurfaceGame::onNextLevel() {

} // onNextLevel


void SurfaceGame::onPlaceBonus(const quint8 ubBonus) {

} // onPlaceBonus


void SurfaceGame::onSpawnWorm(const quint8 ubWorm) {

	this->onDebugMessage("spawn work");

} // onSpawnWorm


void SurfaceGame::onSpawnWorm(Worm *pWorm) {

	pWorm->startSpawning();


	return;
	this->onDebugMessage("spawn wormmmm");

	// go 5 steps in original direction
	SurfaceCell *pCellNext;
	for (quint8 ubCount = 0u; ubCount < 5u; ++ubCount) {

		pCellNext = this->getCell(pWorm->nextPoint());

	} // loop

} // onSpawnWorm


void SurfaceGame::onSpawnWorms() {

	this->onDebugMessage("spawn worssssssssssss");

	quint8 ubTotal = this->apWorms.length();
	quint8 ubCount = 0u;
	for (; ubCount < ubTotal; ++ubCount) {

		this->onSpawnWorm(ubCount);

	} // loop worms

} // onSpawnWorms


void SurfaceGame::setCellState(SurfaceCell *pCell, const quint8 ubState,
								  const bool bUpdate) {

	quint8 ubStateOld = pCell->getState();

	// nothing to do?
	if (ubStateOld == ubState) return;

	// finally change the cell's state and update if requested

	pCell->setState(ubState);
	if (bUpdate) pCell->update();

} // setCellState


void SurfaceGame::setCellState(const quint8 ubColumn, const quint8 ubRow,
								  const quint8 ubState, const bool bUpdate) {

	// check limits
	if (ubRow >= this->aopRows.count()) return;
	if (ubColumn >= this->aopRows.first().count()) return;

	QList<SurfaceCell *> aRow = this->aopRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	this->setCellState(pCell, ubState, bUpdate);

} // setCellState



}	} // namespace SwissalpS::QtNibblers
