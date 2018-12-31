#include "SurfaceBuilder.h"
#include "ui_SurfaceBuilder.h"

#include "definitions.h"
#include <QHBoxLayout>
#include <QVBoxLayout>



namespace SwissalpS { namespace QtNibblers {



SurfaceBuilder::SurfaceBuilder(QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::SurfaceBuilder),
	ubLastColumn(0xFFu),
	ubLastRow(0xFFu) {

	this->pUi->setupUi(this);

	this->aRows.clear();

} // construct


SurfaceBuilder::~SurfaceBuilder() {

	this->aRows.clear();

	delete this->pUi;

} // dealloc


void SurfaceBuilder::changeEvent(QEvent *pEvent) {
	QFrame::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		default:
		break;

	} // switch

} // changeEvent


void SurfaceBuilder::initCells() {

	quint8 ubRows = 0u;
	quint8 ubColumns = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	QHBoxLayout *pHBox;
	quint8 ubState = 0u;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow.clear();
		pHBox = new QHBoxLayout();

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = new SurfaceCell(true, ubState, ubColumns, ubRows, this);

			aRow.append(pCell);
			pHBox->addWidget(pCell);

			connect(pCell, SIGNAL(debugMessage(QString)),
					this, SLOT(onDebugMessage(QString)));

			connect(pCell, SIGNAL(clicked(quint8,quint8,bool,SurfaceCell*)),
					this, SLOT(onCellClicked(quint8,quint8,bool,SurfaceCell*)));

		} // loop columns

		//ubState++;

		this->aRows.append(aRow);
		this->pUi->surfaceRows->addItem(pHBox);

	} // loop rows

} // initCells


void SurfaceBuilder::on_buttonClear_clicked() {

	this->onDebugMessage("Clear clicked");

	quint8 ubRows = this->aRows.count();
	quint8 ubColumns = this->aRows.first().count();
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pCell = aRow.at(ubX);
			pCell->setState(0u);
			//pCell->update();

		} // loop columns

	} // loop rows

	this->update();

} // on_buttonClear_clicked


void SurfaceBuilder::onCellClicked(const quint8 ubColumn, const quint8 ubRow,
								   bool bShift, SurfaceCell *pCell) {


	// has a click already occured?
	if ((0xFF == this->ubLastColumn) || (!bShift)) {

		this->toggleCell(pCell);

		this->ubLastColumn = ubColumn;
		this->ubLastRow = ubRow;

		return;

	} // if first click

	qreal fX1 = this->ubLastColumn;
	qreal fX2 = ubColumn;
	qreal fY1 = this->ubLastRow;
	qreal fY2 = ubRow;

	// Bresenham's line algorithm
	// adopted from: https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.2B.2B
	const bool bSteep = (qAbs(fY2 - fY1) > qAbs(fX2 - fX1));

	if (bSteep) {

		std::swap(fX1, fY1);
		std::swap(fX2, fY2);

	} // if 'steep'

	if (fX1 > fX2) {

		std::swap(fX1, fX2);
		std::swap(fY1, fY2);

	} // if reversed

	const float fDx = fX2 - fX1;
	const float fDy = qAbs(fY2 - fY1);

	float fError = fDx / 2.0f;

	const int iStepY = (fY1 < fY2) ? 1 : -1;
	int iY = int(fY1);

	const int iMaxX = int(fX2);

	for (int iX = int(fX1); iX < iMaxX; iX++) {

		if (bSteep) {

			this->toggleCell(iY, iX);

		} else {

			this->toggleCell(iX, iY);

		} // if steep

		fError -= fDy;
		if (fError < 0) {

			iY += iStepY;
			fError += fDx;

		} // if error margin negative

	} // loop

	this->ubLastColumn = ubColumn;
	this->ubLastRow = ubRow;

} // onCellClicked


void SurfaceBuilder::toggleCell(SurfaceCell *pCell) {

	quint8 ubState = pCell->getState();

	if (0u == ubState) ubState = 200u;
	else ubState = 0u;

	pCell->setState(ubState);
	pCell->update();

} // toggleCell


void SurfaceBuilder::toggleCell(const quint8 ubColumn, const quint8 ubRow) {

	// check limits
	if (ubRow >= this->aRows.count()) return;
	if (ubColumn >= this->aRows.first().count()) return;

	QList<SurfaceCell *> aRow = this->aRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	this->toggleCell(pCell);

} // toggleCell



}	} // namespace SwissalpS::QtNibblers
