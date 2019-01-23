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
