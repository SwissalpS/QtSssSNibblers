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
#include "WormAI.h"

#include "IconEngine.h"

#include <QTime>



namespace SwissalpS { namespace QtNibblers {



WormAI::WormAI(QObject *pParent) :
	QObject(pParent),
	ubCountDeadendRun(1u),
	pMapGame(nullptr),
	pMapShaddow(nullptr) {

	this->pMapShaddow = new Map(SssS_Nibblers_Surface_Width,
								SssS_Nibblers_Surface_Height, this);

	// init randomizer
	qsrand(uint(QTime::currentTime().msecsSinceStartOfDay()));

} // construct


WormAI::~WormAI() {

} // dealloc


// virtual copy of worm.vala Worm.can_move_to(....)
bool WormAI::canMoveTo(Worm *pWorm) {

	if (pWorm->isImmune()) return true;

	quint8 ubState = this->pMapGame->tile(pWorm->nextPoint());
	if (L::NullTile == ubState) return false;

	static QVector<quint8> aStatesToAvoid;
	if (aStatesToAvoid.isEmpty())
		aStatesToAvoid = IconEngine::statesSnakes() + IconEngine::statesWalls();

	return !aStatesToAvoid.contains(ubState);

} // canMoveTo


// virtual copy of worm.vala Worm::ai_deadend_after(....)
/* Check whether the worm will be trapped in a dead end. A location
 * within the dead end and the length of the worm is given. This
 * prevents worms getting trapped in a spiral, or in a corner sharper
 * than 90 degrees.  runnumber is a unique number used to update the
 * deadend board. The principle of the deadend board is that it marks
 * all squares previously checked, so the exact size of the deadend
 * can be calculated in O(n) time; to prevent the need to clear it
 * afterwards, a different number is stored in the board each time
 * (the number will not have been previously used, so the board will
 * appear empty). Although in theory deadend_runnumber may wrap round,
 * after 4 billion steps the entire board is likely to have been
 * overwritten anyway.
 */
qint32 WormAI::deadend(const QPoint oStart, qint32 ilLen) {

	static QVector<quint8> aStatesToAvoid;
	if (aStatesToAvoid.isEmpty())
		aStatesToAvoid = IconEngine::statesSnakes() + IconEngine::statesWalls();

	if (0 >= ilLen) return 0;

	QPoint oNext;

	for (quint8 ubDirection = L::North; ubDirection <= L::East; ubDirection++) {

		oNext = L::warpPoint(oStart, ubDirection);

		if ((!aStatesToAvoid.contains(this->pMapGame->tile(oNext)))
				&& (this->ubCountDeadendRun != this->pMapShaddow->tile(oNext))) {

			this->pMapShaddow->setTile(oNext, this->ubCountDeadendRun);
			ilLen = this->deadend(oNext, ilLen - 1);
			if (0 >= ilLen) return 0;

		} // if free cell

	} // loop all directions

	return ilLen;

} // deadend


// virtual copy of worm.vala Worm::ai_deadend_after(....)
/* Check a deadend starting from the next square in this direction,
 * rather than from this square. Also block off the squares near worm
 * heads, so that humans can't kill AI players by trapping them
 * against a wall.  The given length is quartered and squared; this
 * allows for the situation where the worm has gone round in a square
 * and is about to get trapped in a spiral. However, it's set to at
 * least BOARDWIDTH, so that on the levels with long thin paths a worm
 * won't start down the path if it'll crash at the other end.
 */
qint32 WormAI::deadendAfter(Worm *pWorm, const QVector<Worm *> apWorms, const qint32 ilLen) {

	if (L::NullTile == this->pMapGame->tile(pWorm->nextPoint())) return 0;

	if (255u == this->ubCountDeadendRun) {

		// this should never happen the way I've set it up to clear
		// counter on each call to move (a bit much, only goes up to 23 with 8 worms)
		this->onDebugMessage("ran out of space in deadendnumber SurfaceGame::aiDeadendAfter");

		return 0;

	} // if ran out of space
	this->ubCountDeadendRun++;

	qint32 ilCl;
	QPoint oNext = pWorm->nextPoint();
	quint8 ubX = quint8(oNext.x());
	quint8 ubY = quint8(oNext.y());

	QPoint oHeadOther;
	L::Heading eDirection = pWorm->currentDirection();

	Worm *pWormOther;
	for (int i = 0; i < apWorms.length(); ++i) {

		pWormOther = apWorms.at(i);
		if (pWormOther == pWorm) continue;
		if (pWormOther->isDead()) continue;

		oHeadOther = pWormOther->headCell()->getPos();

		if (oHeadOther.x() != ubX || oHeadOther.y() != ubY) {

			this->pMapShaddow->setTile(L::warpPoint(oHeadOther, L::North), this->ubCountDeadendRun);
			this->pMapShaddow->setTile(L::warpPoint(oHeadOther, L::West), this->ubCountDeadendRun);
			this->pMapShaddow->setTile(L::warpPoint(oHeadOther, L::South), this->ubCountDeadendRun);
			this->pMapShaddow->setTile(L::warpPoint(oHeadOther, L::East), this->ubCountDeadendRun);

		} // if

	} // loop worms

	this->pMapShaddow->setTile(oNext, this->ubCountDeadendRun);

	oNext = L::warpPoint(oNext, eDirection);

	this->pMapShaddow->setTile(oNext, this->ubCountDeadendRun);

	ilCl = (ilLen * ilLen) / 16;
	if (SssS_Nibblers_Surface_Width > ilCl) ilCl = SssS_Nibblers_Surface_Width;

	return this->deadend(oNext, ilCl);

} // deadendAfter


// virtual copy of worm.vala Worm.ai_move(....)
void WormAI::move(Worm *pWorm, const QVector<Worm *> apWorms, const MapGame *pMap) {

	this->pMapGame = pMap;
	if (nullptr == this->pMapGame) {

		this->onDebugMessage("No MapGame defined.");
		return;

	} // if have no map

	// check how messy shaddow map has become
	if (222u <= this->ubCountDeadendRun) {

		this->pMapShaddow->fillAll(L::FloorClean);
		this->ubCountDeadendRun = 1u;

	} // if time to clean up shadow-map aka Worm::deadend_board

	// check immediate surrounding
	bool bAhead = this->wander(pWorm->nextPoint(), pWorm->nextPoint(), pWorm->currentDirection());
	bool bLeft = this->wander(pWorm->leftPoint(), pWorm->leftPoint(), pWorm->headingLeft());
	bool bRight = this->wander(pWorm->rightPoint(), pWorm->rightPoint(), pWorm->headingRight());

	if (!bAhead) {

		if (bLeft) pWorm->onTurnLeft();
		else if (bRight) pWorm->onTurnRight();
		else {

			// move randomly
			if ((qrand() % 31) == 1) {

				if (qrand() & 1) pWorm->onTurnLeft();
				else pWorm->onTurnRight();

			} // if random turn

		} // if left, right or ahead

	} // if not ahead

	/* Avoid walls, dead-ends and other worm's heads. This is done using
	 * an evalution function which is CAPACITY for a wall, 4 if another
	 * worm's head is in the tooclose area, 4 if another worm's head
	 * could move to the same location as ours, plus 0 if there's no
	 * dead-end, or the amount that doesn't fit for a deadend. olddir's
	 * score is reduced by 100, to favour it, but only if its score is 0
	 * otherwise; this is so that if we're currently trapped in a dead
	 * end, the worm will move in a space-filling manner in the hope
	 * that the dead end will disappear (e.g. if it's made from the tail
	 * of some worm, as often happens).
	 */
	L::Heading eDirectionOld = pWorm->currentDirection();
	static const quint16 uiCapacity = SssS_Nibblers_Surface_Height * SssS_Nibblers_Surface_Width;
	qint32 ilBestYet = uiCapacity * 2;
	quint8 ubDirectionBest = 0u;
	qint32 ilLen;

	for (quint8 ubDirection = 0; ubDirection < 3; ++ubDirection) {

		ilLen = 0;

		if (0u == ubDirection) {
			// ahead

		} else if (1u == ubDirection) {
			// left
			pWorm->onTurnLeft();

		} else {
			// right
			pWorm->onTurnRight();
			pWorm->onTurnRight();

		} // switch direction

		if (!this->canMoveTo(pWorm)) ilLen += uiCapacity;

		if (this->tooClose(pWorm, apWorms)) ilLen += 4;

		ilLen += this->deadendAfter(pWorm, apWorms, ilLen);

		// favour current direction
		if ((pWorm->currentDirection() == eDirectionOld) && (0 >= ilLen))
			ilLen -= 100;

		/* If the favoured direction isn't appropriate, then choose
		 * another direction at random rather than favouring one in
		 * particular, to stop the worms bunching in the bottom-
		 * right corner of the board.
		 */
		if (0 >= ilLen) {

			ilLen -= (qrand() % 101);

		} // if introduce noise

		if (ilLen < ilBestYet) {

			ilBestYet = ilLen;
			ubDirectionBest = pWorm->currentDirection();

		} // if better found

	} // loop 3 directions

	// turn worm into next best direction
	if (0u < ubDirectionBest) pWorm->setHeading(L::headingOfUint(ubDirectionBest));

	/* Make sure we are at least avoiding walls.
	 * Mostly other snakes should avoid our head.
	 */
	if (!this->canMoveTo(pWorm)) {

		pWorm->onTurnLeft();

		if (!this->canMoveTo(pWorm)) {

			// try right
			pWorm->onTurnRight();
			pWorm->onTurnRight();

		} // if can not go left

	} // if can not go ahead

} // move


// virtual copy of worm.vala Worm.ai_too_close(....)
/* Check to see if another worm's head is too close in front of us;
 * that is, that it's within 3 in the direction we're going and within
 * 1 to the side.
 */
bool WormAI::tooClose(Worm *pWorm, const QVector<Worm *> apWorms) {

	if (pWorm->isImmune()) return false;

	int iDx, iDy;
	QPoint oHead = pWorm->headCell()->getPos();
	QPoint oHeadOther;
	L::Heading eDirection = pWorm->currentDirection();

	Worm *pWormOther;
	for (int i = 0; i < apWorms.length(); ++i) {

		pWormOther = apWorms.at(i);
		if (pWormOther == pWorm) continue;
		if (pWormOther->isDead()) continue;

this->onDebugMessage("checking worm");

		oHeadOther = pWormOther->headCell()->getPos();

		iDx = oHead.x() - oHeadOther.x();
		iDy = oHead.y() - oHeadOther.y();

		switch (eDirection) {

			case L::North:
				if ((iDy > 0) && (iDy <= 3) && (iDx >= -1) && (iDx <= 1))
					return true;
			break;

			case L::South:
				if ((iDy < 0) && (iDy >= -3) && (iDx >= -1) && (iDx <= 1))
					return true;
			break;

			case L::West:
				if ((iDx > 0) && (iDx <= 3) && (iDy >= -1) && (iDy <= 1))
					return true;
			break;

			case L::East:
				if ((iDx < 0) && (iDx >= -3) && (iDy >= -1) && (iDy <= 1))
					return true;
			break;
			default: break;

		} // switch eDirection

	} // loop worms

	return false;

} // tooClose


// virtual copy of worm.vala Worm::ai_wander(....)
bool WormAI::wander(const QPoint oStart, const QPoint oStop,
					const L::Heading eDirection) {

	if (oStart.isNull() || oStart.x() >= 0xFF || oStart.y() >= 0xFF) return false;
	if (oStop.isNull() || oStop.x() >= 0xFF || oStop.y() >= 0xFF) return false;
	if (L::Nowhere == eDirection) return false;

	static QVector<quint8> aStatesToAvoid;
	if (aStatesToAvoid.isEmpty())
		aStatesToAvoid = IconEngine::statesSnakes() + IconEngine::statesWalls();

	QPoint oNext = L::warpPoint(oStart, eDirection);
	if (oNext.isNull() || oNext.x() >= 0xFF || oNext.y() >= 0xFF) return false;

	quint8 ubState = this->pMapGame->tile(oStart);
	if (L::NullTile == ubState) return false;
	if (aStatesToAvoid.contains(ubState)) return false;

	switch (ubState) {

		case L::BonusAppleNW:
		case L::BonusAppleNE:
		case L::BonusAppleSE:
		case L::BonusAppleSW:
			return true;

		case L::BonusCherryNW:
		case L::BonusCherryNE:
		case L::BonusCherrySE:
		case L::BonusCherrySW:
			return false;

		case L::BonusBananaNW:
		case L::BonusBananaNE:
		case L::BonusBananaSE:
		case L::BonusBananaSW:
			return true;

		case L::BonusHeartNW:
		case L::BonusHeartNE:
		case L::BonusHeartSE:
		case L::BonusHeartSW:
			return true;

		case L::BonusDiamondNW:
		case L::BonusDiamondNE:
		case L::BonusDiamondSE:
		case L::BonusDiamondSW:
			return true;

		default:

			if (oNext == oStop) return false;

			return this->wander(oNext, oStop, eDirection);

	} // switch state

} // wander



}	} // namespace SwissalpS::QtNibblers
