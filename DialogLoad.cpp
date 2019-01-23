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

	this->pUi->comboBox->setCurrentIndex(225);

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
