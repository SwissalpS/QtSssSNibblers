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
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Game.h"
#include "IconEngine.h"
#include "SurfaceBuilder.h"
#include "SurfaceGame.h"

#include <iostream>
#include <QDateTime>
#include <QStatusBar>



namespace SwissalpS { namespace QtNibblers {



MainWindow::MainWindow(QWidget *pParent) :
	QMainWindow(pParent),
	pUi(new Ui::MainWindow),
	pAS(AppSettings::pAppSettings()),
	pHistory(nullptr) {

	this->pUi->setupUi(this);

	connect(this->pAS, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	this->setGeometry(QRect(this->pAS->getWindowMainPosition(),
							this->pAS->getWindowMainSize()));

} // construct


MainWindow::~MainWindow() {

	delete this->pUi;

	this->pHistory->save();
	delete this->pHistory;

	this->pAS->sync();
	this->pAS = nullptr;
	AppSettings::drop();

} // dealloc


void MainWindow::changeEvent(QEvent *pEvent) {

	QMainWindow::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		default:
		break;

	} // switch type

} // changeEvent


void MainWindow::closeEvent(QCloseEvent *pEvent) {
	Q_UNUSED(pEvent)

	this->pAS->setWindowMainPosition(this->pos());
	this->pAS->setWindowMainSize(this->size());
	this->pAS->setValue(AppSettings::sSettingTabMainIndex,
						this->pUi->tabWidgetMain->currentIndex());
	this->pAS->setValue(AppSettings::sSettingTabSettingIndex,
						this->pUi->tabSettingsInner->currentIndex());

	Q_EMIT this->quitting();

	qApp->quit();

} // closeEvent


void MainWindow::initBuilder() {

	SurfaceBuilder *pBuilder = new SurfaceBuilder();
	this->pUi->tabBuilder->layout()->addWidget(pBuilder);

	connect(pBuilder, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	connect(pBuilder, SIGNAL(statusMessage(QString)),
			this, SLOT(onStatusMessage(QString)));

	pBuilder->init();

} // initBuilder


void MainWindow::initGame() {

	SurfaceGame *pSurface = new SurfaceGame();
	this->pUi->tabPlay->layout()->addWidget(pSurface);

	connect(this, SIGNAL(mainTabIndexChanged(int)),
			pSurface, SLOT(onMainTabChanged(int)));

	connect(this, SIGNAL(mouseButtonLeft()),
			pSurface, SLOT(onMouseLeft()));

	connect(this, SIGNAL(mouseButtonRight()),
			pSurface, SLOT(onMouseRight()));

	connect(this, SIGNAL(quitting()),
			pSurface, SLOT(onQuitting()));

	connect(this, SIGNAL(settingsPlayerColoursChanged(QVector<quint8>)),
			pSurface, SLOT(onColoursChanged(QVector<quint8>)));

	connect(this, SIGNAL(settingsPlayerKeyChanged(quint8,QKeySequence,L::Heading)),
			pSurface, SLOT(onPlayerKeyChanged(quint8,QKeySequence,L::Heading)));

	connect(this, SIGNAL(settingsPlayerUseMouseChanged()),
			pSurface, SLOT(onPlayerUseMouseChanged()));

	connect(this, SIGNAL(settingsRelativeChanged(quint8,bool)),
			pSurface, SLOT(onPlayerRelativeChanged(quint8,bool)));

	connect(this, SIGNAL(settingsTrailChanged(int)),
			pSurface, SLOT(onTrailChanged(int)));

	connect(pSurface, SIGNAL(statusMessage(QString)),
			this, SLOT(onStatusMessage(QString)));


	connect(pSurface, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));


	Game *pGame = new Game(this);

	connect(this, SIGNAL(settingsPlayerCountChanged(quint8,quint8)),
			pGame, SLOT(onPlayerCountChanged(quint8,quint8)));

	connect(this, SIGNAL(settingsPlayerNameChanged(quint8,QString)),
			pGame, SLOT(onPlayerNameChanged(quint8,QString)));

	connect(this, SIGNAL(settingsSpeedChanged(int)),
			pGame, SLOT(onSpeedChanged(int)));


	connect(pGame, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	connect(pGame, SIGNAL(updateHistory()),
			this, SLOT(onUpdateHistory()));

	connect(pGame, SIGNAL(statusMessage(QString)),
			this, SLOT(onStatusMessage(QString)));


	connect(pGame, SIGNAL(newHistoryItem(HistoryItem*)),
			this->pHistory, SLOT(addItem(HistoryItem*)));


	connect(pGame, SIGNAL(advanceWormTo(Worm *,QPoint)),
			pSurface, SLOT(onAdvanceWormTo(Worm *,QPoint)));

	connect(pGame, SIGNAL(bonusPlaced(QVector<QPoint>,quint8,bool)),
			pSurface, SLOT(onBonusPlaced(QVector<QPoint>,quint8,bool)));

	connect(pGame, SIGNAL(doGameOver(QString)),
			pSurface, SLOT(onDoGameOver(QString)));

	connect(pGame, SIGNAL(doLevelDone()),
			pSurface, SLOT(onDoLevelDone()));

	connect(pGame, SIGNAL(doLevelIsMissingSpawnPoints(quint8)),
			pSurface, SLOT(onDoLevelIsMissingSpawnPoints(quint8)));

	connect(pGame, SIGNAL(doLevelLoadError()),
			pSurface, SLOT(onDoLevelLoadError()));

	connect(pGame, SIGNAL(doLevelStartCountdown()),
			pSurface, SLOT(onDoLevelStartCountdown()));

	connect(pGame, SIGNAL(loadLevel(MapGame*,quint8)),
			pSurface, SLOT(onLoadLevel(MapGame*,quint8)));

	connect(pGame, SIGNAL(spawnWorm(Worm*)),
			pSurface, SLOT(onSpawnWorm(Worm*)));

	connect(pGame, SIGNAL(wormAteBonus(Worm *)),
			pSurface, SLOT(onWormAteBonus(Worm *)));

	connect(pGame, SIGNAL(wormCrashed(Worm *)),
			pSurface, SLOT(onWormCrashed(Worm *)));

	connect(pGame, SIGNAL(wormCreated(Worm *)),
			pSurface, SLOT(onWormCreated(Worm *)));

	connect(pGame, SIGNAL(wormsInvalidated()),
			pSurface, SLOT(onWormsInvalidated()));


	connect(pSurface, SIGNAL(bonusPlaced(QVector<SurfaceCell*>,bool)),
			pGame, SLOT(onBonusPlaced(QVector<SurfaceCell*>,bool)));

	connect(pSurface, SIGNAL(levelIsLoaded()),
			pGame, SLOT(onLevelIsLoaded()));

	connect(pSurface, SIGNAL(nextLevel()),
			pGame, SLOT(onNextLevel()));

	connect(pSurface, SIGNAL(pauseResumeToggled()),
			pGame, SLOT(onPauseResumeToggled()));

	connect(pSurface, SIGNAL(startNewGame(quint8)),
			pGame, SLOT(onStartNewGame(quint8)));

	connect(pSurface, SIGNAL(tileChanged(QPoint,quint8)),
			pGame, SLOT(onTileChanged(QPoint,quint8)));

	pSurface->init();
	pGame->init();

} // initGame


void MainWindow::initHistory() {

	QString sPath = this->pAS->getDataPath() + "History.json";
	this->pHistory = new History(sPath, this);

	connect(this->pHistory, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	if (!this->pHistory->isOK()) {

		this->onDebugMessage(tr("Could not load History"));
		return;

	} // if failed to load

	this->pUi->buttonClearHistory->setEnabled(
				this->pAS->get(AppSettings::sSettingHistoryEnableClearAll).toBool());

	this->onUpdateHistory();

} // initHistory


void MainWindow::initSettings() {

	quint8 ubAIs = quint8(this->pAS->get(AppSettings::sSettingGameCountAIs).toUInt());
	quint8 ubHumans = quint8(this->pAS->get(AppSettings::sSettingGameCountHumans).toUInt());

	if (0u == ubAIs) this->pUi->buttonAP0->setChecked(true);
	else if (1u == ubAIs) this->pUi->buttonAP1->setChecked(true);
	else if (2u == ubAIs) this->pUi->buttonAP2->setChecked(true);
	else if (3u == ubAIs) this->pUi->buttonAP3->setChecked(true);
	else if (4u == ubAIs) this->pUi->buttonAP4->setChecked(true);
	else if (5u == ubAIs) this->pUi->buttonAP5->setChecked(true);
	else if (6u == ubAIs) this->pUi->buttonAP6->setChecked(true);
	else if (7u == ubAIs) this->pUi->buttonAP7->setChecked(true);
	else this->pUi->buttonAP8->setChecked(true);

	if (0u == ubHumans) this->pUi->buttonHP0->setChecked(true);
	else if (1u == ubHumans) this->pUi->buttonHP1->setChecked(true);
	else if (2u == ubHumans) this->pUi->buttonHP2->setChecked(true);
	else if (3u == ubHumans) this->pUi->buttonHP3->setChecked(true);
	else this->pUi->buttonHP4->setChecked(true);

	if (this->pUi->buttonHP0->isChecked()) ubHumans = 0u;
	else if (this->pUi->buttonHP1->isChecked()) ubHumans = 1u;
	else if (this->pUi->buttonHP2->isChecked()) ubHumans = 2u;
	else if (this->pUi->buttonHP3->isChecked()) ubHumans = 3u;
	else if (this->pUi->buttonHP4->isChecked()) ubHumans = 4u;

	this->pUi->tabSettingsInner->setCurrentIndex(
				this->pAS->get(AppSettings::sSettingTabSettingIndex).toInt());

	this->pUi->cbFakeBonuses->setChecked(
				this->pAS->get(AppSettings::sSettingGameFakeBonuses).toBool());

	this->pUi->cbSound->setChecked(
				this->pAS->get(AppSettings::sSettingGameSound).toBool());

	this->pUi->cbGameOverOnLastDead->setChecked(
				this->pAS->get(AppSettings::sSettingGameOverOnLastDead).toBool());

	this->pUi->selectSpeed->setCurrentIndex(
				this->pAS->get(AppSettings::sSettingGameSpeed).toInt());

	QComboBox *pBox = this->pUi->selectStartLevel;
	QComboBox *pBoxLives = this->pUi->selectStartLives;
	for (quint16 i = 0; i < 256; ++i) {

		pBox->addItem(IconEngine::level(quint8(i)), QString::number(i));
		pBoxLives->addItem(QString::number(i));

	} // loop
	pBox->setCurrentIndex(
				this->pAS->get(AppSettings::sSettingGameStartLevel).toInt());

	this->pUi->cbLoadSetsStartLevel->setChecked(
				this->pAS->get(AppSettings::sSettingGameLoadSetsStartLevel).toBool());

	pBoxLives->setCurrentIndex(
				this->pAS->get(AppSettings::sSettingGameStartLives).toInt());

	// radios bad level reaction mode
	switch (this->pAS->get(AppSettings::sSettingGameBadLevelMode).toUInt()) {

		case 2u:this->pUi->radioBLskip->setChecked(true);
		break;

		case 1u: this->pUi->radioBLloop->setChecked(true);
		break;

		case 0u:
		default:
			this->pUi->radioBLwin->setChecked(true);

	} // switch this->pAS->get(AppSettings::sSettingGameBadLevelMode).toUInt()

	this->pUi->sliderDust->setValue(
				this->pAS->get(AppSettings::sSettingGameTrailLength).toUInt());

	this->settingsUpdatePlayerColours();

	this->settingsUpdatePlayerMouseAndRelative();

	this->settingsUpdatePlayerKeys();

	this->settingsUpdatePlayerNames();

} // initSettings


void MainWindow::mousePressEvent(QMouseEvent *pEvent) {

	if (Qt::LeftButton == pEvent->button()) {

		Q_EMIT this->mouseButtonLeft();

	} else if (Qt::RightButton == pEvent->button()) {

		Q_EMIT this->mouseButtonRight();

	} // if left or right button

	QMainWindow::mousePressEvent(pEvent);

} // mousePressEvent


void MainWindow::on_buttonAP0_clicked() {

	// 0 AI players -> up to 4 humans
	// nothing to do except:

	this->settingsUpdatePlayerCount();

} // on_buttonAP0_clicked


void MainWindow::on_buttonAP1_clicked() {

	// 1 AI player -> up to 4 humans
	// nothing to do except:

	this->settingsUpdatePlayerCount();

} // on_buttonAP1_clicked


void MainWindow::on_buttonAP2_clicked() {

	// 2 AI players -> up to 4 humans
	// nothing to do except:

	this->settingsUpdatePlayerCount();

} // on_buttonAP2_clicked


void MainWindow::on_buttonAP3_clicked() {

	// 3 AI players -> up to 4 humans
	// nothing to do except:

	this->settingsUpdatePlayerCount();

} // on_buttonAP3_clicked


void MainWindow::on_buttonAP4_clicked() {

	// 4 AI players -> up to 4 humans
	// nothing to do except:

	this->settingsUpdatePlayerCount();

} // on_buttonAP4_clicked


void MainWindow::on_buttonAP5_clicked() {

	// 5 AI players -> up to 3 humans
	if (this->pUi->buttonHP4->isChecked())
		this->pUi->buttonHP3->setChecked(true);

	this->settingsUpdatePlayerCount();

} // on_buttonAP5_clicked


void MainWindow::on_buttonAP6_clicked() {

	// 6 AI players -> up to 2 humans
	if (this->pUi->buttonHP4->isChecked()
			|| this->pUi->buttonHP3->isChecked())
		this->pUi->buttonHP2->setChecked(true);

	this->settingsUpdatePlayerCount();

} // on_buttonAP6_clicked


void MainWindow::on_buttonAP7_clicked() {

	// 7 AI players -> up to 1 human
	if (this->pUi->buttonHP4->isChecked()
			|| this->pUi->buttonHP3->isChecked()
			|| this->pUi->buttonHP2->isChecked())
		this->pUi->buttonHP1->setChecked(true);

	this->settingsUpdatePlayerCount();

} // on_buttonAP7_clicked


void MainWindow::on_buttonAP8_clicked() {

	// 8 AI players -> up to 0 humans
	if (this->pUi->buttonHP4->isChecked()
			|| this->pUi->buttonHP3->isChecked()
			|| this->pUi->buttonHP2->isChecked()
			|| this->pUi->buttonHP1->isChecked())
		this->pUi->buttonHP0->setChecked(true);

	this->settingsUpdatePlayerCount();

} // on_buttonAP8_clicked


void MainWindow::on_buttonClearHistory_clicked() {

	this->pHistory->clear();

	this->onUpdateHistory();

} // on_buttonClearHistory_clicked


void MainWindow::on_buttonClearHistorySelected_clicked() {

	//this->onDebugMessage("on_buttonClearHistorySelected_clicked");

	QTableWidget *pTable = this->pUi->tableScore;
	QList<QTableWidgetItem *> apList = pTable->selectedItems();
	if (0 == apList.length()) return;

	QTableWidgetItem *pItem;
	QTableWidgetItem *pItemIndex;
	QVector<int> aiIndexes;
	int iRow = -1;
	for (int i = 0; i < apList.length(); ++i) {

		pItem = apList.at(i);

		// we only need to look at one per row
		if (pItem->row() == iRow) continue;
		iRow = pItem->row();

		// get the original index
		pItemIndex = pTable->item(pItem->row(), 10);
		aiIndexes.append(pItemIndex->data(Qt::DisplayRole).toInt());

	} // loop all selected items

	this->pHistory->clear(aiIndexes);

	this->onUpdateHistory();

} // on_buttonClearHistorySelected_clicked


void MainWindow::on_buttonHP0_clicked() {

	// 0 human players -> up to 8 AIs
	// nothing to do except:

	this->settingsUpdatePlayerCount();

} // on_buttonHP0_clicked


void MainWindow::on_buttonHP1_clicked() {

	// 1 human player -> up to 7 AIs
	if (this->pUi->buttonAP8->isChecked())
		this->pUi->buttonAP7->setChecked(true);

	this->settingsUpdatePlayerCount();

} // on_buttonHP1_clicked


void MainWindow::on_buttonHP2_clicked() {

	// 2 human players -> up to 6 AIs
	if (this->pUi->buttonAP8->isChecked()
			|| this->pUi->buttonAP7->isChecked())
		this->pUi->buttonAP6->setChecked(true);

	this->settingsUpdatePlayerCount();

} // on_buttonHP2_clicked


void MainWindow::on_buttonHP3_clicked() {

	// 3 human players -> up to 5 AIs
	if (this->pUi->buttonAP8->isChecked()
			|| this->pUi->buttonAP7->isChecked()
			|| this->pUi->buttonAP6->isChecked())
		this->pUi->buttonAP5->setChecked(true);

	this->settingsUpdatePlayerCount();

} // on_buttonHP3_clicked


void MainWindow::on_buttonHP4_clicked() {

	// 4 human players -> up to 4 AIs
	if (this->pUi->buttonAP8->isChecked()
			|| this->pUi->buttonAP7->isChecked()
			|| this->pUi->buttonAP6->isChecked()
			|| this->pUi->buttonAP5->isChecked())
		this->pUi->buttonAP4->setChecked(true);

	this->settingsUpdatePlayerCount();

} // on_buttonHP4_clicked


void MainWindow::on_buttonZap0_clicked() {

	this->pUi->selectStartLevel->setCurrentIndex(0);

} // on_buttonZap0_clicked


void MainWindow::on_buttonZap128_clicked() {

	this->pUi->selectStartLevel->setCurrentIndex(128);

} // on_buttonZap128_clicked


void MainWindow::on_buttonZap255_clicked() {

	this->pUi->selectStartLevel->setCurrentIndex(225);

} // on_buttonZap255_clicked


void MainWindow::on_cbFakeBonuses_stateChanged(int iState) {

	this->pAS->setValue(AppSettings::sSettingGameFakeBonuses, 0 < iState);

} // on_cbFakeBonuses_stateChanged


void MainWindow::on_cbGameOverOnLastDead_stateChanged(int iState) {

	this->pAS->setValue(AppSettings::sSettingGameOverOnLastDead, 0 < iState);

} // on_cbGameOverOnLastDead_stateChanged


void MainWindow::on_cbLoadSetsStartLevel_stateChanged(int iState) {

	this->pAS->setValue(AppSettings::sSettingGameLoadSetsStartLevel, 0 < iState);

} // on_cbLoadSetsStartLevel_stateChanged


void MainWindow::on_cbLimitLives_toggled(bool bChecked) {

	//this->onDebugMessage("on_cbLimitLives_toggled");

	this->pAS->setValue(AppSettings::sSettingGameLimitLives, bChecked);

} // on_cbLimitLives_toggled


void MainWindow::on_cbRelative1_toggled(bool bChecked) {

	this->onPlayerRelativeToggled(0u, bChecked);

} // on_cbRelative1_toggled


void MainWindow::on_cbRelative2_toggled(bool bChecked) {

	this->onPlayerRelativeToggled(1u, bChecked);

} // on_cbRelative2_toggled


void MainWindow::on_cbRelative3_toggled(bool bChecked) {

	this->onPlayerRelativeToggled(2u, bChecked);

} // on_cbRelative3_toggled


void MainWindow::on_cbRelative4_toggled(bool bChecked) {

	this->onPlayerRelativeToggled(3u, bChecked);

} // on_cbRelative4_toggled


void MainWindow::on_cbSound_stateChanged(int iState) {

	this->pAS->setValue(AppSettings::sSettingGameSound, 0 < iState);

} // on_cbSound_stateChanged


void MainWindow::on_cbUseMouse1_toggled(bool bChecked) {

	this->onPlayerUseMouseToggled(0u, bChecked);

} // on_cbUseMouse1_toggled


void MainWindow::on_cbUseMouse2_toggled(bool bChecked) {

	this->onPlayerUseMouseToggled(1u, bChecked);

} // on_cbUseMouse2_toggled


void MainWindow::on_cbUseMouse3_toggled(bool bChecked) {

	this->onPlayerUseMouseToggled(2u, bChecked);

} // on_cbUseMouse3_toggled


void MainWindow::on_cbUseMouse4_toggled(bool bChecked) {

	this->onPlayerUseMouseToggled(3u, bChecked);

} // on_cbUseMouse4_toggled


void MainWindow::onDebugMessage(const QString &sMessage) const {

	QString sOut = "MW:" + sMessage;

	std::cout << sOut.toStdString() << std::endl;

	Q_EMIT this->debugMessage(sOut);

} // onDebugMessage


void MainWindow::on_kseDown1_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyDownChanged(0u, oKeySequence);

} // on_kseDown1_keySequenceChanged


void MainWindow::on_kseDown2_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyDownChanged(1u, oKeySequence);

} // on_kseDown2_keySequenceChanged


void MainWindow::on_kseDown3_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyDownChanged(2u, oKeySequence);

} // on_kseDown3_keySequenceChanged


void MainWindow::on_kseDown4_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyDownChanged(3u, oKeySequence);

} // on_kseDown4_keySequenceChanged


