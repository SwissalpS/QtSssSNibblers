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
#include "SurfaceGame.h"
#include "ui_SurfaceGame.h"

#include "definitions.h"
#include "IconEngine.h"

#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>



namespace SwissalpS { namespace QtNibblers {



SurfaceGame::SurfaceGame(QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::SurfaceGame),
	pTimerResize(nullptr),
	ubResizeCount(0u),
	bLevelDone(false),
	bLevelLoading(true),
	bProtectPP(false),
	pAS(AppSettings::pAppSettings()),
	pDialogLoad(nullptr),
	pStartCountDownFrame(nullptr),
	ibWormMouse(-1),
	ubCurrentLevel(0xFFu) {

	this->pUi->setupUi(this);

	// for key events
	setFocusPolicy(Qt::StrongFocus);

	this->ahKeys.clear();
	this->aopRows.clear();
	this->apScoreBoards.clear();
	this->apWorms.clear();

	this->pTimerResize = new QTimer(this);
	this->pTimerResize->setInterval(100);
	this->pTimerResize->setSingleShot(true);

	connect(this->pTimerResize, SIGNAL(timeout()),
			this, SLOT(resizeDelayDone()));

} // construct


SurfaceGame::~SurfaceGame() {

	this->pAS = nullptr;

	this->ahKeys.clear();
	this->aopRows.clear();
	this->apScoreBoards.clear();
	this->apWorms.clear();

	if (this->pTimerResize) {
		this->pTimerResize->stop();
		delete this->pTimerResize;
		this->pTimerResize = nullptr;
	}

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


// set all to 0
void SurfaceGame::clearSurface() {

	quint8 ubRows = quint8(this->aopRows.count());
	quint8 ubColumns = quint8(this->aopRows.first().count());
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aopRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			//pCell = this->aopRows.at(ubY).at(ubX);
			pCell = aRow.at(ubX);
			this->setCellState(pCell, L::FloorClean, false);
			pCell->freezeState();

		} // loop columns

	} // loop rows

	this->update();

} // clearSurface


// set any of ubState to their frozen state (original state on load)
void SurfaceGame::clearSurfaceOf(const quint8 ubState) {

	QVector<quint8> aStates;
	aStates.append(ubState);

	this->clearSurfaceOf(aStates);

} // clearSurfaceOf(single)


void SurfaceGame::clearSurfaceOf(const QVector<quint8> aStates) {

	quint8 ubRows = quint8(this->aopRows.count());
	quint8 ubColumns = quint8(this->aopRows.first().count());
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aopRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pCell = aRow.at(ubX);

			if (aStates.contains(pCell->getState())) {

				pCell->defrostState();

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

	//this->onDebugMessage("countdownTick");

	int iTick = this->pUi->buttonPP->text().toInt() - 1;

	if (0 < iTick) {

		QString sMessage = QString::number(iTick);

		this->showStartCountDownFrame(sMessage);

		this->pUi->buttonPP->setText(sMessage);

		Q_EMIT this->statusMessage(sMessage);

		QTimer::singleShot(1000, this, SLOT(countdownTick()));

		return;

	} // if still got ticks to go

	this->pStartCountDownFrame->hide();

	this->bProtectPP = true;
	this->pUi->buttonPP->setChecked(true);
	this->pUi->buttonPP->setEnabled(true);

	this->pUi->buttonSR->setEnabled(true);
	this->pUi->buttonPP->setText(tr("Pause"));
	this->bProtectPP = false;

	Q_EMIT this->statusMessage(tr("Go, Go, Goooooh!"));

	//this->onDebugMessage("countdown over!! onPRT--->)))");
	Q_EMIT this->pauseResumeToggled();

} // countdownTick


Map *SurfaceGame::currentMap() {

	quint8 ubRows = quint8(this->aopRows.count());
	quint8 ubColumns = quint8(this->aopRows.first().count());
	quint8 ubX = 0u;
	quint8 ubY = 0u;

	Map *pMap = new Map(ubColumns, ubRows, this);

	for (ubY = 0u; ubY < ubRows; ubY++) {

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pMap->setTile(ubX, ubY, this->aopRows.at(ubY).at(ubX)->getState());

		} // loop columns

	} // loop rows

	return pMap;

} // currentMap


void SurfaceGame::dialogLoadFinished(const int iResult) {

	// canceled
	if (0 == iResult) return;

	this->onDebugMessage("dialogLoadFinished");

	this->resetButtons();
	this->bLevelDone = false;

	int iIndex = this->pDialogLoad->getSelected();

	if (this->pAS->get(AppSettings::sSettingGameLoadSetsStartLevel).toBool())
		this->pAS->setValue(AppSettings::sSettingGameStartLevel, iIndex);

	Q_EMIT this->startNewGame(quint8(iIndex));

} // dialogLoadFinished


void SurfaceGame::focusInEvent(QFocusEvent *pEvent) {

	//this->onDebugMessage("focusInEvent");

	if (nullptr != this->pStartCountDownFrame
			&& this->pStartCountDownFrame->isVisible()) {

		this->updateFrameStartCountdown();

	}

	QFrame::focusInEvent(pEvent);

} // focusInEvent


void SurfaceGame::focusOutEvent(QFocusEvent *pEvent) {

	//this->onDebugMessage("focusOutEvent " + QString::number(pEvent->reason()) + " bPPfoc " + QString::number(this->pUi->buttonPP->hasFocus()) + " SCF vis " + QString::number(this->pStartCountDownFrame->isVisible()));

	if (this->pUi->buttonPP->hasFocus()) return;

	if (this->pStartCountDownFrame->isVisible()) {

		if (Qt::TabFocusReason == pEvent->reason()) this->window()->raise();
		return;
	}

	this->pauseIfRunning();

	QFrame::focusOutEvent(pEvent);

} // focusOutEvent


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

