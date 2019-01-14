#include "Bonus.h"

#include <QTimer>



namespace SwissalpS { namespace QtNibblers {



Bonus::Bonus(QVector<SurfaceCell *> apCells, const bool bFake, QObject *pParent) :
	QObject(pParent),
	bExpired(false),
	bFake(bFake),
	ubStateBase(0xFCu),
	uiTicks(0u),
	apCells(apCells) {

	if (apCells.length()) this->setStateBase(apCells.first()->getState());

} // construct


Bonus::~Bonus() {

	this->disconnect();
	this->apCells.clear();
	this->bExpired = true;
	this->ubStateBase = 0xFCu;
	this->uiTicks = 0u;

} // dealloc


bool Bonus::contains(const QPoint oPoint) const {

	for (int i = 0; i < this->apCells.length(); ++i) {

		if (oPoint == this->apCells.at(i)->getPos()) return true;

	} // loop

	return false;

} // contains(QPoint)


void Bonus::defreezeCells() {

	this->bExpired = true;
	this->uiTicks = 0u;

	if (4 != this->apCells.length()) {
		this->onDebugMessage("Error with cell count in Bonus::defreezeCells");
		return;
	} // if length insufficient

	// I've checked these, they are not the cause for bonus not leaveing
	SurfaceCell *pCell = this->apCells.first();
	if (pCell->getState() == this->ubStateBase) pCell->defrostState();
	pCell = this->apCells.at(1);
	if (pCell->getState() == this->ubStateBase + 1u) pCell->defrostState();
	pCell = this->apCells.at(2);
	if (pCell->getState() == this->ubStateBase + 2u) pCell->defrostState();
	pCell = this->apCells.last();
	if (pCell->getState() == this->ubStateBase + 3u) pCell->defrostState();

} // defreezeCells


void Bonus::setStateBase(const quint8 &ubState) {

	this->ubStateBase = ubState;

} // setStateBase


void Bonus::onGotEaten() {

	this->defreezeCells();

	Q_EMIT this->gotEaten();

} // onGotEaten


void Bonus::onTick() {

	if (this->hasTimedOut()) return;

	this->uiTicks--;

	if (!this->hasTimedOut()) return;

	this->defreezeCells();

	Q_EMIT this->timedOut(this);

} // onTick



}	} // namespace SwissalpS::QtNibblers
