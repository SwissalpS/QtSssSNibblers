#ifndef SURFACEGAME_H
#define SURFACEGAME_H

#include <QFrame>
#include <QKeyEvent>
#include "AppSettings.h"
#include "DialogLoad.h"
#include "FrameStartCountdown.h"
#include "Map.h"
#include "MapGame.h"
#include "ScoreBoard.h"
#include "SurfaceCell.h"
#include "Worm.h"



namespace Ui {



class SurfaceGame;



} // namespace Ui


namespace SwissalpS { namespace QtNibblers {



class SurfaceGame : public QFrame {

	Q_OBJECT

private:
	Ui::SurfaceGame *pUi;

private slots:
	void on_buttonPP_toggled(bool bChecked);
	void on_buttonSR_clicked();

protected:
	bool bLevelDone;
	bool bLevelLoading;
	bool bProtectPP;
	QList<QList<SurfaceCell *> > aopRows;
	QVector<SurfaceCell *> apSpawnPoints;
	QVector<ScoreBoard *> apScoreBoards;
	QVector<Worm *> apWorms;
	QHash<quint8, SurfaceCell *> hpTeleporterEntrances;
	QHash<quint8, SurfaceCell *> hpTeleporterExits;
	QVector<QHash<QKeySequence, L::Heading>> ahKeys;
	AppSettings *pAS;
	DialogLoad *pDialogLoad;
	FrameStartCountdown *pStartCountDownFrame;
	quint8 ubCurrentLevel;
	mutable int iLastHeight;
	quint8 ubAIcountDeadendRun;
	//MapGame *pMapGame;
	Map *pAImap;

	static void addCrashPotential(QHash<SurfaceCell *, Worm *> &hppCrashPotential,
								   QVector<Worm *> &apCrashedWorms,
								   SurfaceCell *pCell, Worm *pWorm);

	// old all ai... moved to WormAI
	virtual bool aiCanMoveTo(Worm *pWorm);
	virtual qint32 aiDeadend(const QPoint oStart, qint32 ilLen);
	virtual qint32 aiDeadendAfter(Worm *pWorm, const qint32 ilLen);
	virtual void aiMove(Worm *pWorm);
	virtual bool aiTooClose(Worm *pWorm);
	virtual bool aiWander(const QPoint oStart, const QPoint oStop,
								 const L::Heading eDirection);

	void changeEvent(QEvent *pEvent) override;
	virtual void clearSurface();
	virtual void clearSurfaceOf(const quint8 ubState);
	virtual void clearSurfaceOf(const QVector<quint8>aStates);
	virtual void clearSurfaceOfWorm(const quint8 ubWormColourIndex);
	virtual void clearSurfaceOfWorms();
	virtual Map *currentMap();
	// old
	virtual quint16 findNextGood(const QPoint oStart, const L::Heading eDirection);
	// old
	virtual void findNextMovesForWorm(Worm *pWorm);
	virtual void focusInEvent(QFocusEvent *pEvent) override;
	virtual void focusOutEvent(QFocusEvent *pEvent) override;
	virtual SurfaceCell* getCell(const QPoint oPoint);
	virtual SurfaceCell* getCell(const quint8 ubColumn, const quint8 ubRow);
	virtual void keyPressEvent(QKeyEvent *pEvent) override;
	// old no effect anymore
	virtual void loadCurrentLevel();
	// old
	virtual QVector<quint8> nextPOIinDirection(SurfaceCell *pCell,
											   const L::Heading eDirection);
	virtual void pauseIfRunning();
	// old
	virtual void randomizeSpawns();
	virtual void resetButtons();
	void resizeEvent(QResizeEvent *pEvent) override;
	virtual void setCellState(const QPoint oPoint, const quint8 ubState,
					  const bool bUpdate = true);
	virtual void setCellState(SurfaceCell *pCell, const quint8 ubState,
							  const bool bUpdate = true);
	virtual void setCellState(const quint8 ubColumn, const quint8 ubRow,
							  const quint8 ubState, const bool bUpdate = true);

	virtual void showStartCountDownFrame(const QString sMessage, const QString sButton = QString());
	virtual void updateFrameStartCountdown();

protected slots:
	virtual void countdownTick();
	virtual void dialogLoadFinished(const int iResult);
	virtual void initKeys();
	virtual void initCells();
	virtual void initWorms();
	inline virtual void onSCDFdone() { this->on_buttonPP_toggled(true); }

public:
	explicit SurfaceGame(QWidget *pParent = nullptr);
	~SurfaceGame() override;
	virtual void init();
	virtual QSize sizeHint() const override;

signals:
	void bonusPlaced(QVector<SurfaceCell *> apCells, const bool bFake) const;
	void debugMessage(const QString &sMessage) const;
	void levelIsLoaded() const;
	// old
	void levelIsMissingSpawnPoints() const;
	void nextLevel() const;
	// old
	void noSpaceFoundForBonus(const quint8 ubBonus, const bool bFake) const;
	void pauseResumeToggled() const;
	void resetGame(const bool bInvalidateWorms) const;
	void startNewGame(const quint8 ubLevel) const;
	void statusMessage(const QString &sMessage) const;
	void tileChanged(const QPoint oPoint, const quint8 ubState) const;
	void turnWorm(const quint8 ubWorm, const L::Heading eDirection) const;
	// old
	void wormAteBonus(Worm *pWorm, SurfaceCell *pCell) const;
	// old
	void wormCrashed(Worm *pWorm) const;
	// old
	void wormCreated(Worm *pWorm) const;

public slots:
	virtual void onAdvanceWormTo(Worm *pWorm, const QPoint oPoint);
	virtual void onBonusPlaced(const QVector<QPoint> aoPoints, const quint8 ubBonus, const bool bFake);
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SG:" + sMessage); }

	inline void onCellChanged(const QPoint oPoint, const quint8 ubState) const {
		//this->onDebugMessage("onCC " + QString::number(oPoint.x()) + ":" + QString::number(oPoint.y()) + " " + QString::number(ubState));
		//if (nullptr != this->pMapGame) this->pMapGame->setTile(oPoint, ubState);
		if (this->bLevelLoading) return;
		Q_EMIT this->tileChanged(oPoint, ubState);
	}

	void onColoursChanged(const QVector<quint8> aubColours);
	void onDoGameOver();
	void onDoLevelDone();
	void onDoLevelIsMissingSpawnPoints(const quint8 ubMissing);
	void onDoLevelLoadError();
	void onDoLevelStartCountdown();
	void onLoadLevel(MapGame *pMap, const quint8 ubLevel);
	void onMainTabChanged(const int iIndex);
	// old
	void onMove();
	// old
	void onNextLevel();
	// old
	void onPlaceBonus(const quint8 ubBonus, const bool bFake);
	void onPlayerCountChanged(const quint8 ubCountHumans,
							  const quint8 ubCountAIs);
	void onPlayerKeyChanged(const quint8 ubWorm,
						  const QKeySequence &oKeySequence,
						  const L::Heading eHeading);
	void onPlayerRelativeChanged(const quint8 ubWorm, const bool bRelative);
	void onQuitting();
	virtual void onSpawnWorm(Worm *pWorm);
	// old
	virtual void onSpawnWorms();
	void onWormAteBonus(Worm *pWorm);
	void onWormCrashed(Worm *pWorm);
	void onWormCreated(Worm *pWorm);
	void onWormsInvalidated();

}; // SurfaceGame



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACEGAME_H
