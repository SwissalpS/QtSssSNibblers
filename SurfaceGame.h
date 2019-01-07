#ifndef SURFACEGAME_H
#define SURFACEGAME_H

#include <QFrame>
#include <QKeyEvent>
#include "AppSettings.h"
#include "DialogLoad.h"
#include "FrameStartCountdown.h"
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

	void changeEvent(QEvent *pEvent);
	virtual void clearSurface();
	virtual void clearSurfaceOf(const quint8 ubState);
	virtual void clearSurfaceOf(const QVector<quint8>aStates);
	virtual void clearSurfaceOfWorm(const quint8 ubWormColourIndex);
	virtual void clearSurfaceOfWorms();
	virtual void findNextMovesForWorm(Worm *pWorm);
	virtual void focusInEvent(QFocusEvent *pEvent);
	virtual void focusOutEvent(QFocusEvent *pEvent);
	virtual SurfaceCell* getCell(const QPoint oPoint);
	virtual SurfaceCell* getCell(const quint8 ubColumn, const quint8 ubRow);
	virtual void keyPressEvent(QKeyEvent *pEvent) override;
	virtual void loadCurrentLevel();
	virtual QVector<quint8> nextPOIinDirection(SurfaceCell *pCell,
											   const L::Heading eDirection);
	virtual void pauseIfRunning();
	virtual void resetButtons();
	void resizeEvent(QResizeEvent *pEvent);
	virtual void setCellState(SurfaceCell *pCell, const quint8 ubState,
							  const bool bUpdate = true);
	virtual void setCellState(const quint8 ubColumn, const quint8 ubRow,
							  const quint8 ubState, const bool bUpdate = true);

protected slots:
	void countdownTick();
	virtual void dialogLoadFinished(const int iResult);
	void initKeys();
	void initCells();
	void initWorms();

public:
	explicit SurfaceGame(QWidget *pParent = 0);
	~SurfaceGame();
	virtual void init();
	virtual QSize sizeHint() const;

signals:
	void debugMessage(const QString &sMessage) const;
	void noSpaceFoundForBonus(const quint8 ubBonus) const;
	void pauseResumeToggled() const;
	void resetGame() const;
	void statusMessage(const QString &sMessage) const;
	void wormAteBonus(Worm *pWorm, const quint8 ubBonus) const;
	void wormCrashed(Worm *pWorm) const;
	void wormCreated(Worm *pWorm) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SG:" + sMessage); }

	void onColoursChanged(const QVector<quint8> aubColours);
	void onDoGameOver();
	void onDoLevelStartCountdown();
	void onMainTabChanged(const int iIndex);
	void onMove();
	void onNextLevel();
	void onPlaceBonus(const quint8 ubBonus);
	void onPlayerCountChanged(const quint8 ubCountHumans,
							  const quint8 ubCountAIs);
	void onPlayerKeyChanged(const quint8 ubWorm,
						  const QKeySequence &oKeySequence,
						  const L::Heading eHeading);
	void onPlayerRelativeChanged(const quint8 ubWorm, const bool bRelative);
	void onQuitting();
	virtual void onSpawnWorm(Worm *pWorm);
	virtual void onSpawnWorms();

}; // SurfaceGame



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACEGAME_H
