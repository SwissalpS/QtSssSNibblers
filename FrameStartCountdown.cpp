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

