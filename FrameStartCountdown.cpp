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


void FrameStartCountdown::onSetText(const QString sText) {

	this->pUi->labelNumber->setText(sText);

} // onSetText



}	} // namespace SwissalpS::QtNibblers