void MainWindow::on_kseLeft1_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyLeftChanged(0u, oKeySequence);

} // on_kseLeft1_keySequenceChanged


void MainWindow::on_kseLeft2_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyLeftChanged(1u, oKeySequence);

} // on_kseLeft2_keySequenceChanged


void MainWindow::on_kseLeft3_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyLeftChanged(2u, oKeySequence);

} // on_kseLeft3_keySequenceChanged


void MainWindow::on_kseLeft4_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyLeftChanged(3u, oKeySequence);

} // on_kseLeft4_keySequenceChanged


void MainWindow::on_kseRight1_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyRightChanged(0u, oKeySequence);

} // on_kseRight1_keySequenceChanged


void MainWindow::on_kseRight2_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyRightChanged(1u, oKeySequence);

} // on_kseRight2_keySequenceChanged


void MainWindow::on_kseRight3_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyRightChanged(2u, oKeySequence);

} // on_kseRight3_keySequenceChanged


void MainWindow::on_kseRight4_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyRightChanged(3u, oKeySequence);

} // on_kseRight4_keySequenceChanged


void MainWindow::on_kseUp1_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyUpChanged(0u, oKeySequence);

} // on_kseUp1_keySequenceChanged


void MainWindow::on_kseUp2_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyUpChanged(1u, oKeySequence);

} // on_kseUp2_keySequenceChanged