	this->initKeys();

	this->resetButtons();

} // init


void SurfaceGame::initCells() {

	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	quint8 ubState = L::FloorClean;
	quint16 uiTrail = this->pAS->get(AppSettings::sSettingGameTrailLength).toUInt();
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

			connect(this, SIGNAL(trailChanged(int)),
					pCell, SLOT(onTrailChanged(int)));

			connect(pCell, SIGNAL(debugMessage(QString)),
					this, SLOT(onDebugMessage(QString)));

			connect(pCell, SIGNAL(changed(QPoint,quint8)),
					this, SLOT(onCellChanged(QPoint,quint8)));

			pCell->setCursor(Qt::BlankCursor);

			pCell->onTrailChanged(uiTrail);

		} // loop columns

		this->aopRows.append(aRow);
		pVBox->addItem(pHBox);

	} // loop rows

	this->pUi->frameSurface->setLayout(pVBox);

} // initCells


void SurfaceGame::initKeys() {

	//this->onDebugMessage("initKeys");

	this->ibWormMouse = -1;
	this->ahKeys.clear();

	QHash<QKeySequence, L::Heading> hKeys;
	QKeySequence oKS;

	for (quint8 ubWorm = 0u; ubWorm < 4; ++ubWorm) {

		hKeys.clear();

		if (this->pAS->getPlayerUseMouse(ubWorm)) this->ibWormMouse = ubWorm;

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


void SurfaceGame::keyPressEvent(QKeyEvent *pEvent) {

	//this->onDebugMessage("keyPressEvent");

	QKeySequence oKSin(pEvent->key());

	quint8 ubWorm;
	Worm *pWorm;
	QHash<QKeySequence, L::Heading> hKeys;
	bool bNoMatchFound = true;

	for (ubWorm = 0u; ubWorm < this->ahKeys.length(); ++ubWorm) {

		hKeys = this->ahKeys.at(ubWorm);
		if (!hKeys.contains(oKSin)) continue;

		bNoMatchFound = false;

		if (this->apWorms.length() > ubWorm) {

			pWorm = this->apWorms.at(ubWorm);
			if (!pWorm->isAI()) pWorm->onTurn(hKeys.value(oKSin));

		} // if valid index

	} // loop each set of keys

	if (bNoMatchFound) QFrame::keyPressEvent(pEvent);

} // keyPressEvent


void SurfaceGame::mousePressEvent(QMouseEvent *pEvent) {

	if (Qt::LeftButton == pEvent->button()) {

		this->onMouseLeft();

	} else if (Qt::RightButton == pEvent->button()) {

		this->onMouseRight();

	} // if left or right button

	else QFrame::mousePressEvent(pEvent);

} // mousePressEvent


void SurfaceGame::onAdvanceWormTo(Worm *pWorm, const QPoint oPoint) {

	pWorm->advanceTo(this->getCell(oPoint));

} // onAdvanceWormTo


void SurfaceGame::onBonusPlaced(const QVector<QPoint> aoPoints,
								const quint8 ubBonus, const bool bFake) {

	//this->onDebugMessage("onBonusPlaced");
	//this->onDebugMessage(QString::number(ubBonus) + " " + QString::number(aoPoints.at(0).x()) + ":" + QString::number(aoPoints.at(0).y()) + " " + QString::number(bFake));

	SurfaceCell *pCell = this->getCell(aoPoints.first());
	SurfaceCell *pCell1 = this->getCell(aoPoints.at(1));
	SurfaceCell *pCell2 = this->getCell(aoPoints.at(2));
	SurfaceCell *pCell3 = this->getCell(aoPoints.last());

	this->setCellState(pCell, ubBonus);
	this->setCellState(pCell1, ubBonus + 1u);
	this->setCellState(pCell2, ubBonus + 2u);
	this->setCellState(pCell3, ubBonus + 3u);

	QVector<SurfaceCell *> apCells;
	apCells.clear();
	apCells.append(pCell);
	apCells.append(pCell1);
	apCells.append(pCell2);
	apCells.append(pCell3);

	Q_EMIT this->bonusPlaced(apCells, bFake);

} // onBonusPlaced


void SurfaceGame::on_buttonPP_toggled(bool bStartPlaying) {

	this->pUi->buttonPP->setText(bStartPlaying ? tr("Pause") : tr("Play"));

	if (this->bProtectPP) return;

	//this->onDebugMessage("bPP startPlaying? " + QString::number(bStartPlaying) + " protected? " + QString::number(this->bProtectPP));

	if (bStartPlaying) {

		// starting or resuming?

		// no matter hide cover frame
		if (this->pStartCountDownFrame) this->pStartCountDownFrame->hide();

		// and give us focus for keystrokes
		this->setFocus();

		if (this->bLevelDone) {

			Q_EMIT this->nextLevel();

			//return;

		} // if level complete -> starting new one

	} else {

		this->onDebugMessage("pausing");

		// going into paused state
		this->showStartCountDownFrame(tr("Paused"));

	} // starting/resuming or pausing

	//this->onDebugMessage("onPRT----->)))");
	Q_EMIT this->pauseResumeToggled();

} // on_buttonPP_toggled


void SurfaceGame::on_buttonSR_clicked() {

	this->onDebugMessage("on_buttonSR_clicked");

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


void SurfaceGame::onDoGameOver(const QString &sRanking) {

	this->onDebugMessage("onDoGameOver");

	// button is probably showing "Pause"
	if (this->pUi->buttonPP->isChecked()) {
		this->bProtectPP = true;
		this->pUi->buttonPP->setChecked(false);
		this->bProtectPP = false;
	}
	this->pUi->buttonPP->setEnabled(false);

	// show game over dialog
	QString sMessage = tr("Game Done. Start a new one by clicking the 'Load New Game' button in the upper left.");
	this->showStartCountDownFrame(sRanking);

	Q_EMIT this->statusMessage(sMessage);

} // onDoGameOver


void SurfaceGame::onDoLevelDone() {

	this->onDebugMessage("onDoLevelDone");

	this->bLevelDone = true;

	QString sButton = tr("Start Next Level");
	QString sMessage = tr("Level Done");
	QPushButton *pButton = this->pUi->buttonPP;
	// button is probably showing "Pause"
	if (pButton->isChecked()) {
		this->bProtectPP = true;
		pButton->setChecked(false);
		this->bProtectPP = false;
	}
	pButton->setEnabled(true);
	pButton->setText(sButton);

	// show level done dialog
	this->showStartCountDownFrame(sMessage, sButton);

	Q_EMIT this->statusMessage(sMessage);

} // onDoLevelDone


void SurfaceGame::onDoLevelIsMissingSpawnPoints(const quint8 ubMissing) {
	Q_UNUSED(ubMissing)

	this->onDebugMessage("onDoLevelIsMissingSpawnPoints");

	this->pUi->buttonPP->setEnabled(false);

	this->showStartCountDownFrame(tr("Level Is Missing Spawn Points\nCan't be played with this many worms."));

} // onDoLevelIsMissingSpawnPoints


void SurfaceGame::onDoLevelLoadError() {

	this->onDebugMessage("onDoLevelLoadError");

	this->pUi->buttonPP->setEnabled(false);

	this->showStartCountDownFrame(tr("Error Loading Level."));

} // onDoLevelLoadError


void SurfaceGame::onDoLevelStartCountdown() {

	this->onDebugMessage("onDoLevelStartCountdown");

	this->bLevelDone = false;

	// open count-down dialog
	QString sCount = QString::number(SssS_Nibblers_Game_Start_Countdown);
	this->showStartCountDownFrame(sCount);

	this->pUi->buttonPP->setText(sCount);
	this->pUi->buttonPP->setEnabled(false);
	this->pUi->buttonSR->setEnabled(false);

	Q_EMIT this->statusMessage(tr("Get Ready: ") + sCount + "...");

	QTimer::singleShot(1000, this, SLOT(countdownTick()));

} // onDoLevelStartCountdown


void SurfaceGame::onLoadLevel(MapGame *pMap, const quint8 ubLevel) {

	this->onDebugMessage("onLoadLevel");

	this->bLevelLoading = true;

	this->ubCurrentLevel = ubLevel;

	this->pUi->labelLevel->setText(tr("Level") + " " + QString::number(ubLevel));

	this->clearSurface();

	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	SurfaceCell *pCell;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = this->getCell(ubColumns, ubRows);

			this->setCellState(pCell, pMap->tile(ubColumns, ubRows), false);

			pCell->freezeState();

		} // loop columns

	} // loop rows

	this->update();

	this->bLevelLoading = false;

	Q_EMIT this->levelIsLoaded();

} // onLoadLevel


void SurfaceGame::onMainTabChanged(const int iIndex) {
	Q_UNUSED(iIndex)

	//this->pauseIfRunning();

} // onMainTabChanged


void SurfaceGame::onMouseLeft() {

	if (0 > this->ibWormMouse) return;
	if (this->ibWormMouse >= this->apWorms.length()) return;

	//this->onDebugMessage("onMouseLeft");

	this->apWorms.at(this->ibWormMouse)->onTurn(L::Left);

} // onMouseLeft


void SurfaceGame::onMouseRight() {

	if (0 > this->ibWormMouse) return;
	if (this->ibWormMouse >= this->apWorms.length()) return;

	//this->onDebugMessage("onMouseRight");

	this->apWorms.at(this->ibWormMouse)->onTurn(L::Right);

} // onMouseRight


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

	Worm *pWorm = this->apWorms.at(ubWorm);

	if (pWorm->isAI()) return;

	pWorm->setUseRelativeControls(bRelative);

} // onPlayerRelativeChanged


