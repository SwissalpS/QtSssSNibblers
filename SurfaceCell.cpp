#include "SurfaceCell.h"
#include "ui_SurfaceCell.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include "IconEngine.h"



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
			this->onDebugMessage(" " + QString::number(this->ubColumn)
								 + " : " + QString::number(this->ubRow)
								 + " " + QString::number(pEvent->type()));

		break;

	} // switch

} // changeEvent


QColor SurfaceCell::colour() const {

	switch (this->ubState) {

		// most common -> empty space
		case 0u:
		// spawn points
		case 90u: // headed north
		case 91u: // headed west
		case 92u: // headed south
		case 93u: // headed east
		// teleporter exits
		case 221: // exit A
		case 223: // exit B
		case 225: // exit C
		case 227: // exit D
		case 229: // exit E
		case 231: // exit F
		case 233: // exit G
		case 235: // exit H
		case 237: // exit I
		case 239: // exit J
			return QColor(Qt::transparent); // black
		break;

		// also common -> walls
		case 200u: // vertical
		case 201u: // horizontal
		case 202u: // corner sw
		case 203u: // corner se
		case 204u: // corner nw
		case 205u: // corner ne
		case 206u: // T north
		case 207u: // T east
		case 208u: // T west
		case 209u: // T south
		case 210u: // cross
			return QColor(Qt::white); break;

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

		// teleporter entrances
		case 220: // entrance A
		case 222: // entrance B
		case 224: // entrance C
		case 226: // entrance D
		case 228: // entrance E
		case 230: // entrance F
		case 232: // entrance G
		case 234: // entrance H
		case 236: // entrance I
		case 238: // entrance J
			return QColor(Qt::darkGray); break;

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
		lightGray,
		darkGray,

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

	QIcon oIcon = IconEngine::cell(this->ubState, this->bBuilder);

	if (oIcon.isNull()) {

		if (this->bBuilder) {

			oP.setBrush(this->colour());
			oP.drawRoundedRect(this->rect(), this->width() * 0.32,
							   this->height() * 0.32);

		} else {

			oP.fillRect(this->rect(), this->colour());

		} // if builder mode or not

	} else {

		oP.drawPixmap(this->rect(), oIcon.pixmap(this->size()));

	} // if empty icon returned

	return;

} // paintEvent



}	} // namespace SwissalpS::QtNibblers