void MainWindow::on_kseUp3_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyUpChanged(2u, oKeySequence);

} // on_kseUp3_keySequenceChanged


void MainWindow::on_kseUp4_keySequenceChanged(const QKeySequence &oKeySequence) {

	this->onPlayerKeyUpChanged(3u, oKeySequence);

} // on_kseUp4_keySequenceChanged


void MainWindow::on_leName1_editingFinished() {

	this->onPlayerNameChanged(0u, this->pUi->leName1->text());

} // on_leName1_editingFinished


void MainWindow::on_leName2_editingFinished() {

	this->onPlayerNameChanged(1u, this->pUi->leName2->text());

} // on_leName2_editingFinished


void MainWindow::on_leName3_editingFinished() {

	this->onPlayerNameChanged(2u, this->pUi->leName3->text());

} // on_leName3_editingFinished


void MainWindow::on_leName4_editingFinished() {

	this->onPlayerNameChanged(3u, this->pUi->leName4->text());

} // on_leName4_editingFinished


void MainWindow::onPlayerColourChanged(const quint8 ubWorm, const quint8 ubIndex) {

	quint8 ubIndexOld = this->pAS->getPlayerColour(ubWorm);
	if (ubIndex == ubIndexOld) return;

	// find out which worm has the new colour so we can give it
	quint8 ubWormOldHolder = this->pAS->getPlayerByColour(ubIndex);

	this->pAS->setPlayerColour(ubWorm, ubIndex);
	this->pAS->setPlayerColour(ubWormOldHolder, ubIndexOld);

	QVector<quint8> aubColours;
	for (quint8 ubCount = 0u; ubCount < 8u; ++ubCount)
		aubColours.append(this->pAS->getPlayerColour(ubCount));

	Q_EMIT this->settingsPlayerColoursChanged(aubColours);

	this->settingsUpdatePlayerColours();

} // onPlayerColourChanged


