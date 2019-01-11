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

