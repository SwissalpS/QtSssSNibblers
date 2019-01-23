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
#ifndef FX_H
#define FX_H

#include <QObject>
#include <QHash>
#include <QSoundEffect>


namespace SwissalpS { namespace QtNibblers {



class Fx : public QObject {

	Q_OBJECT

public:

	enum Sounds {
		None = 0u,
		Appear = 10u,
		Bonus = 20u,
		Crash = 30u,
		GameOver = 40u,
		Gobble = 50u,
		Life = 60u,
		Reverse = 70u,
		Teleport = 80u
	};
	Q_ENUM(Sounds)

private:
#ifdef QT_MULTIMEDIA_LIB
	static QHash<const Sounds, QSoundEffect*>aoSounds;
#endif
	// hide constructor as this is an all static class
	explicit Fx(QObject *pParent = nullptr);

public:
	virtual ~Fx();

public slots:
	static void play(const Sounds eSound);

}; // Fx



}	} // namespace SwissalpS::QtNibblers



#endif // FX_H