void MainWindow::onPlayerKeyDownChanged(const quint8 ubWorm,
										const QKeySequence &oKeySequence) {

	QString sOld = this->pAS->getPlayerKeyDown(ubWorm);
	if (0 == sOld.compare(oKeySequence.toString())) return;

	// TODO: check for conflicts

	this->pAS->setPlayerKeyDown(ubWorm, oKeySequence.toString());

	Q_EMIT this->settingsPlayerKeyChanged(ubWorm, oKeySequence, L::Down);

} // onPlayerDownChanged


void MainWindow::onPlayerKeyLeftChanged(const quint8 ubWorm,
										const QKeySequence &oKeySequence) {

	QString sOld = this->pAS->getPlayerKeyLeft(ubWorm);
	if (0 == sOld.compare(oKeySequence.toString())) return;

	// TODO: check for conflicts

	this->pAS->setPlayerKeyLeft(ubWorm, oKeySequence.toString());

	Q_EMIT this->settingsPlayerKeyChanged(ubWorm, oKeySequence, L::Left);

} // onPlayerLeftChanged


void MainWindow::onPlayerKeyRightChanged(const quint8 ubWorm,
										 const QKeySequence &oKeySequence) {

	QString sOld = this->pAS->getPlayerKeyRight(ubWorm);
	if (0 == sOld.compare(oKeySequence.toString())) return;

	// TODO: check for conflicts

	this->pAS->setPlayerKeyRight(ubWorm, oKeySequence.toString());

	Q_EMIT this->settingsPlayerKeyChanged(ubWorm, oKeySequence, L::Right);

} // onPlayerRightChanged


