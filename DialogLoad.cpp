#include "DialogLoad.h"
#include "ui_DialogLoad.h"

#include "IconEngine.h"
#include "AppSettings.h"
#include "MapGame.h"



namespace SwissalpS { namespace QtNibblers {



DialogLoad::DialogLoad(QWidget *parent) :
	QDialog(parent),
	pUi(new Ui::DialogLoad) {

	this->pUi->setupUi(this);

	QComboBox *pBox = this->pUi->comboBox;
	for (int i = 0; i < 256; ++i) {

		pBox->addItem(IconEngine::level(i), QString::number(i));

	} // loop

} // construct


DialogLoad::~DialogLoad() {

	delete this->pUi;

} // dealloc


void DialogLoad::changeEvent(QEvent *pEvent) {

	QDialog::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		default:
		break;

	} // switch

} // changeEvent


int DialogLoad::getSelected() const {

	return this->pUi->comboBox->currentIndex();

} // getSelected


void DialogLoad::on_buttonZap0_clicked() {

	this->pUi->comboBox->setCurrentIndex(0);

} // on_buttonZap0_clicked


void DialogLoad::on_buttonZap128_clicked() {

	this->pUi->comboBox->setCurrentIndex(128);

} // on_buttonZap128_clicked


void DialogLoad::on_buttonZap255_clicked() {

	this->pUi->comboBox->setCurrentIndex(255);

} // on_buttonZap255_clicked


void DialogLoad::on_comboBox_currentIndexChanged(int iIndex) {

	QLabel *pPreview = this->pUi->preview;
	pPreview->setPixmap(IconEngine::level(iIndex).pixmap(pPreview->size()));

	AppSettings *pAS = AppSettings::pAppSettings();
	quint8 ubCountAIs = pAS->get(AppSettings::sSettingGameCountAIs).toUInt();
	quint8 ubCountHumans = pAS->get(AppSettings::sSettingGameCountHumans).toUInt();
	quint8 ubCountAll = ubCountAIs + ubCountHumans;

	MapGame *pMap = MapGame::loadedMap(pAS->getDataPathLevelFile(iIndex), this);
	int iCountSPs = pMap->spawnPoints().length();

	QString sMessage;
	if (0 == iCountSPs) {

		sMessage = tr("There are no spawn-points on this map. Use Builder to add some.");

	} else if (ubCountAll > iCountSPs) {

		// safe now to subtract
		quint8 ubDelta = ubCountAll - iCountSPs;

		// TODO: translatable
		sMessage = QString::number(ubDelta) + " spawn-point";
		if (1 == ubDelta) sMessage += " is";
		else sMessage += "s are";
		sMessage += " missing. ";
		sMessage += tr("Either reduce the number of players or add some points in Builder.");

	} // if need to warn

	this->setWarning(sMessage);

} // on_comboBox_currentIndexChanged


void DialogLoad::setSelected(const int iIndex) const {

	this->pUi->comboBox->setCurrentIndex(iIndex);

} // setSelected


void DialogLoad::setWarning(const QString sMessage) const {

	this->pUi->warning->setText(sMessage);

} // setWarning



}	} // namespace SwissalpS::QtNibblers
