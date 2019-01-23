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
#ifndef SURFACEGAME_H
#define SURFACEGAME_H

#include <QFrame>
#include <QKeyEvent>
#include <QTimer>

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
	QTimer *pTimerResize;
	quint8 ubResizeCount;

private slots:
	void on_buttonPP_toggled(bool bChecked);
	void on_buttonSR_clicked();

protected:
	bool bLevelDone;
	bool bLevelLoading;
	bool bProtectPP;
	QList<QList<SurfaceCell *> > aopRows;
	QVector<ScoreBoard *> apScoreBoards;
	QVector<Worm *> apWorms;
	QVector<QHash<QKeySequence, L::Heading>> ahKeys;
	AppSettings *pAS;
	DialogLoad *pDialogLoad;
	FrameStartCountdown *pStartCountDownFrame;
	qint8 ibWormMouse;
	quint8 ubCurrentLevel;
	mutable int iLastHeight;

	void changeEvent(QEvent *pEvent) override;
	virtual void clearSurface();
	virtual void clearSurfaceOf(const quint8 ubState);
	virtual void clearSurfaceOf(const QVector<quint8>aStates);
	virtual void clearSurfaceOfWorm(const quint8 ubWormColourIndex);
	virtual void clearSurfaceOfWorms();
	virtual Map *currentMap();
	virtual void focusInEvent(QFocusEvent *pEvent) override;
	virtual void focusOutEvent(QFocusEvent *pEvent) override;
	virtual SurfaceCell* getCell(const QPoint oPoint);
	virtual SurfaceCell* getCell(const quint8 ubColumn, const quint8 ubRow);
	virtual void keyPressEvent(QKeyEvent *pEvent) override;
	virtual void mousePressEvent(QMouseEvent *pEvent) override;
	virtual void pauseIfRunning();
	virtual void resetButtons();
	virtual void resizeEvent(QResizeEvent *pEvent) override;
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
	inline virtual void onSCDFdone() { this->on_buttonPP_toggled(true); }
	virtual void resizeDelayDone();

public:
	explicit SurfaceGame(QWidget *pParent = nullptr);
	~SurfaceGame() override;
	virtual void init();
	virtual QSize sizeHint() const override;

signals:
	void bonusPlaced(QVector<SurfaceCell *> apCells, const bool bFake) const;
	void debugMessage(const QString &sMessage) const;
	void levelIsLoaded() const;
	void nextLevel() const;
	void pauseResumeToggled() const;
	void resetGame(const bool bInvalidateWorms) const;
	void startNewGame(const quint8 ubLevel) const;
	void statusMessage(const QString &sMessage) const;
	void tileChanged(const QPoint oPoint, const quint8 ubState) const;
	void trailChanged(const int iValue) const;
	void turnWorm(const quint8 ubWorm, const L::Heading eDirection) const;

public slots:
	virtual void onAdvanceWormTo(Worm *pWorm, const QPoint oPoint);
	virtual void onBonusPlaced(const QVector<QPoint> aoPoints,
							   const quint8 ubBonus, const bool bFake);

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SG:" + sMessage); }

	inline void onCellChanged(const QPoint oPoint, const quint8 ubState) const {
		if (!this->bLevelLoading) Q_EMIT this->tileChanged(oPoint, ubState); }

	virtual void onColoursChanged(const QVector<quint8> aubColours);
	virtual void onDoGameOver(const QString &sRanking);
	virtual void onDoLevelDone();
	virtual void onDoLevelIsMissingSpawnPoints(const quint8 ubMissing);
	virtual void onDoLevelLoadError();
	virtual void onDoLevelStartCountdown();
	virtual void onLoadLevel(MapGame *pMap, const quint8 ubLevel);
	virtual void onMainTabChanged(const int iIndex);
	virtual void onMouseLeft();
	virtual void onMouseRight();
	virtual void onPlayerKeyChanged(const quint8 ubWorm,
									const QKeySequence &oKeySequence,
									const L::Heading eHeading);
	virtual void onPlayerRelativeChanged(const quint8 ubWorm, const bool bRelative);
	inline virtual void onPlayerUseMouseChanged() { this->initKeys(); }
	virtual void onQuitting();
	virtual void onSpawnWorm(Worm *pWorm);
	inline virtual void onTrailChanged(const int iValue) {
		Q_EMIT this->trailChanged(iValue); }

	virtual void onWormAteBonus(Worm *pWorm);
	virtual void onWormCrashed(Worm *pWorm);
	virtual void onWormCreated(Worm *pWorm);
	virtual void onWormsInvalidated();

}; // SurfaceGame



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACEGAME_H
