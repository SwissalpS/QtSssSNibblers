#include "SurfaceCell.h"
#include "ui_SurfaceCell.h"

#include "definitions.h"
#include "IconEngine.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>



namespace SwissalpS { namespace QtNibblers {



SurfaceCell::SurfaceCell(QWidget *pParent) :
	QFrame(pParent),
	pUi(nullptr),
	bBuilder(false),
	ubState(0xFFu),
	ubColumn(0xFFu),
	ubRow(0xFFu) {

	this->aeHeadingsBloated.clear();

} // construct null-cell


SurfaceCell::SurfaceCell(bool bBuilder, quint8 ubState, quint8 ubColumn,
						 quint8 ubRow, QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::SurfaceCell),
	bBuilder(bBuilder),
	ubState(ubState),
	ubColumn(ubColumn),
	ubRow(ubRow) {

	this->pUi->setupUi(this);

	this->aeHeadingsBloated.clear();

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
		case 10u: case 11u: case 12u: case 13u: case 14u:
		case 15u: case 16u: case 17u: case 18u: case 19u:
			return QColor(SssS_Nibblers_Colour_0); break;

		case 20u: case 21u: case 22u: case 23u: case 24u:
		case 25u: case 26u: case 27u: case 28u: case 29u:
			return QColor(SssS_Nibblers_Colour_1); break;

		case 30u: case 31u: case 32u: case 33u: case 34u:
		case 35u: case 36u: case 37u: case 38u: case 39u:
			return QColor(SssS_Nibblers_Colour_2); break;

		case 40u: case 41u: case 42u: case 43u: case 44u:
		case 45u: case 46u: case 47u: case 48u: case 49u:
			return QColor(SssS_Nibblers_Colour_3); break;

		case 50u: case 51u: case 52u: case 53u: case 54u:
		case 55u: case 56u: case 57u: case 58u: case 59u:
			return QColor(SssS_Nibblers_Colour_4); break;

		case 60u: case 61u: case 62u: case 63u: case 64u:
		case 65u: case 66u: case 67u: case 68u: case 69u:
			return QColor(SssS_Nibblers_Colour_5); break;

		case 70u: case 71u: case 72u: case 73u: case 74u:
		case 75u: case 76u: case 77u: case 78u: case 79u:
			return QColor(SssS_Nibblers_Colour_6); break;

		case 80u: case 81u: case 82u: case 83u: case 84u:
		case 85u: case 86u: case 87u: case 88u: case 89u:
			return QColor(SssS_Nibblers_Colour_7); break;

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


void SurfaceCell::desnakeState() {

	// TODO: start counter and cycle through
	// slime states and eventually reach original state

	this->defrostState();

} // desnakeState


void SurfaceCell::mouseReleaseEvent(QMouseEvent *pEvent) {

	if (!this->bBuilder) return;

	if (Qt::LeftButton != pEvent->button()) return;

	bool bShift = Qt::ShiftModifier == pEvent->modifiers();
	Q_EMIT this->clicked(this->ubColumn, this->ubRow, bShift, this);

} // mouseReleaseEvent


void SurfaceCell::paintEvent(QPaintEvent *pEvent) {

	static QVector<quint8> aubSnakes = IconEngine::statesSnakes();

	QFrame::paintEvent(pEvent);

	QPainter oP(this);

	QIcon oIcon = IconEngine::cell(this->ubState, this->bBuilder);

	if (!oIcon.isNull()) {

		// draw icon
		oP.drawPixmap(this->rect(), oIcon.pixmap(this->size()));

		return;

	} // if icon returned

	// empty icon returned

	if (this->bBuilder) {

		// draw floor tile for builder
		oP.setBrush(Qt::lightGray);
		oP.setPen(Qt::darkGray);
		oP.drawRoundedRect(this->rect(), this->width() * 0.32,
						   this->height() * 0.32);

		return;

	} // if builder mode

	if (aubSnakes.contains(this->ubState)) {

		// draw snake
		QColor oColour = this->colour();

		int iA = 1;
		QRect oRect = this->rect().adjusted(iA, iA, -1 * iA, -1 * iA);
		oP.fillRect(oRect, oColour);

		//		oP.setBrush(oColour);
		//		oP.setPen(Qt::black);
				//oP.drawRoundedRect(oRect, oRect.width() * 0.32,
				//				   oRect.height() * 0.32);

		int iBh = this->height() * 0.42 + 1;
		int iBw = this->width() * 0.42 + 1;

		int iSpan;
		int iStart;
		L::Heading eHeading;

		oP.setBrush(oColour);
		oP.setPen(oColour);

		for (int i = 0; i < this->aeHeadingsBloated.length(); ++i) {

			eHeading = this->aeHeadingsBloated.at(i);

			if (L::North == eHeading) {

				iStart = 0;
				iSpan = -180;
				oRect = QRect(0, 0,
							  this->width(), iBh + 1);

			} else if (L::East == eHeading) {

				iStart = 90;
				iSpan = 180;
				oRect = QRect(this->width() - iBw, 0,
							  iBw + 1, this->height());

			} else if (L::South == eHeading) {

				iStart = 0;
				iSpan = 180;
				oRect = QRect(0, this->height() - iBh,
							  this->width(), iBh + 1);

			} else { // must be West

				iStart = 0;
				iSpan = -180;
				oRect = QRect(0, 0,
							  iBw + 1, this->height());

			} // switch heading

			oP.drawChord(oRect, iStart * 16, iSpan * 16);

		} // loop bloated sides

		return;

	} // if snake

	// fallback to just a coloured tile
	oP.fillRect(this->rect(), this->colour());

}  // paintEvent



}	} // namespace SwissalpS::QtNibblers
