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
#include "Worm.h"
#include "definitions.h"



namespace SwissalpS { namespace QtNibblers {



Worm::Worm(const QPoint oPoint, const quint8 ubState, const quint8 ubColour,
		   const bool bAI, const quint8 ubLivesMax, QObject *pParent) :
	QObject(pParent),
	bAmAI(bAI),
	uiCountLevels(0u),
	uiTargetLength(5u),
	ubColourIndex(ubColour),
	ubLives(0u),
	ubLivesLost(0u),
	ubLivesMax(ubLivesMax),
	ubSpawnSafetyTicks(0u),
	ulScore(0u),
	oPointSpawn(oPoint),
	sName("Worm"),
	eNextBloat(L::Nowhere) {

	this->onSetSpawnPoint(oPoint, ubState);

} // construct


Worm::~Worm() {

	this->apCells.clear();

} // dealloc


void Worm::addLength(int iLength) {

	uint uiLen = abs(iLength);

	if (0 > iLength) {

		if (uiLen > this->uiTargetLength) {

			this->uiTargetLength = 0u;

		} else this->uiTargetLength -= uiLen;

	} else this->uiTargetLength += uiLen;

	if (2u > this->uiTargetLength) this->uiTargetLength = 2u;

	while (this->apCells.length() > this->uiTargetLength) {

		this->apCells.takeLast()->defrostState();

	} // loop away excess tail

	this->tailCell()->setState(this->tailState());

} // addLength


void Worm::advanceTo(SurfaceCell *pCell) {

	// make old head a mid-section
	if (this->apCells.length())
		this->apCells.first()->setState(this->midState());

	if (L::Nowhere != this->eNextBloat) {

		pCell->addBloatedHeading(this->eNextBloat);
		this->eNextBloat = L::Nowhere;

	} // if add bloat

	// attach new head
	this->apCells.prepend(pCell);
	pCell->setState(this->headState());
	pCell->update();

	// remove any excess
	while (this->uiTargetLength < this->apCells.length()) {

		// revert to normal game state
		this->apCells.last()->desnakeState();
		this->apCells.removeLast();

	} // loop

	// make new tail
	this->tailCell()->setState(this->tailState());

	if (this->ubSpawnSafetyTicks) this->ubSpawnSafetyTicks--;

	this->doNextTurn();

} // advanceTo


SurfaceCell *Worm::assCell() {

	return this->uiTargetLength <= this->apCells.length()
			? this->apCells.at(this->apCells.length() - 2)
			: new SurfaceCell();

} // assCell


void Worm::doNextTurn() {

	if (0 == this->aeNextHeadings.length()) return;

	L::Heading eDirection = this->aeNextHeadings.takeFirst();

	if (this->bUseRelativeControls) {

		// only interested in left an right
		if (L::Left == eDirection) this->onTurnLeft();
		else if (L::Right == eDirection) this->onTurnRight();
		else Q_EMIT this->fart();

		return;

	} // if 2-button steering

	// 4-button navigation

	// check same direction
	if (eDirection == this->eCurrentHeading) {

		Q_EMIT this->fart();

		return;

	} // if already going that way

	// check opposite direction
	//if (eDirection == L::oppositeHeading(this->eCurrentHeading)) {
	if ((eDirection & 1u) == (this->eCurrentHeading & 1u)) {

		Q_EMIT this->fart();

		return;

	} // if opposite direction

	switch (this->eCurrentHeading) {

		case L::North:

			// possible are: w and e
			if (L::West == eDirection) this->onTurnLeft();
			else this->onTurnRight();

		break;

		case L::South:

			// possible are: w and e
			if (L::East == eDirection) this->onTurnLeft();
			else this->onTurnRight();

		break;

		case L::West:

			// possible are: s and n
			if (L::South == eDirection) this->onTurnLeft();
			else this->onTurnRight();

		break;

		case L::East:

			// possible are: s and n
			if (L::North == eDirection) this->onTurnLeft();
			else this->onTurnRight();

		break;

		default:
		break;

	} // switch this->eCurrentHeading

} // doNextTurn


SurfaceCell *Worm::headCell() {

	return this->apCells.length() ? this->apCells.first()
								  : new SurfaceCell();

} // headCell


L::Heading Worm::headingLeft() const {

	switch (this->eCurrentHeading) {

		case L::North: return L::West;
		case L::South: return L::East;
		case L::West: return L::South;
		case L::East: return L::North;

		default:

			this->onDebugMessage(tr("Weird situation in Worm::headingLeft() ")
									+ QString::number(this->eCurrentHeading));
		break;

	} // switch this->eCurrentHeading

	return L::Nowhere;

} // headingLeft


L::Heading Worm::headingRight() const {

	switch (this->eCurrentHeading) {

		case L::North: return L::East;
		case L::South: return L::West;
		case L::West: return L::North;
		case L::East: return L::South;

		default:

			this->onDebugMessage(tr("Weird situation in Worm::headingRight() ")
									+ QString::number(this->eCurrentHeading));
		break;

	} // switch this->eCurrentHeading

	return L::Nowhere;

} // headingRight


QPoint Worm::leftPoint() {

	if (0 == this->apCells.length()) return QPoint();

	SurfaceCell *pCell = this->apCells.first();

	return L::warpPoint(pCell->getPos(), this->headingLeft());

} // leftPoint


SurfaceCell *Worm::neckCell() {

	return 1 < this->apCells.length() ? this->apCells.at(1)
									  : new SurfaceCell();

} // neckCell


QPoint Worm::nextPoint() {

	if (0 == this->apCells.length()) return QPoint();

	SurfaceCell *pCell = this->apCells.first();

	return L::warpPoint(pCell->getPos(), this->eCurrentHeading);

} // nextPoint


void Worm::onAddLife() {

	if (this->ubLivesMax) {

		// already got max or more (jic)
		if (this->ubLivesMax <= this->ubLives) return;

	} // if limited lives

	this->ubLives++;

	Q_EMIT this->updateLives(this->ubLives);

} // onAddLife


void Worm::onAddScore(const qint16 iScore) {

	quint16 uiScore = abs(iScore);

	if (0 > iScore) {

		// subtract
		if (this->ulScore < uiScore) this->ulScore = 0u;
		else this->ulScore -= uiScore;

	} else this->ulScore += uiScore;

	Q_EMIT this->updateScore(this->ulScore);

} // onAddScore


void Worm::onGrow(const float fFactor) {

	this->uiTargetLength = quint16(fFactor * float(this->uiTargetLength));
	if (2 > this->uiTargetLength) this->uiTargetLength = 2;

} // onGrow


void Worm::onReverse() {

	if (this->isDead()) return;

	this->aeNextHeadings.clear();

	QVector<SurfaceCell *> apOld(this->apCells);
	this->apCells.clear();

	while (apOld.length()) {

		this->apCells.append(apOld.takeLast());

	} // loop cells in backwards

	this->headCell()->setState(this->headState());
	this->tailCell()->setState(this->tailState());

	bool bSolveWithHeadOnly = false;
	L::Heading eNewHeading = this->eCurrentHeading;

	// check the first two segments to determine the heading
	if (2 <= this->apCells.length()) {

		// try with the first 2 cells to find direction
		eNewHeading = L::oppositeHeading(this->apCells.first()->getPos(),
										 this->apCells.at(1)->getPos());

		// did that work?
		if (L::Nowhere == eNewHeading) {

			// try again with 2nd and 3rd cells
			if (3 <= this->apCells.length()) {

				eNewHeading = L::oppositeHeading(this->apCells.at(1)->getPos(),
												 this->apCells.at(2)->getPos());

				if (L::Nowhere == eNewHeading) bSolveWithHeadOnly = true;

			} else {

				// pick random direction or opposite of what head is doing now
				bSolveWithHeadOnly = true;

			} // if got at least 3 cells or not

		} // if teleporter

	} else bSolveWithHeadOnly = true;

	if (bSolveWithHeadOnly)
		this->eCurrentHeading = L::oppositeHeading(this->eCurrentHeading);
	else this->eCurrentHeading = eNewHeading;

} // onReverse


void Worm::onSetLives(const quint8 ubLives) {

	this->ubLives = ubLives;

	Q_EMIT this->updateLives(this->ubLives);

	if (this->isDead()) Q_EMIT this->died(this->bAmAI);

} // onSetLives


// old way
void Worm::onSetSpawnCell(SurfaceCell *pCell) {

	this->onDebugMessage("OLD !!!!!!!!! onSetSpawnCell  ");
	return ;

	this->pCellSpawn = pCell;
	this->oPointSpawn = pCell->getPos();

	// determine initial heading from spawn-cell
	switch (this->pCellSpawn->getState()) {
		case L::SpawnHeadingNorth: this->eCurrentHeading = L::North; break;
		case L::SpawnHeadingWest: this->eCurrentHeading = L::West; break;
		case L::SpawnHeadingSouth: this->eCurrentHeading = L::South; break;
		case L::SpawnHeadingEast:
		default:
			this->eCurrentHeading = L::East;
		break;

	} // switch pCell->getState()

	this->eSpawnHeading = this->eCurrentHeading;

} // onSetSpawnCell


void Worm::onSetSpawnPoint(const QPoint oPoint, const quint8 ubState) {

	this->oPointSpawn = oPoint;

	// determine initial heading from spawn-points state
	switch (ubState) {
		case L::SpawnHeadingNorth: this->eCurrentHeading = L::North; break;
		case L::SpawnHeadingWest: this->eCurrentHeading = L::West; break;
		case L::SpawnHeadingSouth: this->eCurrentHeading = L::South; break;
		case L::SpawnHeadingEast:
		default:
			this->eCurrentHeading = L::East;
		break;

	} // switch ubState

	this->eSpawnHeading = this->eCurrentHeading;

} // onSetSpawnPoint


void Worm::onSubtractLife() {

	this->ubLivesLost++;

	if (this->isDead()) return;

	this->ubLives--;

	Q_EMIT this->updateLives(this->ubLives);
	Q_EMIT this->updateScore(this->ulScore);

	if (this->isDead()) Q_EMIT this->died(this->bAmAI);

} // onSubtractLife


// key press entrance
void Worm::onTurn(const L::Heading eDirection) {

	if (SssS_Nibblers_Max_Key_Cache > this->aeNextHeadings.length())
		this->aeNextHeadings.append(eDirection);

} // onTurn


void Worm::onTurnLeft() {

	this->eCurrentHeading = this->headingLeft();

} // onTurnLeft


void Worm::onTurnRight() {

	this->eCurrentHeading = this->headingRight();

} // onTurnRight


QPoint Worm::rightPoint() {

	if (0 == this->apCells.length()) return QPoint();

	SurfaceCell *pCell = this->apCells.first();

	return L::warpPoint(pCell->getPos(), this->headingRight());

} // rightPoint


void Worm::setColourIndex(const quint8 ubIndex) {

	this->ubColourIndex = ubIndex;

	this->headCell()->setState(this->headState());
	this->tailCell()->setState(this->tailState());

	quint8 ubState = this->midState();

	for (int i = 1; i < this->apCells.length() -1; ++i) {

		this->apCells.at(i)->setState(ubState);

	} // loop

	Q_EMIT this->updateColour(ubIndex);

} // setColourIndex


void Worm::startSpawning() {

	this->apCells.clear();

	this->uiTargetLength = 5u;
	this->ubSpawnSafetyTicks = 7u;

	this->aeNextHeadings.clear();
	this->eCurrentHeading = this->eSpawnHeading;
	this->eNextBloat = L::Nowhere;

} // startSpawning


SurfaceCell *Worm::tailCell() {

	return this->uiTargetLength <= this->apCells.length() ? this->apCells.last()
														  : new SurfaceCell();

} // tailCell



}	} // namespace SwissalpS::QtNibblers
