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
#ifndef BONUS_H
#define BONUS_H

#include <QObject>
#include "SurfaceCell.h"



namespace SwissalpS { namespace QtNibblers {



class Bonus : public QObject {

	Q_OBJECT

private:

protected:
	bool bExpired;
	bool bFake;
	quint8 ubStateBase;
	uint uiTicks;

	QVector<SurfaceCell *> apCells;

protected slots:
	virtual void defreezeCells();

public:
	explicit Bonus(QVector<SurfaceCell *> apCells, const bool bFake,
				   QObject *pParent = nullptr);
	virtual ~Bonus();

	virtual bool contains(const QPoint oPoint) const;
	inline virtual bool contains(SurfaceCell *pCell) const { return this->apCells.contains(pCell); }
	inline virtual QVector<SurfaceCell *> getCells() const { return this->apCells; }
	inline virtual quint8 getStateBase() const { return this->ubStateBase; }
	inline virtual bool hasExpired() const { return this->bExpired; }
	inline virtual bool hasTimedOut() const { return 0 == this->uiTicks;}
	inline virtual bool isFake() const { return this->bFake; }
	virtual void setStateBase(const quint8 &ubState);
	inline virtual void start(const uint uiTicks) { this->uiTicks = uiTicks;
													this->bExpired = false; }

signals:
	void debugMessage(const QString &sMessage) const;
	void gotEaten() const;
	void timedOut(Bonus *pBonus) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Bonus:" + sMessage); }

	virtual void onGotEaten();
	virtual void onTick();

}; // Bonus



}	} // namespace SwissalpS::QtNibblers



#endif // BONUS_H
