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
	bool bLevelStarted;
	bool bPaused;
	quint8 ubCountDead;
	quint8 ubCountLevels;
	AppSettings *pAS;
	QVector<Bonus *> apBonus;
	QVector<Worm *> apWorms;
	QTimer *pTimer;

protected slots:
	virtual void onTick();

public:
	explicit Game(QObject *pParent = nullptr);
	virtual ~Game();

	void init();
	inline virtual bool isGameOver() { return this->ubCountDead >= this->apWorms.length(); }
	inline virtual bool isPaused() { return this->bPaused; }

signals:
	void doGameOver() const;
	void doLevelStartCountdown() const;
	void debugMessage(const QString &sMessage) const;
	void move() const;
	void nextLevel() const;
	void placeBonus(const quint8 ubBonus) const;
	void spawnWorm(Worm *pWorm) const;
	void statusMessage(const QString &sMessage) const;

public slots:
	void onBonusPlaced(const QVector<SurfaceCell *> apCells);
	void onNoSpaceFoundForBonus(const quint8 ubBonus);
	void onPlayerCountChanged(const quint8 ubCountHumans,
							  const quint8 ubCountAIs);
	void onReset();
	void onSpeedChanged(const int iIndex);
	void onWormAteBonus(Worm *pWorm, SurfaceCell *pCell);
	void onWormCrashed(Worm *pWorm);
	void onWormCreated(Worm *pWorm);
	void onWormDied(); //Worm *pWorm);
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("G:" + sMessage); }
	void onPauseResumeToggled();

}; // Game



}	} // namespace SwissalpS::QtNibblers



#endif // GAME_H
