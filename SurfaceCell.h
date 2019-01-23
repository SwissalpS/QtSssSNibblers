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
#ifndef SURFACECELL_H
#define SURFACECELL_H

#include <QFrame>
#include <QTimer>

#include "Lingo.h"



namespace Ui {



class SurfaceCell;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class SurfaceCell : public QFrame {

	Q_OBJECT

private:
	Ui::SurfaceCell *pUi;
	QTimer *pTimer;

protected:
	bool bBuilder;
	quint8 ubState; // L::Tiles
	quint8 ubStateFrozen;
	quint8 ubColumn;
	quint8 ubRow;
	QVector<L::Heading> aeHeadingsBloated;

	void changeEvent(QEvent *pEvent);
	QColor colour() const;
	virtual void mouseReleaseEvent(QMouseEvent *pEvent);
	virtual void paintEvent(QPaintEvent *pEvent);

protected slots:
	virtual void onDesnakeTimer();


public:
	explicit SurfaceCell(QWidget *pParent = nullptr);
	explicit SurfaceCell(bool bBuilder, quint8 ubState,
						 quint8 ubColumn, quint8 ubRow, QWidget *pParent = nullptr);
	~SurfaceCell();

	inline virtual void addBloatedHeading(const L::Heading eHeading) {
		this->aeHeadingsBloated.append(eHeading); }

	virtual void defrostState();
	virtual void desnakeState();
	inline virtual void freezeState() { this->ubStateFrozen = this->ubState; }
	inline virtual quint8 getColumn() const { return this->ubColumn; }
	inline virtual QPoint getPos() const { return QPoint(this->ubColumn, this->ubRow); }
	inline virtual quint8 getRow() const { return this->ubRow; }
	inline virtual quint8 getState() const { return this->ubState; }
	inline virtual quint8 getStateFrozen() const { return this->ubStateFrozen; }
	inline virtual bool isNull() const { return nullptr == this->pUi; }

	inline virtual void setState(const quint8 ubState) {
		this->ubState = ubState; this->onChanged(); }

	bool operator ==(SurfaceCell *pOther) const;

signals:
	void changed(const QPoint oPoint, const quint8 ubState) const;
	void clicked(const quint8 ubColumn, const quint8 ubRow, bool bShift,
				 SurfaceCell *pCell);
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onChanged() const {
		Q_EMIT this->changed(this->getPos(), this->ubState); }

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SurfaceCell:" + sMessage); }

	inline void onTrailChanged(const int iValue) { this->pTimer->setInterval(iValue); }

}; // SurfaceCell



}	} // namespace SwissalpS::QtNibblers



#endif // SURFACECELL_H
