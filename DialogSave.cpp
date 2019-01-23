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
#include "DialogSave.h"
#include "ui_DialogSave.h"
#include "IconEngine.h"



namespace SwissalpS { namespace QtNibblers {



DialogSave::DialogSave(QWidget *pParent) :
	QDialog(pParent),
	pUi(new Ui::DialogSave) {

	this->pUi->setupUi(this);

	QComboBox *pBox = this->pUi->comboBox;
	for (int i = 0; i < 256; ++i) {

		pBox->addItem(IconEngine::level(i), QString::number(i));

	} // loop

} // construct


DialogSave::~DialogSave() {

	delete this->pUi;

} // dealloc


void DialogSave::changeEvent(QEvent *pEvent) {

	QDialog::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		default:
		break;

	} // switch

} // changeEvent


int DialogSave::getSelected() const {

	return this->pUi->comboBox->currentIndex();

} // getSelected


void DialogSave::on_buttonZap0_clicked() {

	this->pUi->comboBox->setCurrentIndex(0);

} // on_buttonZap0_clicked


void DialogSave::on_buttonZap128_clicked() {

	this->pUi->comboBox->setCurrentIndex(128);

} // on_buttonZap128_clicked


void DialogSave::on_buttonZap255_clicked() {

	this->pUi->comboBox->setCurrentIndex(225);

} // on_buttonZap255_clicked


void DialogSave::on_comboBox_currentIndexChanged(int iIndex) {

	QLabel *pPreview = this->pUi->preview;
	pPreview->setPixmap(IconEngine::level(iIndex).pixmap(pPreview->size()));

} // on_comboBox_currentIndexChanged


void DialogSave::setSelected(const int iIndex) const {

	this->pUi->comboBox->setCurrentIndex(iIndex);

} // setSelected


void DialogSave::setWarning(const QString sMessage) const {

	this->pUi->warning->setText(sMessage);

} // setWarning



}	} // namespace SwissalpS::QtNibblers
