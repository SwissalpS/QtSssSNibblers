#include "History.h"

#include <QJsonArray>

namespace SwissalpS { namespace QtNibblers {



History::History(const QString sPath, QObject *pParent) :
	PersistantObject(sPath, pParent) {

	if (!this->isOK()) return;

} // construct


History::~History() {

} // dealloc


void History::addItem(HistoryItem *pHI) {

	QJsonObject oJo = this->toJSONobject();
	QJsonArray oEntries = oJo.value(sTagEntries).toArray();

	oEntries.append(pHI->toJSON());

	oJo.insert(sTagEntries, oEntries);
	this->oJdoc.setObject(oJo);

	this->bChanged = true;
	this->save();

} // addItem


void History::clear() {

	QJsonArray oEntries;
	QJsonObject oJo = this->toJSONobject();

	oJo.insert(sTagEntries, oEntries);
	this->oJdoc.setObject(oJo);

	this->bChanged = true;
	this->save();

} // clear


QVector<HistoryItem *> History::items() {

	QVector<HistoryItem *>apOut;
	QJsonArray oEntries = this->toJSONobject().value(sTagEntries).toArray();

	for (int i = 0; i < oEntries.count(); ++i) {

		apOut.append(new HistoryItem(oEntries.at(i).toObject(), this));

	} // loop

	return apOut;

} // items



}	} // namespace SwissalpS::QtNibblers
