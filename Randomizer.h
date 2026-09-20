/*
 * QtSssSNibblers: SwissalpS Nibbles written with Qt-Framework
 * Copyright (C) 2018-2026 Luke J. Zimmermann aka SwissalpS <SwissalpS@LukeZimmermann.com>
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
#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <QObject>
#include <QRandomGenerator>

namespace SwissalpS { namespace QtNibblers {



class Randomizer : public QObject {

	Q_OBJECT

public:

	// returns a random int between 0 (inclusive) and iMax (exclusive)
	inline static int bounded(const int iMax) {
		return QRandomGenerator::global()->bounded(iMax);
	} // bounded


	// returns a one out of x chance
	inline static bool chance(const int iChance) {
		return 0 == QRandomGenerator::global()->bounded(iChance);
	} // chance


	// returns a random bool
	inline static bool yeahNah() {
		return 0 == (QRandomGenerator::global()->generate() & 1);
	} // yeahNah


}; // Randomizer



}	} // namespace SwissalpS::QtNibblers



#endif // RANDOMIZER_H
