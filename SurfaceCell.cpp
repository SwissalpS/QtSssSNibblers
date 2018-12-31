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

	switch (this->ubState) {

		// most common -> empty space
		case 0u: return QColor(Qt::transparent); // black
		break;

		// also common -> walls
		case 200u:
		case 201u:
		case 202u:
		case 203u:
		case 204u:
		case 205u:
		case 206u:
		case 207u:
		case 208u:
		case 209u: return QColor(Qt::white); break;

			// players
		case 10u:
		case 11u:
		case 12u: return QColor(Qt::green);	break;

		case 20u:
		case 21u:
		case 22u: return QColor(Qt::red); break;

		case 30u:
		case 31u:
		case 32u: return QColor(Qt::blue); break;

		case 40u:
		case 41u:
		case 42u: return QColor(Qt::yellow); break;

		case 50u:
		case 51u:
		case 52u: return QColor(Qt::darkGreen); break;

		case 60u:
		case 61u:
		case 62u: return QColor(Qt::darkRed); break;

		case 70u:
		case 71u:
		case 72u: return QColor(Qt::darkBlue); break;

		case 80u:
		case 81u:
		case 82u: return QColor(Qt::darkYellow); break;

			// bonuses
			// apples -> add length and advance level
		case 100u:
		case 101u:
		case 102u:
		case 103u: return QColor(Qt::cyan); break;
			// cherries -> shorten
		case 110u:
		case 111u:
		case 112u:
		case 113u: return QColor(Qt::magenta); break;
			// bananas -> add length
		case 120u:
		case 121u:
		case 122u:
		case 123u: return QColor(Qt::darkCyan); break;
			// hearts -> add life
		case 130u:
		case 131u:
		case 132u:
		case 133u: return QColor(Qt::darkMagenta); break;
			// diamonds -> reverse direction
		case 140u:
		case 141u:
		case 142u:
		case 143u: return QColor(Qt::gray); break;

		default: return QColor(Qt::lightGray); break;

	} // switch this->ubState

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
