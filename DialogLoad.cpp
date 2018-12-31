#include "DialogLoad.h"
#include "ui_DialogLoad.h"



namespace SwissalpS { namespace QtNibblers {



DialogLoad::DialogLoad(QWidget *parent) :
	QDialog(parent),
	pUi(new Ui::DialogLoad) {

	this->pUi->setupUi(this);

	QComboBox *pBox = this->pUi->comboBox;
	for (int i = 0; i < 256; ++i) {

		pBox->addItem(QString::number(i));

	} // loop

} // construct


DialogLoad::~DialogLoad() {

	delete this->pUi;

} // dealloc


int DialogLoad::getSelected() const {

	return this->pUi->comboBox->currentIndex();

} // getSelected


void DialogLoad::setSelected(const int iIndex) const {

	this->pUi->comboBox->setCurrentIndex(iIndex);

} // setSelected


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



}	} // namespace SwissalpS::QtNibblers
