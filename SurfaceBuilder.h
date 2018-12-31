#ifndef SURFACEBUILDER_H
#define SURFACEBUILDER_H

#include <QFrame>
#include "SurfaceCell.h"



namespace Ui {



class SurfaceBuilder;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class SurfaceBuilder : public QFrame {
	Q_OBJECT

private:
	Ui::SurfaceBuilder *pUi;

protected:
	QList<QList<SurfaceCell*>> aRows;
	quint8 ubLastColumn;
	quint8 ubLastRow;

	void changeEvent(QEvent *pEvent);
	virtual void toggleCell(SurfaceCell *pCell);
	virtual void toggleCell(const quint8 ubColumn, const quint8 ubRow);

public:
	explicit SurfaceBuilder(QWidget *pParent = nullptr);
	~SurfaceBuilder();
	virtual void initCells();
	//virtual SurfaceCell* getCell(const quint8 ubColumn, const quint8 ubRow);

signals:
	void cellClicked(const quint8 ubColumn, const quint8 ubRow,
					 bool bShift) const;

	void debugMessage(const QString &sMessage) const;

public slots:
	void onCellClicked(const quint8 ubColumn, const quint8 ubRow,
							  bool bShift, SurfaceCell *pCell);

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SurfaceBuilder:" + sMessage); }

private slots:
	void on_buttonClear_clicked();
}; // SurfaceBuilder



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACEBUILDER_H
