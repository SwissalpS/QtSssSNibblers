/*
 * QtSssSNibblers: SwissalpS Nibbles written with Qt-Framework
 * Copyright (C) 2018-2019 Luke J. Zimmermann aka SwissalpS <SwissalpS@LukeZimmermann.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
	pTimer(nullptr),
	bBuilder(bBuilder),
	ubState(ubState),
	ubColumn(ubColumn),
	ubRow(ubRow) {

	this->pUi->setupUi(this);

	this->aeHeadingsBloated.clear();

	this->pTimer = new QTimer(this);
	this->pTimer->setInterval(108);
	this->pTimer->setTimerType(Qt::PreciseTimer);

	connect(this->pTimer, SIGNAL(timeout()),
			this, SLOT(onDesnakeTimer()));

} // construct


SurfaceCell::~SurfaceCell() {

	delete this->pUi;

	if (nullptr != this->pTimer) {
		this->pTimer->stop();
		delete this->pTimer;
		this->pTimer = nullptr;
	}

} // dealloc


bool SurfaceCell::operator==(SurfaceCell *pOther) const {

	return pOther->getPos() == this->getPos();

} // operator==


void SurfaceCell::changeEvent(QEvent *pEvent) {

	QFrame::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		case QEvent::PaletteChange: break;
		case QEvent::ParentChange: break;

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
		case L::FloorClean:
		// spawn points
		case L::SpawnHeadingNorth:
		case L::SpawnHeadingWest:
		case L::SpawnHeadingSouth:
		case L::SpawnHeadingEast:
		// teleporter exits
		case L::TeleporterOutA: // exit A
		case L::TeleporterOutB: // exit B
		case L::TeleporterOutC: // exit C
		case L::TeleporterOutD: // exit D
		case L::TeleporterOutE: // exit E
		case L::TeleporterOutF: // exit F
		case L::TeleporterOutG: // exit G
		case L::TeleporterOutH: // exit H
		case L::TeleporterOutI: // exit I
		case L::TeleporterOutJ: // exit J
			return QColor(Qt::black);//transparent); // black

		case L::FloorWet1:
		case L::FloorWet2: //return QColor(Qt::darkGray).darker(920);
		case L::FloorWet3:
		case L::FloorWet4: //return QColor(Qt::darkGray).darker(460);
		case L::FloorWet5:
		case L::FloorWet6: //return QColor(Qt::darkGray).darker(230);
		case L::FloorWet7:
		case L::FloorWet8: //return QColor(Qt::darkGray).darker(115);
		case L::FloorWet9:
			return QColor(247, 239, 212);//return QColor(Qt::darkGray);

		// also common -> walls
		case L::WallVertical:
		case L::WallHorizontal:
		case L::WallCornerSW:
		case L::WallCornerSE:
		case L::WallCornerNW:
		case L::WallCornerNE:
		case L::WallTnorth:
		case L::WallTeast:
		case L::WallTwest:
		case L::WallTsouth:
		case L::WallCross:
			return QColor(Qt::white);

		// players
		case 10u: case 11u: case 12u: case 13u: case 14u:
		case 15u: case 16u: case 17u: case 18u: case 19u:
			return QColor(SssS_Nibblers_Colour_0);

		case 20u: case 21u: case 22u: case 23u: case 24u:
		case 25u: case 26u: case 27u: case 28u: case 29u:
			return QColor(SssS_Nibblers_Colour_1);

		case 30u: case 31u: case 32u: case 33u: case 34u:
		case 35u: case 36u: case 37u: case 38u: case 39u:
			return QColor(SssS_Nibblers_Colour_2);

		case 40u: case 41u: case 42u: case 43u: case 44u:
		case 45u: case 46u: case 47u: case 48u: case 49u:
			return QColor(SssS_Nibblers_Colour_3);

		case 50u: case 51u: case 52u: case 53u: case 54u:
		case 55u: case 56u: case 57u: case 58u: case 59u:
			return QColor(SssS_Nibblers_Colour_4);

		case 60u: case 61u: case 62u: case 63u: case 64u:
		case 65u: case 66u: case 67u: case 68u: case 69u:
			return QColor(SssS_Nibblers_Colour_5);

		case 70u: case 71u: case 72u: case 73u: case 74u:
		case 75u: case 76u: case 77u: case 78u: case 79u:
			return QColor(SssS_Nibblers_Colour_6);

		case 80u: case 81u: case 82u: case 83u: case 84u:
		case 85u: case 86u: case 87u: case 88u: case 89u:
			return QColor(SssS_Nibblers_Colour_7);

		// teleporter entrances
		case L::TeleporterInA: // entrance A
		case L::TeleporterInB: // entrance B
		case L::TeleporterInC: // entrance C
		case L::TeleporterInD: // entrance D
		case L::TeleporterInE: // entrance E
		case L::TeleporterInF: // entrance F
		case L::TeleporterInG: // entrance G
		case L::TeleporterInH: // entrance H
		case L::TeleporterInI: // entrance I
		case L::TeleporterInJ: // entrance J
			return QColor(Qt::darkGray);

		// bonuses
		// apples -> add length and advance level
		case L::BonusAppleNW:
		case L::BonusAppleNE:
		case L::BonusAppleSE:
		case L::BonusAppleSW: return QColor(Qt::cyan);
			// cherries -> shorten
		case L::BonusCherryNW:
		case L::BonusCherryNE:
		case L::BonusCherrySE:
		case L::BonusCherrySW: return QColor(Qt::magenta);
			// bananas -> add length
		case L::BonusBananaNW:
		case L::BonusBananaNE:
		case L::BonusBananaSE:
		case L::BonusBananaSW: return QColor(Qt::darkCyan);
			// hearts -> add life
		case L::BonusHeartNW:
		case L::BonusHeartNE:
		case L::BonusHeartSE:
		case L::BonusHeartSW: return QColor(Qt::darkMagenta);
			// diamonds -> reverse direction
		case L::BonusDiamondNW:
		case L::BonusDiamondNE:
		case L::BonusDiamondSE:
		case L::BonusDiamondSW: return QColor(Qt::gray);

		default: return QColor(Qt::lightGray);

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


void SurfaceCell::defrostState() {

	this->aeHeadingsBloated.clear();

	this->ubState = this->ubStateFrozen;

	this->onChanged();

	this->update();

} // defrostState


void SurfaceCell::desnakeState() {

	if (1 > this->pTimer->interval()) {

		this->defrostState();

		return;

	} // if no need to bother

	// start counter and cycle through
	// slime states and eventually reach original state

	this->aeHeadingsBloated.clear();

	this->ubState = L::FloorWet9;

	this->onChanged();

	this->update();

	this->pTimer->start();

} // desnakeState


void SurfaceCell::mouseReleaseEvent(QMouseEvent *pEvent) {

	if (!this->bBuilder) {

		// let it bubble up to surface
		QFrame::mouseReleaseEvent(pEvent);

		return;

	} // if in game mode

	if (Qt::LeftButton != pEvent->button()) return;

	bool bShift = Qt::ShiftModifier == pEvent->modifiers();
	Q_EMIT this->clicked(this->ubColumn, this->ubRow, bShift, this);

} // mouseReleaseEvent


void SurfaceCell::onDesnakeTimer() {

	if ((L::FloorClean == this->ubState)
			|| (L::FloorWet9 < this->ubState)) {

		this->pTimer->stop();
		return;

	} // if already moved on to other state

	this->ubState--;
	if (L::FloorClean == this->ubState) {

		this->defrostState();
		return;

	} // if returning to normal

	this->onChanged();

	this->update();

} // onDesnakeTimer


void SurfaceCell::paintEvent(QPaintEvent *pEvent) {

	static QVector<quint8> aubSnakes = IconEngine::statesSnakes();
	static QVector<quint8> aubWetFloors = IconEngine::statesFloorsWet();

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

	QColor oColour = this->colour();

	if (aubSnakes.contains(this->ubState)) {

		// draw snake

		oP.fillRect(this->rect(), Qt::black);

		QRect oRect;
		int iA = 2;
		if (this->aeHeadingsBloated.length()) {

			oRect = this->rect();
			oColour = oColour.darker(115);

		} else {

			oRect = this->rect().adjusted(iA, iA, -1 * iA, -1 * iA);

		} // if bloated

		oP.fillRect(oRect, oColour);

		return;

		//		oP.setBrush(oColour);
		//		oP.setPen(Qt::black);
				//oP.drawRoundedRect(oRect, oRect.width() * 0.32,
				//				   oRect.height() * 0.32);

		// this 'works' but it didn't look good enough
		int iBh = int(double(this->height() * 0.42) + 1);
		int iBw = int(double(this->width() * 0.42) + 1);

		int iSpan;
		int iStart;
		L::Heading eHeading;

		oP.setBrush(oColour.darker(125));
		oP.setPen(oColour.darker(120));

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

				iStart = 90;
				iSpan = -180;
				oRect = QRect(0, 0,
							  iBw + 1, this->height());

			} // switch heading

			oP.drawChord(oRect, iStart * 16, iSpan * 16);

		} // loop bloated sides

		return;

	} // if snake

	if (aubWetFloors.contains(this->ubState)) {

		//oColour = Qt::gray;

		oColour.setAlpha((this->ubState * 192/9));

	} // if post snake

	// fallback to just a coloured tile
	oP.fillRect(this->rect(), oColour);

}  // paintEvent



}	} // namespace SwissalpS::QtNibblers
