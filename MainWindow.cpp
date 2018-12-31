#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include "SurfaceBuilder.h"
#include <QStatusBar>



namespace SwissalpS { namespace QtNibblers {



MainWindow::MainWindow(QWidget *pParent) :
	QMainWindow(pParent),
	pUi(new Ui::MainWindow),
	pAS(AppSettings::pAppSettings()) {

	this->pUi->setupUi(this);

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
	this->pAS->setValue(AppSettings::sSettingTabMain,
						this->pUi->tabWidget->currentIndex());

} // closeEvent


void MainWindow::initBuilder() {

	SurfaceBuilder *pBuilder = new SurfaceBuilder();
	this->pUi->tabBuilder->layout()->addWidget(pBuilder);

	connect(pBuilder, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	connect(pBuilder, SIGNAL(statusMessage(QString)),
			this, SLOT(onStatusMessage(QString)));

	pBuilder->initCells();

} // initBuilder


void MainWindow::onDebugMessage(const QString &sMessage) const {

	QString sOut = "MW:" + sMessage;

	std::cout << sOut.toStdString() << std::endl;

	Q_EMIT this->debugMessage(sOut);

} // onDebugMessage


void MainWindow::onStatusMessage(const QString &sMessage) const {

	this->pUi->statusBar->showMessage(sMessage);
	this->pUi->statusBar->setToolTip(sMessage);

} // onStatusMessage


void MainWindow::run() {

	this->initBuilder();

} // run



}	} // namespace SwissalpS::QtNibblers
