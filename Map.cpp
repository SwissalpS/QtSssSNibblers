#include "Map.h"

#include <QPainter>
#include <QPixmap>



namespace SwissalpS { namespace QtNibblers {



Map::Map(QObject *pParent) :
	QObject(pParent),
	ubTotalColumns(0),
	ubTotalRows(0) {

	this->aaubRows.clear();

} // construct


Map::Map(const quint8 ubColumns, const quint8 ubRows, QObject *pParent) :
	QObject(pParent),
	ubTotalColumns(ubColumns),
	ubTotalRows(ubRows) {

	this->fillAll();

} // construct


Map::~Map() {

	this->aaubRows.clear();

} // dealloc


void Map::fillAll(const quint8 ubState) {

	QVector<quint8>aubRow;
	aubRow.fill(ubState, this->ubTotalColumns);

	this->aaubRows.clear();
	this->aaubRows.fill(aubRow, this->ubTotalRows);

} // fillAll


QPixmap Map::pixmap(const bool bSimple) const {

	// prepare vector of states we react to
	quint8 ubState;
	QVector<quint8> aCOIs;

	for (ubState = L::WallVertical; ubState < 211u; ubState++) aCOIs.append(ubState);

	//if (!bSimple) aCOIs += some more

	QPixmap oPixmap(this->ubTotalColumns, this->ubTotalRows);
	QPainter oP(&oPixmap);

	oP.setBackground(Qt::black);
	oP.fillRect(oPixmap.rect(), Qt::black);
	oP.setPen(Qt::white);

	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;

	for (; ubRows < this->ubTotalRows; ++ubRows) {

		for (ubColumns = 0u; ubColumns < this->ubTotalColumns; ++ubColumns) {

			ubState = this->aaubRows.at(ubRows).at(ubColumns);
			if (aCOIs.contains(ubState)) oP.drawPoint(ubColumns, ubRows);

		} // loop columns

	} // loop rows

	return oPixmap;

} // pixmap


void Map::setTile(const quint8 ubColumn, const quint8 ubRow, quint8 ubState) {

	// check limits
	int iTotalRows = this->aaubRows.length();
	if (0 == iTotalRows) return;
	if (ubRow >= iTotalRows) return;

	int iTotalColumns = this->aaubRows.first().length();
	if (0 == iTotalColumns) return;
	if (ubColumn >= iTotalColumns) return;

	this->aaubRows[ubRow][ubColumn] = ubState;

} // setTile


quint8 Map::tile(const quint8 ubColumn, const quint8 ubRow) const {

	// check limits
	int iTotalRows = this->aaubRows.length();
	if (0 == iTotalRows) return L::NullTile;
	if (ubRow >= iTotalRows) return L::NullTile;

	int iTotalColumns = this->aaubRows.first().length();
	if (0 == iTotalColumns) return L::NullTile;
	if (ubColumn >= iTotalColumns) return L::NullTile;

	return this->aaubRows.at(ubRow).at(ubColumn);

} // tile



}	} // namespace SwissalpS::QtNibblers
