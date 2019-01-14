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
	quint8 ubLivesLost;
	quint8 ubSpawnSafetyTicks;
	quint32 ulScore;
	QPoint oPointSpawn;
	SurfaceCell *pCellSpawn;
	QString sName;
	QVector<SurfaceCell *> apCells;
	QVector<L::Heading> aeNextHeadings;
	L::Heading eCurrentHeading;
	L::Heading eSpawnHeading;
	L::Heading eNextBloat;

	void doNextTurn();

public:

	explicit Worm(const QPoint oPoint, const quint8 ubState,
				  const quint8 ubColour, const bool bAI,
				  QObject *pParent = nullptr);
	virtual ~Worm();

	virtual void addLength(int iLength);
	virtual void advanceTo(SurfaceCell *pCell);
	// cell that will become tail next (2nd-last)
	virtual SurfaceCell *assCell();
	// these may not be used as we may be using 8 of 10 states for heads and tails
	QT_DEPRECATED inline virtual quint8 bloatedNstate() { return (this->ubColourIndex * 10u) + 13u; }
	QT_DEPRECATED inline virtual quint8 bloatedEstate() { return (this->ubColourIndex * 10u) + 14u; }
	QT_DEPRECATED inline virtual quint8 bloatedWstate() { return (this->ubColourIndex * 10u) + 15u; }
	QT_DEPRECATED inline virtual quint8 bloatedSstate() { return (this->ubColourIndex * 10u) + 16u; }
	inline virtual quint8 colourIndex() const { return this->ubColourIndex; }
	inline virtual L::Heading currentDirection() const { return this->eCurrentHeading; }
	// cell that leads the worm (1st)
	virtual SurfaceCell *headCell();
	virtual L::Heading headingLeft() const;
	virtual L::Heading headingRight() const;
	inline virtual quint8 headState() { return (this->ubColourIndex * 10u) + 10u; }
	inline virtual bool isAI() { return this->bAmAI; }
	inline virtual bool isDead() { return 0u == this->ubLives; }
	inline virtual bool isImmune() { return 0u < this->ubSpawnSafetyTicks; }
	inline virtual quint8 midState() { return (this->ubColourIndex * 10u) + 14u; }
	// cell that comes after the head (2nd)
	virtual SurfaceCell *neckCell();
	virtual QPoint leftPoint();
	inline virtual quint8 livesLost() const { return this->ubLivesLost; }
	inline virtual QString name() const { return this->sName; }
	virtual QPoint nextPoint();
	virtual QPoint rightPoint();
	inline virtual quint32 score() const { return this->ulScore; }
	virtual void setColourIndex(const quint8 ubIndex);
	inline virtual void setHeading(const L::Heading eDirection) { this->eCurrentHeading = eDirection; }
	inline virtual void setNextBloatHeading(const L::Heading eDirection) { this->eNextBloat = eDirection; }
	inline virtual void setUseRelativeControls(bool bUse) { this->bUseRelativeControls = bUse; }
	inline virtual QPoint spawnPoint() const { return this->oPointSpawn; }
	virtual void startSpawning();
	// cell that comes last
	virtual SurfaceCell *tailCell();
	inline virtual quint8 tailState() { return (this->ubColourIndex * 10u) + 19u; }
	inline virtual quint16 targetLength() { return this->uiTargetLength; }
	inline virtual bool usesRelativeControls() const { return this->bUseRelativeControls; }

signals:
	void debugMessage(const QString &sMessage) const;
	void died(const bool bAI) const;
	void fart() const;
	void updateColour(const quint8 ubColour) const;
	void updateLives(const quint8 ubLives) const;
	void updateName(const QString sName) const;
	void updateScore(const quint32 ulScore) const;

public slots:
	void onAddLife();
	void onAddScore(const qint16 iScore);
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Worm:" + sMessage); }

	inline void onMultiplyScore(const float fFactor) { this->ulScore *= fFactor; }
	inline void onResetScore() { this->ulScore = 0u; Q_EMIT this->updateScore(0u);}
	void onReverse();
	void onSetLives(const quint8 ubLives);
	inline void onSetName(const QString sName) { this->sName = sName; Q_EMIT this->updateName(sName); }
	QT_DEPRECATED void onSetSpawnCell(SurfaceCell *pCell);
	void onSetSpawnPoint(const QPoint oPoint, const quint8 ubState);
	void onSubtractLife();
	// key press entrance
	void onTurn(const L::Heading eDirection);
	void onTurnLeft();
	void onTurnRight();
	void onGrow(const float fFactor);

}; // Worm



}	} // namespace SwissalpS::QtNibblers



#endif // WORM_H
