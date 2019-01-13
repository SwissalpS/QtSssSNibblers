#ifndef WORMAI_H
#define WORMAI_H

#include <QObject>
#include "Lingo.h"
#include "Map.h"
#include "MapGame.h"
#include "Worm.h"



namespace SwissalpS { namespace QtNibblers {



class WormAI : public QObject {

	Q_OBJECT

private:

protected:
	quint8 ubCountDeadendRun;

	const MapGame *pMapGame;
	Map *pMapShaddow;

	virtual bool canMoveTo(Worm *pWorm);
	virtual qint32 deadend(const QPoint oStart, qint32 ilLen);
	virtual qint32 deadendAfter(Worm *pWorm, const QVector<Worm *>apWorms, const qint32 ilLen);
	virtual bool tooClose(Worm *pWorm, const QVector<Worm *>apWorms);
	virtual bool wander(const QPoint oStart, const QPoint oStop,
								 const L::Heading eDirection);

public:
	explicit WormAI(QObject *pParent = nullptr);
	virtual ~WormAI();

	virtual void move(Worm *pWorm, const QVector<Worm *>apWorms, const MapGame *pMap);
	inline virtual void setMap(MapGame *pMapGame) { this->pMapGame = pMapGame; }

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("WormAI:" + sMessage); }

}; // WormAI



}	} // namespace SwissalpS::QtNibblers



#endif // WORMAI_H
