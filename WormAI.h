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
/* The methods in this class are strongly based on methods found in the
 * GNOME-Nibbles project which can be found at:
 * https://github.com/GNOME/gnome-nibbles
*/
#ifndef WORMAI_H
#define WORMAI_H

#include <QObject>
#include "Lingo.h"
#include "Map.h"
#include "MapGame.h"
#include "Worm.h"



namespace SwissalpS { namespace QtNibblers {



class WormAI : public QObject {

	Q_OBJECT

private:

protected:
	quint8 ubCountDeadendRun;

	const MapGame *pMapGame;
	Map *pMapShaddow;

	virtual bool canMoveTo(Worm *pWorm);
	virtual qint32 deadend(const QPoint oStart, qint32 ilLen);
	virtual qint32 deadendAfter(Worm *pWorm, const QVector<Worm *>apWorms, const qint32 ilLen);
	virtual bool tooClose(Worm *pWorm, const QVector<Worm *>apWorms);
	virtual bool wander(const QPoint oStart, const QPoint oStop,
								 const L::Heading eDirection);

public:
	explicit WormAI(QObject *pParent = nullptr);
	virtual ~WormAI();

	virtual void move(Worm *pWorm, const QVector<Worm *>apWorms, const MapGame *pMap);
	inline virtual void setMap(MapGame *pMapGame) { this->pMapGame = pMapGame; }

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("WormAI:" + sMessage); }

}; // WormAI



}	} // namespace SwissalpS::QtNibblers



#endif // WORMAI_H
