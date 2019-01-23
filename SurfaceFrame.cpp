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
#include "SurfaceFrame.h"
#include "definitions.h"



SurfaceFrame::SurfaceFrame(QWidget *pParent) :
	QFrame(pParent) {

	this->iLastHeight = this->height();

	this->setMinimumHeight(SssS_Nibblers_Surface_Minimum_Cell_Side * SssS_Nibblers_Surface_Width);
	this->setMinimumWidth(SssS_Nibblers_Surface_Minimum_Cell_Side * SssS_Nibblers_Surface_Width);

} // construct


SurfaceFrame::~SurfaceFrame() {

} // dealloc


void SurfaceFrame::resizeEvent(QResizeEvent *pEvent) {
	Q_UNUSED(pEvent)
	//return QFrame::resizeEvent(pEvent);

	if (this->height() != this->iLastHeight) this->updateGeometry();

} // resizeEvent


QSize SurfaceFrame::sizeHint() const {
	//return QFrame::sizeHint();

	QSize oS(this->size());

	this->iLastHeight = oS.height();

	oS.setWidth((oS.height() * SssS_Nibblers_Surface_Width)
				/ SssS_Nibblers_Surface_Height);
	oS.setHeight(QFrame::sizeHint().height());

	return oS;

} // sizeHint

