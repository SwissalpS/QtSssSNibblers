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

	void on_buttonSetStarts_clicked();

protected:
	QList<QList<SurfaceCell*>> aRows;
	AppSettings *pAS;
	DialogLoad *pDialogLoad;
	DialogSave *pDialogSave;
	quint8 ubCurrentLevel;
	quint8 ubDrawMode;
	quint8 ubLastColumn;
	quint8 ubLastRow;

	void changeEvent(QEvent *pEvent);
	virtual void clearSurface();
	virtual void clearSurfaceOf(const quint8 ubState);
	virtual void clearSurfaceOf(const QVector<quint8>aStates);
	virtual void setCellState(SurfaceCell *pCell, const quint8 ubState);
	virtual void setCellState(const quint8 ubColumn, const quint8 ubRow, const quint8 ubState);
	virtual void toggleCell(SurfaceCell *pCell);
	virtual void toggleCell(const quint8 ubColumn, const quint8 ubRow);

protected slots:
	virtual void dialogLoadFinished(const int iResult);
	virtual void dialogSaveFinished(const int iResult);
	virtual void onCellClickedForStartPoints(const quint8 ubColumn,
											 const quint8 ubRow, SurfaceCell *pCell);

public:
	explicit SurfaceBuilder(QWidget *pParent = nullptr);
	~SurfaceBuilder();
	virtual void initCells();
	//virtual SurfaceCell* getCell(const quint8 ubColumn, const quint8 ubRow);

signals:
	void cellClicked(const quint8 ubColumn, const quint8 ubRow,
					 bool bShift) const;

	void statusMessage(const QString &sMessage) const;
	void debugMessage(const QString &sMessage) const;

public slots:
	void onCellClicked(const quint8 ubColumn, const quint8 ubRow,
							  bool bShift, SurfaceCell *pCell);

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SurfaceBuilder:" + sMessage); }
}; // SurfaceBuilder



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACEBUILDER_H
