#ifndef WORM_H
#define WORM_H

#include <QObject>
#include "SurfaceCell.h"
#include "Lingo.h"



namespace SwissalpS { namespace QtNibblers {



class Worm : public QObject {
	Q_OBJECT

private:

protected:
	bool bAmAI;
	bool bAmCrashed;
	bool bUseRelativeControls;
	quint16 uiTargetLength;
	quint8 ubColourIndex;
	quint8 ubLives;
	quint8 ubSpawnSafetyTicks;
	quint32 ulScore;
	SurfaceCell *pCellSpawn;
	QVector<SurfaceCell *> apCells;
	QVector<L::Heading> aeNextHeadings;
	L::Heading eCurrentHeading;
	L::Heading eSpawnHeading;

	void doNextTurn();

public:

	explicit Worm(SurfaceCell *pCell, const quint8 ubColour, const bool bAI,
				  QObject *pParent = nullptr);
	virtual ~Worm();

	virtual void advanceTo(SurfaceCell *pCell);
	// cell that will become tail next (2nd-last)
	virtual SurfaceCell *assCell();
	inline virtual quint8 colourIndex() const { return this->ubColourIndex; }
	// cell that leads the worm (1st)
	virtual SurfaceCell *headCell();
	inline virtual bool isDead() { return 0u == this->ubLives; }
	inline virtual bool isImmune() { return 0u < this->ubSpawnSafetyTicks; }
	// cell that comes after the head (2nd)
	virtual SurfaceCell *neckCell();
	virtual void startSpawning();
	// cell that comes last
	virtual SurfaceCell *tailCell();
	// these may not be used as we may be using 8 of 10 states for heads and tails
	inline virtual quint8 bloatedNstate() { return (this->ubColourIndex * 10u) + 13u; }
	inline virtual quint8 bloatedEstate() { return (this->ubColourIndex * 10u) + 14u; }
	inline virtual quint8 bloatedWstate() { return (this->ubColourIndex * 10u) + 15u; }
	inline virtual quint8 bloatedSstate() { return (this->ubColourIndex * 10u) + 16u; }
	inline virtual quint8 headState() { return (this->ubColourIndex * 10u) + 10u; }
	inline virtual quint8 midState() { return (this->ubColourIndex * 10u) + 14u; }
	inline virtual quint8 tailState() { return (this->ubColourIndex * 10u) + 19u; }

	QPoint nextPoint();

signals:
	void debugMessage(const QString &sMessage) const;
	void died() const;
	void updateLives(const quint8 ubLives) const;
	void updateName(const QString sName) const;
	void updateScore(const quint32 ulScore) const;

public slots:
	void onAddLife();
	void onAddScore(const qint16 iScore);
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Worm:" + sMessage); }
	//void onMove();
	void onSetLives(const quint8 ubLives);
	inline void onSetName(const QString sName) { Q_EMIT this->updateName(sName); }
	void onSubtractLife();
	void onTurnLeft();
	void onTurnRight();

}; // Worm



}	} // namespace SwissalpS::QtNibblers



#endif // WORM_H
