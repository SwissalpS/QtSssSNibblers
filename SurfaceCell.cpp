#include "SurfaceCell.h"
#include "ui_SurfaceCell.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>



namespace SwissalpS { namespace QtNibblers {



SurfaceCell::SurfaceCell(bool bBuilder, quint8 ubState, quint8 ubColumn,
						 quint8 ubRow, QWidget *parent) :
	QFrame(parent),
	pUi(new Ui::SurfaceCell),
	bBuilder(bBuilder),
	ubState(ubState),
	ubColumn(ubColumn),
	ubRow(ubRow) {

	this->pUi->setupUi(this);

} // construct


SurfaceCell::~SurfaceCell() {

	delete this->pUi;

} // dealloc


void SurfaceCell::changeEvent(QEvent *pEvent) {

	QFrame::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		case QEvent::PaletteChange: break;

		case QEvent::MouseButtonRelease:
			this->onDebugMessage("Clicked at " + QString::number(this->ubColumn)
								 + " : " + QString::number(this->ubRow));
		break;

		default:
			this->onDebugMessage(" " + QString::number(this->ubColumn) + " : " + QString::number(this->ubRow) + " " + QString::number(pEvent->type()));

		break;

	} // switch

} // changeEvent


QColor SurfaceCell::colour() const {

	// most common -> empty space
	if (0u == this->ubState) return QColor(Qt::transparent); // black

	// also common -> walls
	if (200u <= this->ubState) return QColor(Qt::white);

	// players
	if (1u == this->ubState) return QColor(Qt::green);
	if (2u == this->ubState) return QColor(Qt::red);
	if (3u == this->ubState) return QColor(Qt::blue);
	if (4u == this->ubState) return QColor(Qt::yellow);
	if (5u == this->ubState) return QColor(Qt::darkGreen);
	if (6u == this->ubState) return QColor(Qt::darkRed);
	if (7u == this->ubState) return QColor(Qt::darkBlue);
	if (8u == this->ubState) return QColor(Qt::darkYellow);

	// bonuses
	// apples -> add length and advance level
	if (100u == this->ubState) return QColor(Qt::cyan);
	// cherries -> shorten
	if (110u == this->ubState) return QColor(Qt::magenta);
	// bananas -> add length
	if (120u == this->ubState) return QColor(Qt::darkCyan);
	// hearts -> add life
	if (130u == this->ubState) return QColor(Qt::darkMagenta);
	// diamonds -> reverse direction
	if (140u == this->ubState) return QColor(Qt::gray);

	// default
	return QColor(Qt::lightGray);
	/*
		color0,
		color1,
		darkGray,
		lightGray,

		transparent
		green,
		red,
		blue,
		yellow,
		darkGreen,
		darkRed,
		darkBlue,
		darkYellow,
		white,
		black,
		gray,
		darkMagenta,
		magenta,
		cyan,
		darkCyan,

*/

} // colour


void SurfaceCell::mouseReleaseEvent(QMouseEvent *pEvent) {

	if (!this->bBuilder) return;

	if (Qt::LeftButton != pEvent->button()) return;

	bool bShift = Qt::ShiftModifier == pEvent->modifiers();
	Q_EMIT this->clicked(this->ubColumn, this->ubRow, bShift, this);

} // mouseReleaseEvent


void SurfaceCell::paintEvent(QPaintEvent *pEvent) {

	QFrame::paintEvent(pEvent);

	QPainter oP(this);

	//oP.fillRect(this->rect(), this->colour());

	oP.setBrush(this->colour());
	oP.drawRoundedRect(this->rect(), this->width() * 0.32,
					   this->height() * 0.32);

} // paintEvent



}	} // namespace SwissalpS::QtNibblers
