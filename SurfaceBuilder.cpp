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
	quint8 ubState = 1u;

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

		ubState++;
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

	this->onDebugMessage(QString::number(ubColumn) + " " + QString::number(ubRow)
						 + " " + QString::number(this->aRows.count()));

	// has a click already occured?
	if (0xFF > ubLastColumn) {

		if (bShift) {

			quint8 ubCountX;
			quint8 ubCountY;
			int iX;
			int iY;
			QList<SurfaceCell *> aRow;
			SurfaceCell *pCell_;

			if ((ubColumn == this->ubLastColumn) && (ubRow == this->ubLastRow)) {

				// same cell, skip and handle as simple click

			} else if (ubColumn == this->ubLastColumn) {

				// same column

				if (ubRow < this->ubLastRow) {

					// bottom to top
					for (iY = this->ubLastRow - 1u; iY >= ubRow; --iY) {

						aRow = this->aRows.at(iY);
						pCell_ = aRow.at(ubColumn);
						this->toggleCell(pCell_);

					} // loop

				} else {

					// top to bottom
					for (ubCountY = this->ubLastRow + 1u; ubCountY <= ubRow; ++ubCountY) {

						aRow = this->aRows.at(ubCountY);
						pCell_ = aRow.at(ubColumn);
						this->toggleCell(pCell_);

					} // loop

				} // if ttb or btt

				this->ubLastColumn = ubColumn;
				this->ubLastRow = ubRow;

				return;

			} else if (ubRow == this->ubLastRow) {

				// same row
				aRow = this->aRows.at(ubRow);

				if (ubColumn < this->ubLastColumn) {

					// right to left {
					for (iX = this->ubLastColumn - 1u; iX >= ubColumn; iX--) {

						pCell_ = aRow.at(iX);
						this->toggleCell(pCell_);

					} // loop

				} else {

					// left to right
					for (ubCountX = this->ubLastColumn + 1u; ubCountX <= ubColumn; ++ubCountX) {

						pCell_ = aRow.at(ubCountX);
						this->toggleCell(pCell_);

					} // loop

				} // if rtl or ltr

				this->ubLastColumn = ubColumn;
				this->ubLastRow = ubRow;

				return;

			} else {

				// 'diagonal'
//if (false) {
				quint8 ubBiggerX = qMax(ubColumn, this->ubLastColumn);
				quint8 ubBiggerY = qMax(ubRow, this->ubLastRow);
				quint8 ubSmallerX = qMin(ubColumn, this->ubLastColumn);
				quint8 ubSmallerY = qMin(ubRow, this->ubLastRow);
				quint8 ubDiffX = ubBiggerX - ubSmallerX;
				quint8 ubDiffY = ubBiggerY - ubSmallerY;
				qreal fStepX = qreal(ubDiffX) / qreal(ubDiffY);
				qreal fStepY = qreal(ubDiffY) / qreal(ubDiffX);
				qreal ubSteps = qMax(ubDiffX, ubDiffY);

				quint8 ubX = ubSmallerX;
				quint8 ubY = ubSmallerY;
				ubCountX = 0u;

				this->onDebugMessage(" diffX: " + QString::number(ubDiffX) +
									 " diffY: " + QString::number(ubDiffY) +
									 " stepX: " + QString::number(fStepX) +
									 " stepY: " + QString::number(fStepY));

				for (; ubSteps > 0u; ubSteps--) {

					this->toggleCell(ubX, ubY);

//					aRow = this->aRows.at(ubY);
//					pCell_ = aRow.at(ubX);

//					this->toggleCell(pCell_);

					ubCountX++;
					ubX = ubSmallerX + quint8(qreal(ubCountX) * fStepX);
					ubY = ubSmallerY + quint8(qreal(ubCountX) * fStepY);

				} // loop

				this->ubLastColumn = ubColumn;
				this->ubLastRow = ubRow;

				return;
//} // if false
			} // if same cell, same column, same row or diagonal

		} // if shift was pressed

	} // if not first click

	// check limits just in case
	//if (ubRow >= this->aRows.count()) return;
	//QList<SurfaceCell*> aRow = this->aRows.at(ubRow);

	//if (ubColumn >= aRow.count()) return;
	//SurfaceCell *pCell = aRow.at(ubColumn);

	toggleCell(pCell);
	//this->update();

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
