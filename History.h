#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include <QVector>

#include "HistoryItem.h"
#include "PersistantObject.h"



namespace SwissalpS { namespace QtNibblers {



class History : public PersistantObject {

	Q_OBJECT
	Q_DISABLE_COPY(History)

private:

protected:

public:
	const QString sTagEntries = QStringLiteral("aEntries");
	const QString sTagVersion = QStringLiteral("sVersion");

	explicit History(const QString sPath, QObject *pParent = nullptr);
	virtual ~History();

	virtual void clear();
	virtual QVector<HistoryItem *> items();

public slots:
	virtual void addItem(HistoryItem *pHI);

}; // History



}	} // namespace SwissalpS::QtNibblers



#endif // HISTORY_H