void MainWindow::onPlayerKeyUpChanged(const quint8 ubWorm,
									  const QKeySequence &oKeySequence) {

	QString sOld = this->pAS->getPlayerKeyUp(ubWorm);
	if (0 == sOld.compare(oKeySequence.toString())) return;

	// TODO: check for conflicts

	this->pAS->setPlayerKeyUp(ubWorm, oKeySequence.toString());

	Q_EMIT this->settingsPlayerKeyChanged(ubWorm, oKeySequence, L::Up);

} // onPlayerUpChanged


void MainWindow::onPlayerNameChanged(const quint8 ubWorm, const QString sName) {

	QString sOld = this->pAS->getPlayerName(ubWorm);
	if (0 == sOld.compare(sName)) return;

	this->pAS->setPlayerName(ubWorm, sName);

	Q_EMIT this->settingsPlayerNameChanged(ubWorm, sName);

} // onPlayerNameChanged


void MainWindow::onPlayerRelativeToggled(const quint8 ubWorm, const bool bChecked) {

	bool bOld = this->pAS->getPlayerRelative(ubWorm);
	if (bOld == bChecked) return;

	this->pAS->setPlayerRelative(ubWorm, bChecked);

	Q_EMIT this->settingsRelativeChanged(ubWorm, bChecked);

} // onPlayerRelativeToggled


void MainWindow::onPlayerUseMouseToggled(const quint8 ubWorm, const bool bChecked) {

	bool bOld = this->pAS->getPlayerUseMouse(ubWorm);
	if (bOld == bChecked) return;

	if (bChecked) {
		// checked -> uncheck any others
		quint8 ubCount;
		for (ubCount = 0u; ubCount < 4u; ++ubCount) {

			if (ubCount == ubWorm) this->pAS->setPlayerUseMouse(ubWorm, bChecked);
			else this->pAS->setPlayerUseMouse(ubCount, false);

		} // loop

	} else {

		// unchecked -> just uncheck this one
		this->pAS->setPlayerUseMouse(ubWorm, bChecked);

	} // if checked or not

	this->settingsUpdatePlayerMouseAndRelative();

	Q_EMIT this->settingsPlayerUseMouseChanged();

} // onPlayerUseMouseToggled


