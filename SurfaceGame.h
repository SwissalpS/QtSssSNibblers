#ifndef SURFACEGAME_H
#define SURFACEGAME_H

#include <QFrame>
#include "AppSettings.h"
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

protected:
	QList<QList<SurfaceCell *> > aRows;
	QVector<SurfaceCell *> aSpawnPoints;
	QVector<Worm *> aWorms;
	QHash<quint8, SurfaceCell *> hTeleporterEntrances;
	QHash<quint8, SurfaceCell *> hTeleporterExits;
	AppSettings *pAS;
	quint8 ubCurrentLevel;

	void changeEvent(QEvent *pEvent);
	virtual void clearSurface();
	virtual void clearSurfaceOf(const quint8 ubState);
	virtual void clearSurfaceOf(const QVector<quint8>aStates);
	virtual void loadCurrentLevel();
	virtual void setCellState(SurfaceCell *pCell, const quint8 ubState, const bool bUpdate = true);
	virtual void setCellState(const quint8 ubColumn, const quint8 ubRow, const quint8 ubState, const bool bUpdate = true);

protected slots:
	void initCells();
	void initWorms();

public:
	explicit SurfaceGame(QWidget *pParent = 0);
	~SurfaceGame();
	virtual void init();

signals:
	void debugMessage(const QString &sMessage) const;
	void statusMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SG:" + sMessage); }

}; // SurfaceGame



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACEGAME_H
