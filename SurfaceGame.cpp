#include "SurfaceGame.h"
#include "ui_SurfaceGame.h"

#include "definitions.h"
#include "IconEngine.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>



namespace SwissalpS { namespace QtNibblers {



SurfaceGame::SurfaceGame(QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::SurfaceGame),
	bProtectPP(false),
	pAS(AppSettings::pAppSettings()),
	pDialogLoad(nullptr),
	pStartCountDownFrame(nullptr),
	ubCurrentLevel(0xFFu) {

	this->pUi->setupUi(this);

	// init randomizer
	qsrand(QTime::currentTime().msecsSinceStartOfDay());

	// for forced aspect ratio
	this->iLastHeight = height();

	// for key events
	setFocusPolicy(Qt::StrongFocus);

	this->ahKeys.clear();
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


void SurfaceGame::focusInEvent(QFocusEvent *pEvent) {

	//this->onDebugMessage("todo: focus in");

	QFrame::focusInEvent(pEvent);

} // focusInEvent


void SurfaceGame::focusOutEvent(QFocusEvent *pEvent) {

	if (this->pUi->buttonPP->hasFocus()) return;
	if (this->pStartCountDownFrame->isVisible()) return;

	this->pauseIfRunning();

	QFrame::focusOutEvent(pEvent);

} // focusOutEvent


void SurfaceGame::countdownTick() {

	int iTick = this->pUi->buttonPP->text().toInt() - 1;

	if (0 < iTick) {

		QString sMessage = QString::number(iTick);
		this->pStartCountDownFrame->onSetText(sMessage);
		this->pStartCountDownFrame->update();
		this->pUi->buttonPP->setText(sMessage);
		Q_EMIT this->statusMessage(sMessage);

		QTimer::singleShot(1000, this, SLOT(countdownTick()));

		return;

	} // if still got ticks to go

	this->pStartCountDownFrame->hide();
	this->pUi->buttonPP->setChecked(true);
	this->pUi->buttonPP->setEnabled(true);
	this->pUi->buttonSR->setEnabled(true);
	this->pUi->buttonPP->setText(tr("Pause"));
	Q_EMIT this->statusMessage(tr("Go, Go, Goooooh!"));

	Q_EMIT this->pauseResumeToggled();

} // countdownTick


void SurfaceGame::dialogLoadFinished(const int iResult) {

	// canceled
	if (0 == iResult) return;

	this->ubCurrentLevel = this->pDialogLoad->getSelected();

	// TODO: make checkbox in settings for this feature
	this->pAS->setValue(AppSettings::sSettingGameStartLevel, this->ubCurrentLevel);

	this->loadCurrentLevel();

	Q_EMIT this->resetGame();

	this->resetButtons();

} // dialogLoadFinished


void SurfaceGame::findNextMovesForWorm(Worm *pWorm) {

	static QVector<quint8> aStatesPickups = IconEngine::statesPickups();
	//static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	//static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();
	static QVector<quint8> aStatesToAvoid =
			IconEngine::statesSnakes() + IconEngine::statesWalls();

	SurfaceCell *pCell = this->getCell(pWorm->nextPoint());
	if (pCell->isNull()) return;

	bool bCanTurnLeft = false;
	bool bCanTurnRight = false;
	quint8 ubCanTurnCount;

	quint8 ubState = pCell->getState();

	if (aStatesToAvoid.contains(ubState)) {

		// need to change direction if possible

		bCanTurnLeft = !aStatesToAvoid.contains(
						   this->getCell(pWorm->leftPoint())->getState());

		bCanTurnRight = !aStatesToAvoid.contains(
							this->getCell(pWorm->rightPoint())->getState());

		ubCanTurnCount = quint8(bCanTurnLeft) + quint8(bCanTurnRight);

		// no safe direction
		if (0u == ubCanTurnCount) return;

		// either direction is free
		if (2u == ubCanTurnCount) {

			// pick a direction
			if (qrand() & 1) pWorm->onTurnLeft();
			else pWorm->onTurnRight();

			return;

		} // if can go either way

		if (bCanTurnLeft) pWorm->onTurnLeft();
		else pWorm->onTurnRight();

		return;

	} else if (aStatesPickups.contains(ubState)) {

		// we're good, keep going
		return;

	} // if need to imediately turn or keep going

	// cell ahead contains no imediate danger or bonus
	// find the nearest bonus (and prioratize)

	pCell = pWorm->headCell();
	if (pCell->isNull()) return;

	QVector<quint8> aubAhead = this->nextPOIinDirection(pCell, pWorm->currentDirection());
	QVector<quint8> aubLeft = this->nextPOIinDirection(pCell, pWorm->headingLeft());
	QVector<quint8> aubRight = this->nextPOIinDirection(pCell, pWorm->headingRight());

	// which is the most promising?
	QVector<quint8> aubGoodDistances;
	if (2u == aubAhead.at(2)) aubGoodDistances.append(1u);
	if (2u == aubLeft.at(2)) aubGoodDistances.append(2u);
	if (2u == aubRight.at(2)) aubGoodDistances.append(3u);

	// which is nearest?
	quint8 ubNearestDistance = 0xFFu;
	quint8 ubIndexNearestDistance = 0u;
	if (aubGoodDistances.contains(1u) && ubNearestDistance > aubAhead.at(1)) {
		ubNearestDistance = aubAhead.at(1);
		ubIndexNearestDistance = 1u;
	} // if ahead has something good

	if (aubGoodDistances.contains(2u) && ubNearestDistance > aubLeft.at(1)) {
		ubNearestDistance = aubLeft.at(1);
		ubIndexNearestDistance = 2u;
	} // if left has something good

	if (aubGoodDistances.contains(3u) && ubNearestDistance > aubRight.at(1)) {
		ubNearestDistance = aubRight.at(1);
		ubIndexNearestDistance = 3u;
	} // if right has something good

	switch (ubIndexNearestDistance) {

		case 3u: pWorm->onTurnRight(); return;
		case 2u: pWorm->onTurnLeft(); return;
		case 1u: return;

		case 0u:
		default:
		break;

	} // switch ubIndexNearestDistance

	// nothing promising to go for
	// randomly change direction
	if (!((qrand() & 1u) && (qrand() & 1u))) return;

	// which is the most promising?
	QVector<quint8> aubBadDistances;
	if ((1u == aubAhead.at(2)) && (0u < aubAhead.at(1))) aubBadDistances.append(1u);
	if ((1u == aubLeft.at(2)) && (0u < aubLeft.at(1))) aubBadDistances.append(2u);
	if ((1u == aubRight.at(2)) && (0u < aubRight.at(1))) aubBadDistances.append(3u);

	// which is farthest?
	quint8 ubFarthestDistance = 0u;
	quint8 ubIndexFarthestDistance = 0u;
	if (aubBadDistances.contains(1u) && (ubFarthestDistance < aubAhead.at(1))) {
		ubFarthestDistance = aubAhead.at(1);
		ubIndexFarthestDistance = 1u;
	} // if ahead has something bad

	if (aubBadDistances.contains(2u) && (ubFarthestDistance < aubLeft.at(1))) {
		ubFarthestDistance = aubLeft.at(1);
		ubIndexFarthestDistance = 2u;
	} // if left has something bad

	if (aubBadDistances.contains(3u) && (ubFarthestDistance < aubRight.at(1))) {
		ubFarthestDistance = aubRight.at(1);
		ubIndexFarthestDistance = 3u;
	} // if right has something bad

	switch (ubIndexFarthestDistance) {

		case 3u: pWorm->onTurnRight(); return;
		case 2u: pWorm->onTurnLeft(); return;
		case 1u: return;

		case 0u:
			// should not happen
		default:
		break;

	} // switch ubIndex

} // findNextMovesForWorm


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

	this->initKeys();

	this->resetButtons();

} // init


void SurfaceGame::initCells() {

	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	quint8 ubState = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	QHBoxLayout *pHBox;
	QVBoxLayout *pVBox = new QVBoxLayout();

	pVBox->setSpacing(0);

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
		pVBox->addItem(pHBox);

	} // loop rows

