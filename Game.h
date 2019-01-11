#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include "AppSettings.h"
#include "Bonus.h"
#include "Worm.h"



namespace SwissalpS { namespace QtNibblers {



class Game : public QObject {

	Q_OBJECT

private:

protected:
	bool bGameStarted;
	bool bLevelStarted;
	bool bPaused;
	bool bUseFakes;
	quint8 ubCountApplesLeft;
	quint8 ubCountBonus;
	quint8 ubCountBonusLeft;
	quint8 ubCountBonusMissed;
	quint8 ubCountDead;
	quint8 ubCountDeadHumans;
	quint8 ubCountHumans;
	quint8 ubCountLevels;
	quint8 ubCountNeedApple;
	quint8 ubSpeedIndex;
	quint16 uiApplesToGo;
	float fFactorApple;
	float fFactorBanana;
	float fFactorCherry;
	float fFractionApple;
	float fFractionBanana;
	float fFractionCherry;
	AppSettings *pAS;
	QVector<Bonus *> apBonus;
	QVector<Worm *> apWorms;
	QTimer *pTimer;
	QTimer *pTimerBonus;

	virtual void addBonus(const bool bApple);
	virtual void clearExpiredBonuses();
	virtual void destroyBonus(Bonus *pBonus);

protected slots:
	virtual void onTick();
	virtual void onTickBonus();

public:
	explicit Game(QObject *pParent = nullptr);
	virtual ~Game();

	void init();
	virtual bool isGameOver();
	inline virtual bool isPaused() { return this->bPaused; }

signals:
	void doGameOver() const;
	void doLevelDone() const;
	void doLevelStartCountdown() const;
	void debugMessage(const QString &sMessage) const;
	void move() const;
	void nextLevel() const;
	void placeBonus(const quint8 ubBonus, const bool bFake) const;
	void spawnWorm(Worm *pWorm) const;
	void statusMessage(const QString &sMessage) const;

public slots:
	void onBonusPlaced(const QVector<SurfaceCell *> apCells, const bool bFake);
	void onBonusTimedOut(Bonus *pBonus);

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("G:" + sMessage); }

	void onNoSpaceFoundForBonus(const quint8 ubBonus, const bool bFake);
	void onPauseResumeToggled();
	void onPlayerCountChanged(const quint8 ubCountHumans,
							  const quint8 ubCountAIs);
	void onReset();
	void onResetSoft();
	void onSpeedChanged(const int iIndex);
	void onWormAteBonus(Worm *pWorm, SurfaceCell *pCell);
	void onWormCrashed(Worm *pWorm);
	void onWormCreated(Worm *pWorm);
	void onWormDied(const bool bAI);

}; // Game



}	} // namespace SwissalpS::QtNibblers



#endif // GAME_H
