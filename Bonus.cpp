#include "Bonus.h"



namespace SwissalpS { namespace QtNibblers {



Bonus::Bonus(QVector<SurfaceCell *> apCells, QObject *pParent) :
	QObject(pParent),
	bExpired(false),
	ubStateBase(0xFFu),
	uiTicks(0u),
	apCells(apCells) {

	if (apCells.length()) this->setStateBase(apCells.first()->getState());

} // construct


Bonus::~Bonus() {

	this->apCells.clear();

} // dealloc


void Bonus::defreezeCells() {

	this->bExpired = true;
	this->uiTicks = 0u;

	if (4 != this->apCells.length()) {
		this->onDebugMessage("Error with cell count in Bonus::defreezeCells");
		return;
	} // if length insufficient

	SurfaceCell *pCell = this->apCells.first();
	if (pCell->getState() == this->ubStateBase) { pCell->defrostState(); this->onDebugMessage("OK");}
	else this->onDebugMessage("cell has other state " + QString::number(this->ubStateBase) + " " + QString::number(pCell->getState()));
	pCell = this->apCells.at(1);
	if (pCell->getState() == this->ubStateBase + 1u) { pCell->defrostState(); this->onDebugMessage("OK.");}
	else this->onDebugMessage("cell has other state." + QString::number(this->ubStateBase+1u) + " " + QString::number(pCell->getState()));
	pCell = this->apCells.at(2);
	if (pCell->getState() == this->ubStateBase + 2u) { pCell->defrostState(); this->onDebugMessage("OK..");}
	else this->onDebugMessage("cell has other state.." + QString::number(this->ubStateBase+2u) + " " + QString::number(pCell->getState()));
	pCell = this->apCells.last();
	if (pCell->getState() == this->ubStateBase + 3u) { pCell->defrostState(); this->onDebugMessage("OK...");}
	else this->onDebugMessage("cell has other state..." + QString::number(this->ubStateBase + 3u) + " " + QString::number(pCell->getState()));

} // defreezeCells


void Bonus::setStateBase(const quint8 &ubState) {

	this->ubStateBase = ubState;

} // setStateBase


void Bonus::onGotEaten() {

	this->defreezeCells();

	Q_EMIT this->gotEaten();

} // onGotEaten



}	} // namespace SwissalpS::QtNibblers