void SurfaceGame::onQuitting() {

	if (this->pStartCountDownFrame) this->pStartCountDownFrame->close();

} // onQuitting


void SurfaceGame::onSpawnWorm(Worm *pWorm) {

	//this->onDebugMessage("onSpawnWorm");

	pWorm->startSpawning();

	pWorm->advanceTo(this->getCell(pWorm->spawnPoint()));
	pWorm->advanceTo(this->getCell(pWorm->nextPoint()));
	pWorm->advanceTo(this->getCell(pWorm->nextPoint()));
	pWorm->advanceTo(this->getCell(pWorm->nextPoint()));
	pWorm->advanceTo(this->getCell(pWorm->nextPoint()));

} // onSpawnWorm


void SurfaceGame::onWormAteBonus(Worm *pWorm) {

	//this->onDebugMessage("onWormAteBonus");

	// mark bloatedness on worm
	pWorm->headCell()->addBloatedHeading(pWorm->currentDirection());

	// this should be dealt with by Worm automatically
	//pWorm->setNextBloatHeading(L::oppositeHeading(pWorm->currentDirection()));

} // onWormAteBonus


void SurfaceGame::onWormCrashed(Worm *pWorm) {

	this->clearSurfaceOfWorm(pWorm->colourIndex());

} // onWormCrashed


