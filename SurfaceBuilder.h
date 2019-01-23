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
#ifndef SURFACEBUILDER_H
#define SURFACEBUILDER_H

#include <QFrame>
#include "AppSettings.h"
#include "DialogLoad.h"
#include "DialogSave.h"
#include "SurfaceCell.h"



namespace Ui {



class SurfaceBuilder;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class SurfaceBuilder : public QFrame {

	Q_OBJECT

private:
	Ui::SurfaceBuilder *pUi;

private slots:
	void on_buttonClear_clicked();
	void on_buttonSave_clicked();
	void on_buttonLoad_clicked();
	void on_selectTool_currentIndexChanged(int iIndex);

protected:
	QList<QList<SurfaceCell*>> aopRows;
	QVector<SurfaceCell*> apSpawnPoints;
	QHash<quint8, SurfaceCell*> hpTeleporterEntrances;
	QHash<quint8, SurfaceCell*> hpTeleporterExits;
	AppSettings *pAS;
	DialogLoad *pDialogLoad;
	DialogSave *pDialogSave;
	quint8 ubCurrentLevel;
	quint8 ubLastColumn;
	quint8 ubLastRow;

	void changeEvent(QEvent *pEvent);
	virtual void clearSurface();
	virtual void clearSurfaceOf(const quint8 ubState);
	virtual void clearSurfaceOf(const QVector<quint8>aStates);
	virtual QIcon currentBrushIcon() const;
	virtual quint8 currentBrushState() const;
	virtual void loadCurrentLevel();
	virtual void setCellState(SurfaceCell *pCell, const quint8 ubState, const bool bUpdate = true);
	virtual void setCellState(const quint8 ubColumn, const quint8 ubRow, const quint8 ubState, const bool bUpdate = true);

protected slots:
	virtual void dialogLoadFinished(const int iResult);
	virtual void dialogSaveFinished(const int iResult);
	virtual void initBrushes();
	virtual void initCells();

public:
	explicit SurfaceBuilder(QWidget *pParent = nullptr);
	~SurfaceBuilder();
	virtual void init();

signals:
	void cellClicked(const quint8 ubColumn, const quint8 ubRow,
					 bool bShift) const;

	void debugMessage(const QString &sMessage) const;
	void statusMessage(const QString &sMessage) const;

public slots:
	void onCellClicked(const quint8 ubColumn, const quint8 ubRow,
							  bool bShift, SurfaceCell *pCell);

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SurfaceBuilder:" + sMessage); }

}; // SurfaceBuilder



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACEBUILDER_H
