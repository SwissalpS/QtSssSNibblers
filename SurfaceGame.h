#ifndef SURFACEGAME_H
#define SURFACEGAME_H

#include <QFrame>
#include "AppSettings.h"
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
	QList<QList<SurfaceCell *> > aopRows;
	QVector<SurfaceCell *> apSpawnPoints;
	QVector<ScoreBoard *> apScoreBoards;
	QVector<Worm *> apWorms;
	QHash<quint8, SurfaceCell *> hpTeleporterEntrances;
	QHash<quint8, SurfaceCell *> hpTeleporterExits;
	AppSettings *pAS;
	quint8 ubCurrentLevel;
	mutable int iLastHeight;

	void changeEvent(QEvent *pEvent);
	virtual void clearSurface();
	virtual void clearSurfaceOf(const quint8 ubState);
	virtual void clearSurfaceOf(const QVector<quint8>aStates);
	virtual void clearSurfaceOfWorm(const quint8 ubWormColourIndex);
	virtual void clearSurfaceOfWorms();
	virtual SurfaceCell* getCell(const QPoint oPoint);
	virtual SurfaceCell* getCell(const quint8 ubColumn, const quint8 ubRow);
	virtual void loadCurrentLevel();
	void resizeEvent(QResizeEvent *pEvent);
	virtual void setCellState(SurfaceCell *pCell, const quint8 ubState, const bool bUpdate = true);
	virtual void setCellState(const quint8 ubColumn, const quint8 ubRow, const quint8 ubState, const bool bUpdate = true);

protected slots:
	void countdownTick();
	void initCells();
	void initWorms();

public:
	explicit SurfaceGame(QWidget *pParent = 0);
	~SurfaceGame();
	virtual void init();
	virtual QSize sizeHint() const;

signals:
	void wormAteBonus(Worm *pWorm, const quint8 ubBonus) const;
	void debugMessage(const QString &sMessage) const;
	void pauseResumeToggled() const;
	void statusMessage(const QString &sMessage) const;
	void wormCrashed(Worm *pWorm) const;
	void wormCreated(Worm *pWorm) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SG:" + sMessage); }
	void onDoLevelStartCountdown();
	void onMove();
	void onNextLevel();
	void onPlaceBonus(const quint8 ubBonus);
	virtual void onSpawnWorm(const quint8 ubWorm);
	virtual void onSpawnWorm(Worm *pWorm);
	virtual void onSpawnWorms();

}; // SurfaceGame



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACEGAME_H
