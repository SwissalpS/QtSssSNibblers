#ifndef SURFACECELL_H
#define SURFACECELL_H

#include <QFrame>
#include "Lingo.h"



namespace Ui {



class SurfaceCell;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class SurfaceCell : public QFrame {
	Q_OBJECT

private:
	Ui::SurfaceCell *pUi;

protected:
	bool bBuilder;
	quint8 ubState; // 0 = empty; 1-99 = player 1-99; 100-199 = bonuses and teleporters; 200-255 = walls
	quint8 ubStateFrozen;
	quint8 ubColumn;
	quint8 ubRow;
	QVector<L::Heading> aeHeadingsBloated;

	void changeEvent(QEvent *pEvent);
	QColor colour() const;
	virtual void mouseReleaseEvent(QMouseEvent *pEvent);
	virtual void paintEvent(QPaintEvent *pEvent);


public:
	explicit SurfaceCell(QWidget *pParent = 0);
	explicit SurfaceCell(bool bBuilder, quint8 ubState,
						 quint8 ubColumn, quint8 ubRow, QWidget *pParent = 0);
	~SurfaceCell();
	inline virtual void defrostState() { this->ubState = this->ubStateFrozen; this->update(); }
	virtual void desnakeState();
	inline virtual void freezeState() { this->ubStateFrozen = this->ubState; }
	inline virtual quint8 getColumn() { return this->ubColumn; }
	inline virtual quint8 getRow() { return this->ubRow; }
	inline virtual quint8 getState() { return this->ubState; }
	inline virtual quint8 getStateFrozen() { return this->ubStateFrozen; }
	inline virtual bool isNull() { return nullptr == this->pUi; }

	inline virtual void setState(const quint8 ubState) { this->ubState = ubState; }

signals:
	void clicked(const quint8 ubColumn, const quint8 ubRow, bool bShift,
				 SurfaceCell *pCell);
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SurfaceCell:" + sMessage); }

}; // SurfaceCell



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACECELL_H