void SurfaceGame::onWormCreated(Worm *pWorm) {

	this->apWorms.append(pWorm);

	ScoreBoard *pSB = new ScoreBoard(pWorm->colourIndex(), this);
	this->pUi->frameScore->layout()->addWidget(pSB);
	this->apScoreBoards.append(pSB);

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

	this->ubResizeCount = 0u;
	this->resizeDelayDone();

} // onWormCreated


void SurfaceGame::onWormsInvalidated() {

	this->apWorms.clear();

	ScoreBoard *pSB;
	while (this->apScoreBoards.length()) {

		pSB = this->apScoreBoards.takeLast();
		this->pUi->frameScore->layout()->removeWidget(pSB);

		delete pSB;
		pSB = nullptr;

	} // loop

} // onWormsInvalidated


void SurfaceGame::pauseIfRunning() {

	this->onDebugMessage("pauseIfRunning");

	// pause game if running
	if (this->pUi->buttonPP->isChecked())
		this->pUi->buttonPP->setChecked(false);

} // pauseIfRunning


void SurfaceGame::resetButtons() {

	this->onDebugMessage("resetButtons");

	this->bProtectPP = true;
	this->pUi->buttonPP->setChecked(false);
	this->pUi->buttonPP->setEnabled(true);
	this->pUi->buttonSR->setEnabled(true);
	this->pUi->buttonPP->setText(tr("Start"));
	this->bProtectPP = false;

} // resetButtons