	this->pUi->frameSurface->setLayout(pVBox);

} // initCells


void SurfaceGame::initKeys() {

	this->ahKeys.clear();

	QHash<QKeySequence, L::Heading> hKeys;
	QKeySequence oKS;
	quint8 ubTotalHumans = this->pAS->get(AppSettings::sSettingGameCountHumans).toUInt();

	for (quint8 ubWorm = 0u; ubWorm < ubTotalHumans; ++ubWorm) {

		hKeys.clear();
		oKS = QKeySequence(this->pAS->getPlayerKeyDown(ubWorm),
						   QKeySequence::PortableText);
		hKeys.insert(oKS, L::Down);
		oKS = QKeySequence(this->pAS->getPlayerKeyLeft(ubWorm),
						   QKeySequence::PortableText);
		hKeys.insert(oKS, L::Left);
		oKS = QKeySequence(this->pAS->getPlayerKeyRight(ubWorm),
						   QKeySequence::PortableText);
		hKeys.insert(oKS, L::Right);
		oKS = QKeySequence(this->pAS->getPlayerKeyUp(ubWorm),
						   QKeySequence::PortableText);
		hKeys.insert(oKS, L::Up);

		this->ahKeys.append(hKeys);

	} // loop

} // initKeys


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

		pWorm->setUseRelativeControls(this->pAS->getPlayerRelative(ubCount));