void MainWindow::on_radioBLloop_toggled(bool bChecked) {

	if (!bChecked) return;

	//this->onDebugMessage("on_radioBLloop_toggled");

	this->pAS->setValue(AppSettings::sSettingGameBadLevelMode, 1u);

} // on_radioBLloop_toggled


void MainWindow::on_radioBLskip_toggled(bool bChecked) {

	if (!bChecked) return;

	//this->onDebugMessage("on_radioBLskip_toggled");

	this->pAS->setValue(AppSettings::sSettingGameBadLevelMode, 2u);

} // on_radioBLskip_toggled


void MainWindow::on_radioBLwin_toggled(bool bChecked) {

	if (!bChecked) return;

	//this->onDebugMessage("on_radioBLwin_toggled");

	this->pAS->setValue(AppSettings::sSettingGameBadLevelMode, 0u);

} // on_radioBLwin_toggled


void MainWindow::on_sliderDust_valueChanged(int iValue) {

	this->onDebugMessage("on_sliderDust_valueChanged " + QString::number(iValue));

	if (iValue == this->pAS->get(AppSettings::sSettingGameTrailLength).toUInt())
		return;

	// actually changed
	this->pAS->setValue(AppSettings::sSettingGameTrailLength, iValue);

	Q_EMIT this->settingsTrailChanged(iValue);

} // on_sliderDust_valueChanged


void MainWindow::on_selectColour1_currentIndexChanged(int iIndex) {

	this->onPlayerColourChanged(0u, quint8(iIndex));

} // on_selectColour1_currentIndexChanged


void MainWindow::on_selectColour2_currentIndexChanged(int iIndex) {

	this->onPlayerColourChanged(1u, quint8(iIndex));

} // on_selectColour2_currentIndexChanged


void MainWindow::on_selectColour3_currentIndexChanged(int iIndex) {

	this->onPlayerColourChanged(2u, quint8(iIndex));

} // on_selectColour3_currentIndexChanged


void MainWindow::on_selectColour4_currentIndexChanged(int iIndex) {

	this->onPlayerColourChanged(3u, quint8(iIndex));

} // on_selectColour4_currentIndexChanged


void MainWindow::on_selectColour5_currentIndexChanged(int iIndex) {

	this->onPlayerColourChanged(4u, quint8(iIndex));

} // on_selectColour5_currentIndexChanged


void MainWindow::on_selectColour6_currentIndexChanged(int iIndex) {

	this->onPlayerColourChanged(5u, quint8(iIndex));

} // on_selectColour6_currentIndexChanged


void MainWindow::on_selectColour7_currentIndexChanged(int iIndex) {

	this->onPlayerColourChanged(6u, quint8(iIndex));

} // on_selectColour7_currentIndexChanged


void MainWindow::on_selectColour8_currentIndexChanged(int iIndex) {

	this->onPlayerColourChanged(7u, quint8(iIndex));

} // on_selectColour8_currentIndexChanged


void MainWindow::on_selectSpeed_currentIndexChanged(int iIndex) {

	this->pAS->setValue(AppSettings::sSettingGameSpeed, iIndex);

	Q_EMIT this->settingsSpeedChanged(iIndex);

} // on_selectSpeed_currentIndexChanged


void MainWindow::on_selectStartLevel_currentIndexChanged(int iIndex) {

	// avoid while items are being added
	if (256 > this->pUi->selectStartLevel->count()) return;

	this->pAS->setValue(AppSettings::sSettingGameStartLevel, iIndex);

} // on_selectStartLevel_currentIndexChanged


void MainWindow::on_selectStartLives_currentIndexChanged(int iIndex) {

	// avoid while items are being added
	if (256 > this->pUi->selectStartLives->count()) return;

	this->pAS->setValue(AppSettings::sSettingGameStartLives, iIndex);

} // on_selectStartLives_currentIndexChanged


void MainWindow::onStatusMessage(const QString &sMessage) const {

	this->pUi->statusBar->showMessage(sMessage);
	this->pUi->statusBar->setToolTip(sMessage);

} // onStatusMessage


void MainWindow::on_tabWidgetMain_currentChanged(int iIndex) {

	Q_EMIT this->mainTabIndexChanged(iIndex);

	QString sMessage;
	switch (iIndex) {

		case 0: sMessage = tr("Settings: Set movement keys and more."); break;
		case 1: sMessage = tr("Builder: Edit levels and make your own."); break;
		case 2: sMessage = tr("Click on the buttons at the top to Start/Resume/Stop/Restart the game"); break;
		case 3: sMessage = tr("Scores: Remember that time..."); break;

		default:
		break;

	} // switch iIndex

	this->onStatusMessage(sMessage);

} // on_tabWidgetMain_currentChanged


