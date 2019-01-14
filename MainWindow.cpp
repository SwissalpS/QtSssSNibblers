#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Game.h"
#include "IconEngine.h"
#include "SurfaceBuilder.h"
#include "SurfaceGame.h"

#include <iostream>
#include <QStatusBar>



namespace SwissalpS { namespace QtNibblers {



MainWindow::MainWindow(QWidget *pParent) :
	QMainWindow(pParent),
	pUi(new Ui::MainWindow),
	pAS(AppSettings::pAppSettings()) {

	this->pUi->setupUi(this);

	connect(this->pAS, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	this->setGeometry(QRect(this->pAS->getWindowMainPosition(),
							this->pAS->getWindowMainSize()));

} // construct


MainWindow::~MainWindow() {

	delete this->pUi;

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

	connect(this, SIGNAL(quitting()),
			pSurface, SLOT(onQuitting()));

	connect(this, SIGNAL(settingsPlayerColoursChanged(QVector<quint8>)),
			pSurface, SLOT(onColoursChanged(QVector<quint8>)));

	connect(this, SIGNAL(settingsPlayerKeyChanged(quint8,QKeySequence,L::Heading)),
			pSurface, SLOT(onPlayerKeyChanged(quint8,QKeySequence,L::Heading)));

	connect(this, SIGNAL(settingsRelativeChanged(quint8,bool)),
			pSurface, SLOT(onPlayerRelativeChanged(quint8,bool)));


	connect(pSurface, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	connect(pSurface, SIGNAL(statusMessage(QString)),
			this, SLOT(onStatusMessage(QString)));


	Game *pGame = new Game(this);

	connect(this, SIGNAL(settingsPlayerCountChanged(quint8,quint8)),
			pGame, SLOT(onPlayerCountChanged(quint8,quint8)));

	connect(this, SIGNAL(settingsSpeedChanged(int)),
			pGame, SLOT(onSpeedChanged(int)));


	connect(pGame, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	connect(pGame, SIGNAL(statusMessage(QString)),
			this, SLOT(onStatusMessage(QString)));


	connect(pGame, SIGNAL(advanceWormTo(Worm *,QPoint)),
			pSurface, SLOT(onAdvanceWormTo(Worm *,QPoint)));

	connect(pGame, SIGNAL(bonusPlaced(QVector<QPoint>,quint8,bool)),
			pSurface, SLOT(onBonusPlaced(QVector<QPoint>,quint8,bool)));

	connect(pGame, SIGNAL(doGameOver()),
			pSurface, SLOT(onDoGameOver()));

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


	this->settingsUpdatePlayerColours();

	this->settingsUpdatePlayerMouseAndRelative();

	this->settingsUpdatePlayerKeys();

} // initSettings


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

	this->pUi->selectStartLevel->setCurrentIndex(255);

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

	//Q_EMIT this->settingsPlayerMouseChanged(ubWorm, bChecked);

} // onPlayerUseMouseToggled


void MainWindow::on_radioNetClient_toggled(bool bChecked) {

} // on_radioNetClient_toggled


void MainWindow::on_radioNetOff_toggled(bool bChecked) {

	if (!bChecked) return;

	// set setting to off
	// kill any connections

} // on_radioNetOff_toggled


void MainWindow::on_radioNetServer_toggled(bool bChecked) {

} // on_radioNetServer_toggled


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


void MainWindow::run() {

	// init tab views

	this->initSettings();

	this->initBuilder();

	this->initGame();

	//this->initScores();

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
