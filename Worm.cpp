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

	// determine initial heading from spawn-cell
	switch (this->pCellSpawn->getState()) {
		case 90u: this->eCurrentHeading = North; break;
		case 91u: this->eCurrentHeading = West; break;
		case 92u: this->eCurrentHeading = South; break;

		case 93u:
		default:
			this->eCurrentHeading = East;
		break;

	} // switch pCell->getState()

	this->eSpawnHeading = this->eCurrentHeading;

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
		this->apCells.last()->defrostState();
		this->apCells.remove(this->apCells.length() - 1);

	} // loop

	// make new tail
	this->tailCell()->setState(this->tailState());

	if (this->ubSpawnSafetyTicks) this->ubSpawnSafetyTicks--;

} // advanceTo


SurfaceCell *Worm::assCell() {

	return this->uiTargetLength <= this->apCells.length()
			? this->apCells.at(this->apCells.length() - 2)
			: new SurfaceCell();

} // assCell


SurfaceCell *Worm::headCell() {

	return this->apCells.length() ? this->apCells.first()
								  : new SurfaceCell();

} // headCell


SurfaceCell *Worm::neckCell() {

	return 1 < this->apCells.length() ? this->apCells.at(1)
									  : new SurfaceCell();

} // neckCell


QPoint Worm::nextPoint() {

	if (0 == this->apCells.length()) return QPoint();

	SurfaceCell *pCell = this->apCells.first();
	quint8 ubX = pCell->getColumn();
	quint8 ubY = pCell->getRow();

	switch (this->eCurrentHeading) {

		case North:

			if (0u == ubY) {
				ubY = SssS_Nibblers_Surface_Height - 1u;
			} else ubY--;

		break;

		case South:

			ubY++;
			if (SssS_Nibblers_Surface_Height <= ubY) ubY = 0u;

		break;

		case West:

			if (0u == ubX) {
				ubX = SssS_Nibblers_Surface_Width - 1u;
			} else ubX--;

		break;

		case East:

			ubX++;
			if (SssS_Nibblers_Surface_Width <= ubX) ubX = 0u;

		break;

		default:
			this->onDebugMessage(tr("Weird situation in Worm::nextPoint() ")
									+ QString::number(this->eCurrentHeading));
		break;

	} // switch this->eCurrentHeading

	return QPoint(ubX, ubY);

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


void Worm::onSetLives(const quint8 ubLives) {

	this->ubLives = ubLives;

	Q_EMIT this->updateLives(this->ubLives);

	if (this->isDead()) Q_EMIT this->died();

} // onSetLives


void Worm::onSubtractLife() {

	if (this->isDead()) return;

	this->ubLives--;

	Q_EMIT this->updateLives(this->ubLives);

	if (this->isDead()) Q_EMIT this->died();

} // onSubtractLife


void Worm::onTurnLeft() {

	switch (this->eCurrentHeading) {

		case North: this->eCurrentHeading = West; break;

		case South:	this->eCurrentHeading = East; break;

		case West: this->eCurrentHeading = South; break;

		case East: this->eCurrentHeading = North; break;

		default:

			this->onDebugMessage(tr("Weird situation in Worm::onTurnLeft() ")
									+ QString::number(this->eCurrentHeading));
		break;

	} // switch this->eCurrentHeading

} // onTurnLeft


void Worm::onTurnRight() {

	switch (this->eCurrentHeading) {

		case North: this->eCurrentHeading = East; break;

		case South:	this->eCurrentHeading = West; break;

		case West: this->eCurrentHeading = North; break;

		case East: this->eCurrentHeading = South; break;

		default:

			this->onDebugMessage(tr("Weird situation in Worm::onTurnRight() ")
									+ QString::number(this->eCurrentHeading));
		break;

	} // switch this->eCurrentHeading

} // onTurnRight


void Worm::startSpawning() {

	this->apCells.clear();
	this->apCells.prepend(this->pCellSpawn);
	this->uiTargetLength = 5u;
	this->ubSpawnSafetyTicks = 7u;
	this->eCurrentHeading = this->eSpawnHeading;

} // startSpawning


SurfaceCell *Worm::tailCell() {

	return this->uiTargetLength <= this->apCells.length() ? this->apCells.last()
														  : new SurfaceCell();

} // tailCell



}	} // namespace SwissalpS::QtNibblers
