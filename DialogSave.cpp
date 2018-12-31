#include "DialogSave.h"
#include "ui_DialogSave.h"



namespace SwissalpS { namespace QtNibblers {



DialogSave::DialogSave(QWidget *pParent) :
	QDialog(pParent),
	pUi(new Ui::DialogSave) {

	this->pUi->setupUi(this);

	QComboBox *pBox = this->pUi->comboBox;
	for (int i = 0; i < 256; ++i) {

		pBox->addItem(QString::number(i));

	} // loop

} // construct


DialogSave::~DialogSave() {

	delete this->pUi;

} // dealloc


int DialogSave::getSelected() const {

	return this->pUi->comboBox->currentIndex();

} // getSelected


void DialogSave::setSelected(const int iIndex) const {

	this->pUi->comboBox->setCurrentIndex(iIndex);

} // setSelected


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



}	} // namespace SwissalpS::QtNibblers
