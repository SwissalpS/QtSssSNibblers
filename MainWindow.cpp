#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "IconEngine.h"
#include "SurfaceBuilder.h"

#include <iostream>
#include <QStatusBar>



namespace SwissalpS { namespace QtNibblers {



MainWindow::MainWindow(QWidget *pParent) :
	QMainWindow(pParent),
	pUi(new Ui::MainWindow),
	pAS(AppSettings::pAppSettings()) {

	this->pUi->setupUi(this);

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
						this->pUi->tabWidget->currentIndex());
	this->pAS->setValue(AppSettings::sSettingTabSettingIndex,
						this->pUi->tabSettingsInner->currentIndex());

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


void MainWindow::initSettings() {

	quint8 ubAIs = this->pAS->get(AppSettings::sSettingGameCountAIs).toUInt();
	quint8 ubHumans = this->pAS->get(AppSettings::sSettingGameCountHumans).toUInt();

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

	this->pUi->selectSpeed->setCurrentIndex(
				this->pAS->get(AppSettings::sSettingGameSpeed).toInt());

	QComboBox *pBox = this->pUi->selectStartLevel;
	for (int i = 0; i < 256; ++i) {

		pBox->addItem(IconEngine::level(i), QString::number(i));

	} // loop
	pBox->setCurrentIndex(
				this->pAS->get(AppSettings::sSettingGameStartLevel).toInt());

	this->settingsUpdatePlayerColours();

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


void MainWindow::on_cbFakeBonuses_stateChanged(int iState) {

	this->pAS->setValue(AppSettings::sSettingGameFakeBonuses, 0 < iState);

} // on_cbFakeBonuses_stateChanged


void MainWindow::on_cbRelative1_toggled(bool bChecked) {


} // on_cbRelative1_toggled


void MainWindow::on_cbRelative2_toggled(bool bChecked) {


} // on_cbRelative2_toggled


void MainWindow::on_cbRelative3_toggled(bool bChecked) {


} // on_cbRelative3_toggled


void MainWindow::on_cbRelative4_toggled(bool bChecked) {


} // on_cbRelative4_toggled


void MainWindow::on_cbSound_stateChanged(int iState) {

	this->pAS->setValue(AppSettings::sSettingGameSound, 0 < iState);

} // on_cbSound_stateChanged


void MainWindow::on_cbUseMouse1_toggled(bool bChecked) {


} // on_cbUseMouse1_toggled


void MainWindow::on_cbUseMouse2_toggled(bool bChecked) {


} // on_cbUseMouse2_toggled


void MainWindow::on_cbUseMouse3_toggled(bool bChecked) {


} // on_cbUseMouse3_toggled


void MainWindow::on_cbUseMouse4_toggled(bool bChecked) {


} // on_cbUseMouse4_toggled


void MainWindow::onDebugMessage(const QString &sMessage) const {

	QString sOut = "MW:" + sMessage;

	std::cout << sOut.toStdString() << std::endl;

	Q_EMIT this->debugMessage(sOut);

} // onDebugMessage


void MainWindow::on_kseDown1_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseDown1_keySequenceChanged


void MainWindow::on_kseDown2_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseDown2_keySequenceChanged


void MainWindow::on_kseDown3_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseDown3_keySequenceChanged


void MainWindow::on_kseDown4_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseDown4_keySequenceChanged


void MainWindow::on_kseLeft1_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseLeft1_keySequenceChanged


void MainWindow::on_kseLeft2_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseLeft2_keySequenceChanged


void MainWindow::on_kseLeft3_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseLeft3_keySequenceChanged


void MainWindow::on_kseLeft4_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseLeft4_keySequenceChanged


void MainWindow::on_kseRight1_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseRight1_keySequenceChanged


void MainWindow::on_kseRight2_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseRight2_keySequenceChanged


void MainWindow::on_kseRight3_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseRight3_keySequenceChanged


void MainWindow::on_kseRight4_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseRight4_keySequenceChanged


void MainWindow::on_kseUp1_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseUp1_keySequenceChanged


void MainWindow::on_kseUp2_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseUp2_keySequenceChanged


void MainWindow::on_kseUp3_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseUp3_keySequenceChanged


void MainWindow::on_kseUp4_keySequenceChanged(const QKeySequence &oKeySequence) {


} // on_kseUp4_keySequenceChanged


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

} // on_selectSpeed_currentIndexChanged


void MainWindow::on_selectStartLevel_currentIndexChanged(int iIndex) {

	// avoid while items are being added
	if (256 > this->pUi->selectStartLevel->count()) return;

	this->pAS->setValue(AppSettings::sSettingGameStartLevel, iIndex);

} // on_selectStartLevel_currentIndexChanged


void MainWindow::onStatusMessage(const QString &sMessage) const {

	this->pUi->statusBar->showMessage(sMessage);
	this->pUi->statusBar->setToolTip(sMessage);

} // onStatusMessage


void MainWindow::onPlayerColourChanged(const quint8 ubWorm, const quint8 ubIndex) {

	quint8 ubIndexOld = this->pAS->getPlayerColour(ubWorm);
	if (ubIndex == ubIndexOld) return;

	// find out which worm has the new colour so we can give it
	quint8 ubWormOldHolder = this->pAS->getPlayerByColour(ubIndex);

	this->pAS->setPlayerColour(ubWorm, ubIndex);
	this->pAS->setPlayerColour(ubWormOldHolder, ubIndexOld);

	this->settingsUpdatePlayerColours();

} // onWormPlayerChanged


void MainWindow::run() {

	// init tab views

	this->initSettings();

	this->initBuilder();

	//this->initScores();

	// bring last used to front
	this->pUi->tabWidget->setCurrentIndex(
				this->pAS->get(AppSettings::sSettingTabMainIndex).toInt());


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

} // settingsUpdatePlayerCount



}	} // namespace SwissalpS::QtNibblers