		connect(pWorm, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		pSB = new ScoreBoard(ubColour, this);
		this->pUi->frameScore->layout()->addWidget(pSB);

		connect(pSB, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		// connect worm with score board
		connect(pWorm, SIGNAL(updateColour(quint8)),
				pSB, SLOT(setColour(quint8)));

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


void SurfaceGame::keyPressEvent(QKeyEvent *pEvent) {

	QKeySequence oKSin(pEvent->key());
//	QKeySequence oKey2("K");

//	if (oKSin == oKey2) this->onDebugMessage("Matched K");
//	else this->onDebugMessage(QString::number(pEvent->key()));

	quint8 ubWorm;
	QHash<QKeySequence, L::Heading> hKeys;
	bool bNoMatchFound = true;

	for (ubWorm = 0u; ubWorm < this->ahKeys.length(); ++ubWorm) {

		hKeys = this->ahKeys.at(ubWorm);
		if (!hKeys.contains(oKSin)) continue;

		bNoMatchFound = false;

		this->apWorms.at(ubWorm)->onTurn(hKeys.value(oKSin));

	} // loop each set of keys

	if (bNoMatchFound) QFrame::keyPressEvent(pEvent);

} // keyPressEvent


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


QVector<quint8> SurfaceGame::nextPOIinDirection(SurfaceCell *pCell,
											 const L::Heading eDirection) {

	static QVector<quint8> aStatesPickups = IconEngine::statesPickups();
	//static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	//static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();
	static QVector<quint8> aStatesToAvoid =
			IconEngine::statesSnakes() + IconEngine::statesWalls();

	QVector<quint8> aubResult;
	quint8 ubX = pCell->getColumn();
	quint8 ubY = pCell->getRow();
	int iDx = 0;
	int iDy = 0;

	switch (eDirection) {

		case L::North: iDy = -1; break;
		case L::South: iDy = 1; break;
		case L::West: iDx = -1; break;
		case L::East: iDx = 1; break;

	} // switch eDirection

	quint8 ubDistance = 0;
	quint8 ubState;
	while (true) {

		ubDistance++;

		if (0 > iDx) {
			if (0 == ubX) ubX = SssS_Nibblers_Surface_Width - 1u;
			else ubX--;
		} else {
			ubX++;
			if (SssS_Nibblers_Surface_Width <= ubX) ubX = 0u;
		} // if horizontal direction

		if (0 > iDy) {
			if (0 == ubY) ubY = SssS_Nibblers_Surface_Height - 1u;
			else ubY--;
		} else {
			ubY++;
			if (SssS_Nibblers_Surface_Height <= ubY) ubY = 0u;
		} // if vertical direction

		ubState = this->getCell(ubX, ubY)->getState();

		if (aStatesToAvoid.contains(ubState)) {

			aubResult.append(ubState);
			aubResult.append(ubDistance);
			aubResult.append(1u);
			return aubResult;

		} // if something to avoid encountered

		if (aStatesPickups.contains(ubState)) {

			aubResult.append(ubState);
			aubResult.append(ubDistance);
			aubResult.append(2u);
			return aubResult;

		} // if something worth while encountered

	} // loop true

	// should never happen but jic
	aubResult.append(0XFFu);
	aubResult.append(0xFFu);
	aubResult.append(0xFFu);
	return aubResult;

} // nextPOIinDirection


void SurfaceGame::on_buttonPP_toggled(bool bChecked) {

	this->pUi->buttonPP->setText(bChecked ? tr("Pause") : tr("Play"));

	if (this->bProtectPP) return;

	Q_EMIT this->pauseResumeToggled();

	if (bChecked) this->setFocus();

} // on_buttonPP_toggled


void SurfaceGame::on_buttonSR_clicked() {

	// pause game if running
	this->pauseIfRunning();

	// open restart dialog
	if (nullptr == this->pDialogLoad) {

		this->pDialogLoad = new DialogLoad(this);

		connect(this->pDialogLoad, SIGNAL(finished(int)),
			this, SLOT(dialogLoadFinished(int)));

	} // if first time

	this->pDialogLoad->setSelected(this->ubCurrentLevel);

	this->pDialogLoad->exec();

} // on_buttonSR_clicked


void SurfaceGame::onColoursChanged(const QVector<quint8> aubColours) {

	// better safe than sorry
	if (aubColours.length() < this->apWorms.length()) return;

	quint8 ubWorm;
	for (ubWorm = 0u; ubWorm < this->apWorms.length(); ++ubWorm) {

		this->apWorms.at(ubWorm)->setColourIndex(aubColours.at(ubWorm));

	} // loop

} // onColoursChanged


void SurfaceGame::onDoGameOver() {

	// button is probably showing "Pause"
	if (this->pUi->buttonPP->isChecked()) {
		this->bProtectPP = true;
		this->pUi->buttonPP->setChecked(false);
		this->pUi->buttonPP->setEnabled(false);
		this->bProtectPP = false;
	}

	// show game over dialog

	Q_EMIT this->statusMessage("Game Over");

} // onDoGameOver


void SurfaceGame::onDoLevelStartCountdown() {

	// go 5 steps in original direction
	this->onMove();
	this->onMove();
	this->onMove();
	this->onMove();
	this->onMove();

	// open count-down dialog
	if (nullptr == this->pStartCountDownFrame) {

		FrameStartCountdown *pF = new FrameStartCountdown();
		this->pStartCountDownFrame = pF;
		pF->setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

	} // if first time

	this->pStartCountDownFrame->onSetText("3");
	this->pStartCountDownFrame->setGeometry(this->window()->geometry());
	this->pStartCountDownFrame->show();

	this->pUi->buttonPP->setText("3");
	this->pUi->buttonPP->setEnabled(false);
	this->pUi->buttonSR->setEnabled(false);

	Q_EMIT this->statusMessage(tr("Get Ready: 3..."));

	QTimer::singleShot(1000, this, SLOT(countdownTick()));

} // onDoLevelStartCountdown


void SurfaceGame::onMainTabChanged(const int iIndex) {
	Q_UNUSED(iIndex)

	//this->pauseIfRunning();

} // onMainTabChanged


void SurfaceGame::onMove() {

	static QVector<quint8> aStatesPickups = IconEngine::statesPickups();
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
			if (this->hpTeleporterExits.contains(ubState + 1u)) {

				pWorm->advanceTo(this->hpTeleporterExits.value(ubState + 1u));
				continue;

			} // if matching exit exists

		} // if not empty

		if (aStatesPickups.contains(ubState)) {

			// figure out which bonus it is

		} // if picked up something

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

		this->clearSurfaceOfWorm(pWorm->colourIndex());

		Q_EMIT this->wormCrashed(pWorm);

	} // loop

	// do AI-moves
	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWorm = this->apWorms.at(i);

		if (!pWorm->isAI()) continue;

		if (pWorm->isDead()) continue;

		this->findNextMovesForWorm(pWorm);

	} // loop worms

} // onMove


