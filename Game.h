/*
 * QtSssSNibblers: SwissalpS Nibbles written with Qt-Framework
 * Copyright (C) 2018-2019 Luke J. Zimmermann aka SwissalpS <SwissalpS@LukeZimmermann.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include "AppSettings.h"
#include "Bonus.h"
#include "HistoryItem.h"
#include "MapGame.h"
#include "Worm.h"
#include "WormAI.h"



namespace SwissalpS { namespace QtNibblers {



class Game : public QObject {

	Q_OBJECT

private:

protected:
	bool bGameStarted;
	bool bLevelStarted;
	bool bPaused;
	bool bUseFakes;
	quint8 ubCountAllPlayers;
	quint8 ubCountApplesLeft;
	quint8 ubCountBonus;
	quint8 ubCountBonusLeft;
	quint8 ubCountBonusMissed;
	quint8 ubCountDead;
	quint8 ubCountDeadHumans;
	quint8 ubCountHumans;
	quint8 ubCountLevels;
	quint8 ubCountNeedApple;
	quint8 ubCurrentLevel;
	quint8 ubSpeedIndex;
	quint8 ubStartLevel;
	AppSettings *pAS;
	MapGame *pMapGame;
	QVector<Bonus *> apBonus;
	QVector<Worm *> apWorms;
	QTimer *pTimer;
	QTimer *pTimerBonus;
	WormAI *pWormAI;

	virtual void addBonus(const bool bApple);
	static void addCrashPotential(QHash<QString, Worm *> &hppCrashPotential,
								  QVector<Worm *> &apCrashedWorms,
								  QPoint oPoint, Worm *pWorm);

	virtual void destroyBonus(Bonus *pBonus);
	virtual void destructBonuses();
	virtual void destructWorms();
	virtual void gameDone(const bool bGameWon);
	virtual void initWorms();
	virtual void loadCurrentLevel();
	virtual QVector<Worm *> makeRanking();
	virtual void placeBonus(const quint8 ubBonus, const bool bFake);
	virtual void wormAteBonus(Worm *pWorm, const QPoint oPoint);

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
	void advanceWormTo(Worm *pWorm, const QPoint oPoint) const;
	void bonusPlaced(const QVector<QPoint> aoPoints, const quint8 ubBonus, const bool bFake);
	void doGameOver(const QString &sRanking) const;
	void doLevelDone() const;
	void doLevelIsMissingSpawnPoints(const quint8 ubMissing) const;
	void doLevelLoadError() const;
	void doLevelStartCountdown() const;
	void debugMessage(const QString &sMessage) const;
	void move() const;
	void newHistoryItem(HistoryItem *pHI) const;
	void loadLevel(MapGame *pMap, const quint8 ubLevel);
	void spawnWorm(Worm *pWorm) const;
	void statusMessage(const QString &sMessage) const;
	void updateHistory() const;
	void wormAteBonus(Worm *pWorm) const;
	void wormCrashed(Worm *pWorm) const;
	void wormCreated(Worm *pWorm) const;
	void wormsInvalidated() const;

public slots:
	void onBonusPlaced(const QVector<SurfaceCell *> apCells, const bool bFake);
	void onBonusTimedOut(Bonus *pBonus);

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("G:" + sMessage); }

	void onLevelIsLoaded();
	void onNextLevel();
	void onPauseResumeToggled();
	void onPlayerCountChanged(const quint8 ubCountHumans,
							  const quint8 ubCountAIs);
	void onPlayerNameChanged(const quint8 ubWorm, const QString sName);
	void onReset();
	void onResetSoft();
	void onSpeedChanged(const int iIndex);
	void onStartNewGame(const quint8 ubLevel);
	void onTileChanged(const QPoint oPoint, const quint8 ubState);
	void onWormDied(const bool bAI);
	inline void onWormFarted() const { }

}; // Game



}	} // namespace SwissalpS::QtNibblers



#endif // GAME_H