void MainWindow::onUpdateHistory() {

	QVector<HistoryItem *> apHIs = this->pHistory->items();
	int iTotal = apHIs.length();

	QTableWidget *pTable = this->pUi->tableScore;
	pTable->clear();

	static QString sNo;
	static QString sYes;
	static QStringList aLabelsH;
	if (aLabelsH.isEmpty()) {

		aLabelsH << tr("Points") << tr("Name") << tr("First Level")
				<< tr("Levels Completed") << tr("Speed") << tr("Fakes")
				<< tr("Health Lost") << tr("#Humans") << tr("#AI") << tr("Date");

		sNo = tr("No");
		sYes = tr("Yes");

	} // if first call

	// set titles
	pTable->setColumnCount(aLabelsH.length() + 1);
	pTable->setHorizontalHeaderLabels(aLabelsH);
	pTable->setRowCount(iTotal);

	// no need for more
	if (0 >= iTotal) return;

	// number the vertical headers
	QStringList aLabelsV;
	for (int i = 0; i < iTotal; ++i) {

		aLabelsV.append(QString::number(i + 1));

	} // loop
	pTable->setVerticalHeaderLabels(aLabelsV);

	// disable sorting while adding items
	pTable->setSortingEnabled(false);

	QString sSpeed;
	HistoryItem *pHI;
	QTableWidgetItem *pTWI;
	for (int i = 0; i < iTotal; ++i) {

		pHI = apHIs.at(i);

		pTWI = new QTableWidgetItem(QString::number(pHI->score() * 1.01f), 10);
		pTWI->setData(Qt::DisplayRole, pHI->score());
		pTable->setItem(i, 0, pTWI);

		pTWI = new QTableWidgetItem(pHI->name(), 11);
		pTable->setItem(i, 1, pTWI);

		pTWI = new QTableWidgetItem(QString::number(pHI->levelStart() * 1.1f), 12);
		pTWI->setData(Qt::DisplayRole, pHI->levelStart());
		pTable->setItem(i, 2, pTWI);

		pTWI = new QTableWidgetItem(QString::number(pHI->levelsDone() * 1.1f), 13);
		pTWI->setData(Qt::DisplayRole, pHI->levelsDone());
		pTable->setItem(i, 3, pTWI);

		pTWI = new QTableWidgetItem(QString::number(pHI->speedIndex() * 1.01f), 14);
		switch (pHI->speedIndex()) {
			case 0: sSpeed = tr("Beginner"); break;
			case 1: sSpeed = tr("Slow"); break;
			case 2: sSpeed = tr("Medium"); break;
			case 3: sSpeed = tr("Fast"); break;
			case 4:
			default:
				sSpeed = tr("Full Speed");
			break;
		} // switch pHI->speedIndex()
		pTWI->setData(Qt::DisplayRole, QString::number(pHI->speedIndex()) + " " + sSpeed);
		pTable->setItem(i, 4, pTWI);

		pTWI = new QTableWidgetItem((pHI->fakes() ? sYes : sNo), 15);
		//pTWI->setData(Qt::DisplayRole, pHI->fakes());
		pTable->setItem(i, 5, pTWI);

		pTWI = new QTableWidgetItem(QString::number(pHI->livesLost() * 1.1f), 16);
		pTWI->setData(Qt::DisplayRole, pHI->livesLost());
		pTable->setItem(i, 6, pTWI);

		pTWI = new QTableWidgetItem(QString::number(pHI->countHuman() * 1.1f), 17);
		pTWI->setData(Qt::DisplayRole, pHI->countHuman());
		pTable->setItem(i, 7, pTWI);

		pTWI = new QTableWidgetItem(QString::number(pHI->countAI() * 1.1f), 18);
		pTWI->setData(Qt::DisplayRole, pHI->countAI());
		pTable->setItem(i, 8, pTWI);

		pTWI = new QTableWidgetItem(
				   QDateTime::fromSecsSinceEpoch(
					   pHI->timeStamp()).toString("yyyy.MM.dd_HH:mm"), 19);
		pTable->setItem(i, 9, pTWI);

		pTWI = new QTableWidgetItem(QString::number(i * 1.1f), 20);
		pTWI->setData(Qt::DisplayRole, i);
		pTable->setItem(i, 10, pTWI);

	} // loop making items

	// collect up to 4 most recent entries
	quint8 ubCountMostRecent = 0u;
	qint64 illMostRecent = apHIs.last()->timeStamp();
	quint32 ulHighest = 0;
	int iHighestIndex = iTotal - 1;
	for (int i = iTotal - 1; i > iTotal - 5; --i) {

		pHI = apHIs.at(i);
		if (pHI->timeStamp() == illMostRecent) {

			ubCountMostRecent++;

			if (pHI->score() > ulHighest) {

				ulHighest = pHI->score();
				iHighestIndex = i;

			} // if highscore found

		} else break;

	} // loop most recent

	// we can select on easily
	pTable->selectRow(iHighestIndex);
	for (int i = iTotal - 1; i > iTotal - 1 - ubCountMostRecent; --i) {

		if (0 == i - iHighestIndex) continue;

		pTable->item(i, 0)->setSelected(true);
		pTable->item(i, 1)->setSelected(true);
		pTable->item(i, 3)->setSelected(true);
		pTable->item(i, 6)->setSelected(true);
		pTable->item(i, 9)->setSelected(true);

	} // loop

	pTable->setSortingEnabled(true);
	pTable->sortItems(0, Qt::DescendingOrder);
	pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	pTable->hideColumn(10);

} // onUpdateHistory


void MainWindow::run() {

	// init tab views

	this->initSettings();

	this->initBuilder();

	//this->initScores();
	this->initHistory();

	// init game and surface (view)
	this->initGame();

	// init about and help
	this->pUi->textBrowserAbout->setSource(QUrl::fromLocalFile(":/html/About/About.html"));
	this->pUi->textBrowserHelp->setSource(QUrl::fromLocalFile(":/html/Help/Help.html"));

	// bring last used to front
	int iLastTabIndex = this->pAS->get(AppSettings::sSettingTabMainIndex).toInt();
	this->pUi->tabWidgetMain->setCurrentIndex(iLastTabIndex);

} // run


