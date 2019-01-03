#include "Worm.h"



namespace SwissalpS { namespace QtNibblers {



Worm::Worm(SurfaceCell *pCell, const quint8 ubColour, const bool bAI,
		   QObject *pParent) :
	QObject(pParent),
	bAmAI(bAI),
	ubColourIndex(ubColour),
	pCellSpawn(pCell) {

	this->apCells.clear();
	this->apCells.append(pCell);

	// determine initial heading from spawn-cell
	switch (pCell->getState()) {
		case 90u: this->eCurrentHeading = North; break;
		case 91u: this->eCurrentHeading = West; break;
		case 92u: this->eCurrentHeading = South; break;

		case 93u:
		default:
			this->eCurrentHeading = East;
		break;

	} // switch pCell->getState()

} // construct


Worm::~Worm() {

	this->apCells.clear();

} // dealloc



}	} // namespace SwissalpS::QtNibblers