void SurfaceGame::onNextLevel() {

	if (0xFFu == this->ubCurrentLevel) this->ubCurrentLevel = 0u;
	else this->ubCurrentLevel++;

	this->loadCurrentLevel();

	// distribute spawn points. this could be done by Game

} // onNextLevel


void SurfaceGame::onPlaceBonus(const quint8 ubBonus) {

	// TODO:

} // onPlaceBonus


void SurfaceGame::onPlayerCountChanged(const quint8 ubCountHumans,
									   const quint8 ubCountAIs) {

	// TODO:

} // onPlayerCountChanged


void SurfaceGame::onPlayerKeyChanged(const quint8 ubWorm,
										   const QKeySequence &oKeySequence,
										   const L::Heading eHeading) {
	Q_UNUSED(ubWorm)
	Q_UNUSED(oKeySequence)
	Q_UNUSED(eHeading)

	this->initKeys();

} // onPlayerKeyChanged


void SurfaceGame::onPlayerRelativeChanged(const quint8 ubWorm,
										  const bool bRelative) {

	if (this->apWorms.length() <= ubWorm) return;

	this->apWorms.at(ubWorm)->setUseRelativeControls(bRelative);

} // onPlayerRelativeChanged


void SurfaceGame::onSpawnWorm(Worm *pWorm) {

	pWorm->startSpawning();

} // onSpawnWorm


