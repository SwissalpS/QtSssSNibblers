/*
 * QtSssSNibblers: SwissalpS Nibbles written with Qt-Framework
 * Copyright (C) 2018-2026 Luke J. Zimmermann aka SwissalpS <SwissalpS@LukeZimmermann.com>
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

	QKeySequence oKSin(pEvent->key());

	static QKeySequence oKeyReturn(16777220);
	static QKeySequence oKeyEnter(16777221);
	static QKeySequence oKeySpace(32);

	if ((oKSin == oKeyEnter)
			|| (oKSin == QKeySequence::Cancel) // Qt::Key_Escape
			|| (oKSin == oKeyReturn)
			|| (oKSin == oKeySpace)) {

		pEvent->accept();
		Q_EMIT this->done();

	} else QFrame::keyPressEvent(pEvent);

} // keyPressEvent


void FrameStartCountdown::mousePressEvent(QMouseEvent *pEvent) {

	pEvent->accept();
	Q_EMIT this->done();

} // mousePressEvent


void FrameStartCountdown::onSetText(const QString sText, const QString sTextButton) {

	if (sTextButton.length()) {
		this->pUi->labelNumber->setText(sText + "\n\n" + sTextButton);
	} else {
		this->pUi->labelNumber->setText(sText);
	} // if got button text

	this->setFocus();

} // onSetText



}	} // namespace SwissalpS::QtNibblers

