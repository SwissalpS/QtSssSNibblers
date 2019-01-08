#include "Worm.h"
#include "definitions.h"



namespace SwissalpS { namespace QtNibblers {



Worm::Worm(SurfaceCell *pCell, const quint8 ubColour,
		   const bool bAI, QObject *pParent) :
	QObject(pParent),
	bAmAI(bAI),
	uiTargetLength(5u),
	ubColourIndex(ubColour),
	ubLives(0u),
	ubSpawnSafetyTicks(0u),
	ulScore(0u),
	pCellSpawn(pCell) {

	this->onSetSpawnCell(pCell);

	this->startSpawning();

} // construct


Worm::~Worm() {

	this->apCells.clear();

} // dealloc


void Worm::advanceTo(SurfaceCell *pCell) {

	// make old head a mid-section
	this->apCells.first()->setState(this->midState());

	// attach new head
	this->apCells.prepend(pCell);
	pCell->setState(this->headState());
	pCell->update();

	// remove any excess
	while (this->uiTargetLength < this->apCells.length()) {

		// revert to normal game state
		this->apCells.last()->desnakeState();
		this->apCells.remove(this->apCells.length() - 1);

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

} // headingRight


QPoint Worm::leftPoint() {

	if (0 == this->apCells.length()) return QPoint();

	SurfaceCell *pCell = this->apCells.first();

	return L::warpPoint(pCell->getColumn(), pCell->getRow(),
						this->headingLeft());

} // leftPoint


SurfaceCell *Worm::neckCell() {

	return 1 < this->apCells.length() ? this->apCells.at(1)
									  : new SurfaceCell();

} // neckCell


QPoint Worm::nextPoint() {

	if (0 == this->apCells.length()) return QPoint();

	SurfaceCell *pCell = this->apCells.first();

	return L::warpPoint(pCell->getColumn(), pCell->getRow(), this->eCurrentHeading);

} // nextPoint


void Worm::onAddLife() {

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



void Worm::onReverse() {

	QVector<SurfaceCell *> apOld(this->apCells);
	this->apCells.clear();

	for (int i = apOld.length() - 1; i >= 0 ; ++i) {

		this->apCells.append(apOld.at(i));

	} // loop

	this->eCurrentHeading = L::oppositeHeading(this->eCurrentHeading);

} // onReverse


void Worm::onSetLives(const quint8 ubLives) {

	this->ubLives = ubLives;

	Q_EMIT this->updateLives(this->ubLives);

	if (this->isDead()) Q_EMIT this->died();

} // onSetLives


void Worm::onSetSpawnCell(SurfaceCell *pCell) {

	this->pCellSpawn = pCell;

	// determine initial heading from spawn-cell
	switch (this->pCellSpawn->getState()) {
		case 90u: this->eCurrentHeading = L::North; break;
		case 91u: this->eCurrentHeading = L::West; break;
		case 92u: this->eCurrentHeading = L::South; break;

		case 93u:
		default:
			this->eCurrentHeading = L::East;
		break;

	} // switch pCell->getState()

	this->eSpawnHeading = this->eCurrentHeading;

} // onSetSpawnCell


void Worm::onSubtractLife() {

	if (this->isDead()) return;

	this->ubLives--;

	Q_EMIT this->updateLives(this->ubLives);

	if (this->isDead()) Q_EMIT this->died();

} // onSubtractLife


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

	return L::warpPoint(pCell->getColumn(), pCell->getRow(), this->headingRight());

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
	this->apCells.prepend(this->pCellSpawn);

	this->uiTargetLength = 5u;
	this->ubSpawnSafetyTicks = 7u;

	this->aeNextHeadings.clear();
	this->eCurrentHeading = this->eSpawnHeading;

} // startSpawning


SurfaceCell *Worm::tailCell() {

	return this->uiTargetLength <= this->apCells.length() ? this->apCells.last()
														  : new SurfaceCell();

} // tailCell



}	} // namespace SwissalpS::QtNibblers