void SurfaceGame::resizeDelayDone() {

	//this->onDebugMessage("resizeDelayDone" + QString::number(qrand()));

	this->ubResizeCount++;
	if (2 < this->ubResizeCount) {

		this->ubResizeCount = 0u;
		return;

	} // if need to break a moment to avoid a jittering and a hanging

	int iSBwidth = 200;
	int iDiff = this->window()->width() - this->width();

	for (int i = 0; i < this->apScoreBoards.length(); ++i) {

		if (iDiff >= 2 * iSBwidth) {
			// place score-boards left and right of field
			if (i & 1) this->pUi->frameScoreRight->layout()->addWidget(this->apScoreBoards.at(i));
			else this->pUi->frameScoreLeft->layout()->addWidget(this->apScoreBoards.at(i));
		} else if (iDiff >= iSBwidth) {
			// place score-boards on left of field
			this->pUi->frameScoreLeft->layout()->addWidget(this->apScoreBoards.at(i));
		} else {
			// move them to bottom
			this->pUi->frameScore->layout()->addWidget(this->apScoreBoards.at(i));
		} // switch spare space

	} // loop

} // resizeDelayDone


void SurfaceGame::resizeEvent(QResizeEvent *pEvent) {

	QFrame::resizeEvent(pEvent);

	// throttle that we don't end up hanging
	this->pTimerResize->start();

} // resizeEvent


void SurfaceGame::setCellState(const QPoint oPoint, const quint8 ubState,
							   const bool bUpdate) {

	SurfaceCell *pCell = this->getCell(oPoint);
	if (pCell->isNull()) return;

	this->setCellState(pCell, ubState, bUpdate);

} // setCellState


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


void SurfaceGame::showStartCountDownFrame(const QString sMessage,
										  const QString sButton) {

	//this->onDebugMessage("showStartCountDownFrame");

	FrameStartCountdown *pFrame = this->pStartCountDownFrame;
	if (nullptr == pFrame) {

		pFrame = new FrameStartCountdown();
		pFrame->setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

		this->pStartCountDownFrame = pFrame;

		connect(pFrame, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		connect(pFrame, SIGNAL(done()),
				this, SLOT(onSCDFdone()));

	} // if first time

	pFrame->onSetText(sMessage, sButton);
	pFrame->show();
	this->updateFrameStartCountdown();

} // showStartCountDownFrame


QSize SurfaceGame::sizeHint() const {

	return QFrame::sizeHint();

} // sizeHint


void SurfaceGame::updateFrameStartCountdown() {

	FrameStartCountdown *pFrame = this->pStartCountDownFrame;
	if (nullptr == pFrame) return;
	if (pFrame->isHidden()) return;

	QRect oRect = this->pUi->frameSurface->geometry();
	pFrame->setGeometry(oRect);
	pFrame->move(mapToGlobal(QPoint(oRect.left(), oRect.top())));
	pFrame->show();
	pFrame->raise();
	pFrame->update();

} // updateFrameStartCountdown



}	} // namespace SwissalpS::QtNibblers