void SurfaceGame::onSpawnWorms() {

	this->onDebugMessage("spawn worssssssssssss");

	quint8 ubTotal = this->apWorms.length();
	quint8 ubCount = 0u;
	for (; ubCount < ubTotal; ++ubCount) {

//		this->onSpawnWorm(ubCount);

	} // loop worms

} // onSpawnWorms


void SurfaceGame::pauseIfRunning() {

	// pause game if running
	if (this->pUi->buttonPP->isChecked())
		this->pUi->buttonPP->setChecked(false);

} // pauseIfRunning


void SurfaceGame::resetButtons() {

	this->bProtectPP = true;
	this->pUi->buttonPP->setChecked(false);
	this->pUi->buttonPP->setEnabled(true);
	this->pUi->buttonSR->setEnabled(true);
	this->pUi->buttonPP->setText(tr("Start"));
	this->bProtectPP = false;

} // resetButtons


void SurfaceGame::resizeEvent(QResizeEvent *pEvent) {

	QFrame::resizeEvent(pEvent);

	int iSBwidth = 200;
	int iDiff = this->window()->width() - this->width();

	for (int i = 0; i < this->apScoreBoards.length(); ++i) {

		if (iDiff >= 2 * iSBwidth) {
			// place score-boards left and right of field
			if (i & 1) this->pUi->frameScoreLeft->layout()->addWidget(this->apScoreBoards.at(i));
			else this->pUi->frameScoreRight->layout()->addWidget(this->apScoreBoards.at(i));
		} else if (iDiff >= iSBwidth) {
			// place score-boards on left of field
			this->pUi->frameScoreLeft->layout()->addWidget(this->apScoreBoards.at(i));
		} else {
			// move them to bottom
			this->pUi->frameScore->layout()->addWidget(this->apScoreBoards.at(i));
		} // switch spare space

	} // loop

} // resizeEvent


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


QSize SurfaceGame::sizeHint() const {

	return QFrame::sizeHint();

} // sizeHint



}	} // namespace SwissalpS::QtNibblers
