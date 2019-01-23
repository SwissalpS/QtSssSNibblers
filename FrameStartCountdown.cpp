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
#include "FrameStartCountdown.h"
#include "ui_FrameStartCountdown.h"

#include <QPushButton>



namespace SwissalpS { namespace QtNibblers {



FrameStartCountdown::FrameStartCountdown(QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::FrameStartCountdown) {

	this->pUi->setupUi(this);

} // construct


FrameStartCountdown::~FrameStartCountdown() {

	delete this->pUi;

} // dealloc


void FrameStartCountdown::changeEvent(QEvent *pEvent) {

	QFrame::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);

		break;
		default:
		break;

	} // switch type

} // changeEvent


void FrameStartCountdown::keyPressEvent(QKeyEvent *pEvent) {

	if (!this->pUi->button->isVisible()) return;

	QKeySequence oKSin(pEvent->key());

	static QKeySequence oKeyReturn(16777220);
	static QKeySequence oKeyEnter(16777221);
	static QKeySequence oKeySpace(32);

	if ((oKSin == oKeyEnter)
			|| (oKSin == oKeyReturn)
			|| (oKSin == oKeySpace)) this->on_button_clicked();

	else QFrame::keyPressEvent(pEvent);

} // keyPressEvent


void FrameStartCountdown::on_button_clicked() {

	Q_EMIT this->done();

} // on_button_clicked


void FrameStartCountdown::onSetText(const QString sText, const QString sTextButton) {

	this->pUi->labelNumber->setText(sText);

	QPushButton *pButton = this->pUi->button;
	if (sTextButton.length()) {

		pButton->setText(sTextButton);
		pButton->setVisible(true);
		pButton->setEnabled(true);
		pButton->setFocus();

	} else {

		pButton->setVisible(false);
		pButton->setEnabled(false);

	} // if got button text

} // onSetText



}	} // namespace SwissalpS::QtNibblers