void MainWindow::settingsUpdatePlayerColours() {

	this->pUi->selectColour1->setCurrentIndex(this->pAS->getPlayerColour(0u));
	this->pUi->selectColour2->setCurrentIndex(this->pAS->getPlayerColour(1u));
	this->pUi->selectColour3->setCurrentIndex(this->pAS->getPlayerColour(2u));
	this->pUi->selectColour4->setCurrentIndex(this->pAS->getPlayerColour(3u));
	this->pUi->selectColour5->setCurrentIndex(this->pAS->getPlayerColour(4u));
	this->pUi->selectColour6->setCurrentIndex(this->pAS->getPlayerColour(5u));
	this->pUi->selectColour7->setCurrentIndex(this->pAS->getPlayerColour(6u));
	this->pUi->selectColour8->setCurrentIndex(this->pAS->getPlayerColour(7u));

} // settingsUpdatePlayerColours


void MainWindow::settingsUpdatePlayerCount() {

	quint8 ubAIs = 0u;
	quint8 ubHumans = 0u;

	if (this->pUi->buttonAP0->isChecked()) ubAIs = 0u;
	else if (this->pUi->buttonAP1->isChecked()) ubAIs = 1u;
	else if (this->pUi->buttonAP2->isChecked()) ubAIs = 2u;
	else if (this->pUi->buttonAP3->isChecked()) ubAIs = 3u;
	else if (this->pUi->buttonAP4->isChecked()) ubAIs = 4u;
	else if (this->pUi->buttonAP5->isChecked()) ubAIs = 5u;
	else if (this->pUi->buttonAP6->isChecked()) ubAIs = 6u;
	else if (this->pUi->buttonAP7->isChecked()) ubAIs = 7u;
	else if (this->pUi->buttonAP8->isChecked()) ubAIs = 8u;

	if (this->pUi->buttonHP0->isChecked()) ubHumans = 0u;
	else if (this->pUi->buttonHP1->isChecked()) ubHumans = 1u;
	else if (this->pUi->buttonHP2->isChecked()) ubHumans = 2u;
	else if (this->pUi->buttonHP3->isChecked()) ubHumans = 3u;
	else if (this->pUi->buttonHP4->isChecked()) ubHumans = 4u;

	this->pAS->setValue(AppSettings::sSettingGameCountAIs, ubAIs);
	this->pAS->setValue(AppSettings::sSettingGameCountHumans, ubHumans);

	Q_EMIT this->settingsPlayerCountChanged(ubHumans, ubAIs);

} // settingsUpdatePlayerCount


void MainWindow::settingsUpdatePlayerKeys() {

	this->pUi->kseDown1->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyDown(0u),
											QKeySequence::PortableText));
	this->pUi->kseDown2->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyDown(1u),
											QKeySequence::PortableText));
	this->pUi->kseDown3->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyDown(2u),
											QKeySequence::PortableText));
	this->pUi->kseDown4->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyDown(3u),
											QKeySequence::PortableText));

	this->pUi->kseLeft1->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyLeft(0u),
											QKeySequence::PortableText));
	this->pUi->kseLeft2->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyLeft(1u),
											QKeySequence::PortableText));
	this->pUi->kseLeft3->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyLeft(2u),
											QKeySequence::PortableText));
	this->pUi->kseLeft4->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyLeft(3u),
											QKeySequence::PortableText));

	this->pUi->kseRight1->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyRight(0u),
											QKeySequence::PortableText));
	this->pUi->kseRight2->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyRight(1u),
											QKeySequence::PortableText));
	this->pUi->kseRight3->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyRight(2u),
											QKeySequence::PortableText));
	this->pUi->kseRight4->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyRight(3u),
											QKeySequence::PortableText));

	this->pUi->kseUp1->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyUp(0u),
											QKeySequence::PortableText));
	this->pUi->kseUp2->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyUp(1u),
											QKeySequence::PortableText));
	this->pUi->kseUp3->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyUp(2u),
											QKeySequence::PortableText));
	this->pUi->kseUp4->setKeySequence(QKeySequence(
											this->pAS->getPlayerKeyUp(3u),
											QKeySequence::PortableText));

} // settingsUpdatePlayerKeys


void MainWindow::settingsUpdatePlayerNames() {

	QStringList aNames = this->pAS->get(AppSettings::sSettingGameNames).toStringList();
	if (4 > aNames.length()) return;

	this->pUi->leName1->setText(aNames.at(0));
	this->pUi->leName2->setText(aNames.at(1));
	this->pUi->leName3->setText(aNames.at(2));
	this->pUi->leName4->setText(aNames.at(3));

} // settingsUpdatePlayerNames


void MainWindow::settingsUpdatePlayerMouseAndRelative() {

	this->pUi->cbUseMouse1->setChecked(this->pAS->getPlayerUseMouse(0u));
	this->pUi->cbUseMouse2->setChecked(this->pAS->getPlayerUseMouse(1u));
	this->pUi->cbUseMouse3->setChecked(this->pAS->getPlayerUseMouse(2u));
	this->pUi->cbUseMouse4->setChecked(this->pAS->getPlayerUseMouse(3u));

	this->pUi->cbRelative1->setChecked(this->pAS->getPlayerRelative(0u));
	this->pUi->cbRelative2->setChecked(this->pAS->getPlayerRelative(1u));
	this->pUi->cbRelative3->setChecked(this->pAS->getPlayerRelative(2u));
	this->pUi->cbRelative4->setChecked(this->pAS->getPlayerRelative(3u));

} // settingsUpdatePlayerMouseAndRelative



}	} // namespace SwissalpS::QtNibblers
