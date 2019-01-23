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
#ifndef LINGO_H
#define LINGO_H

#include "definitions.h"
#include <QObject>
#include <QPoint>

namespace SwissalpS { namespace QtNibblers {



class L : public QObject {

	Q_OBJECT

public:

	enum Heading {
		NorthWest = 0u,
		NorthEast = 1u,
		SouthEast = 2u,
		SouthWest = 3u,

		North = 90u, // ==> Tiles::SpawnHeadingNorth
		West = 91u,
		South = 92u,
		East = 93u,

		Nowhere = 255u,

		LeftUpper = NorthWest,
		RightUpper = NorthEast,
		RightLower = SouthEast,
		LeftLower = SouthWest,

		Up = North,
		Left = West,
		Down = South,
		Right = East
	};
	Q_ENUM(Heading)

	// not in use, was an idea I dropped when I realized
	// it was not required at all to have Tiles with
	// human readable names
	enum Tiles {

		FloorClean = 0u,
		FloorWet1 = 1u,
		FloorWet2 = 2u,
		FloorWet3 = 3u,
		FloorWet4 = 4u,
		FloorWet5 = 5u,
		FloorWet6 = 6u,
		FloorWet7 = 7u,
		FloorWet8 = 8u,
		FloorWet9 = 9u,

		Player_1_Head = 10u,
		Player_1_Mid1 = 11u,
		Player_1_Mid2 = 12u,
		Player_1_Mid3 = 13u,
		Player_1_Mid4 = 14u,
		Player_1_Mid5 = 15u,
		Player_1_Mid6 = 16u,
		Player_1_Mid7 = 17u,
		Player_1_Mid8 = 18u,
		Player_1_Tail = 19u,

		Player_2_Head = 20u,
		Player_2_Mid1 = 21u,
		Player_2_Mid2 = 22u,
		Player_2_Mid3 = 23u,
		Player_2_Mid4 = 24u,
		Player_2_Mid5 = 25u,
		Player_2_Mid6 = 26u,
		Player_2_Mid7 = 27u,
		Player_2_Mid8 = 28u,
		Player_2_Tail = 29u,

		Player_3_Head = 30u,
		Player_3_Mid1 = 31u,
		Player_3_Mid2 = 32u,
		Player_3_Mid3 = 33u,
		Player_3_Mid4 = 34u,
		Player_3_Mid5 = 35u,
		Player_3_Mid6 = 36u,
		Player_3_Mid7 = 37u,
		Player_3_Mid8 = 38u,
		Player_3_Tail = 39u,

		Player_4_Head = 40u,
		Player_4_Mid1 = 41u,
		Player_4_Mid2 = 42u,
		Player_4_Mid3 = 43u,
		Player_4_Mid4 = 44u,
		Player_4_Mid5 = 45u,
		Player_4_Mid6 = 46u,
		Player_4_Mid7 = 47u,
		Player_4_Mid8 = 48u,
		Player_4_Tail = 49u,

		Player_5_Head = 50u,
		Player_5_Mid1 = 51u,
		Player_5_Mid2 = 52u,
		Player_5_Mid3 = 53u,
		Player_5_Mid4 = 54u,
		Player_5_Mid5 = 55u,
		Player_5_Mid6 = 56u,
		Player_5_Mid7 = 57u,
		Player_5_Mid8 = 58u,
		Player_5_Tail = 59u,

		Player_6_Head = 60u,
		Player_6_Mid1 = 61u,
		Player_6_Mid2 = 62u,
		Player_6_Mid3 = 63u,
		Player_6_Mid4 = 64u,
		Player_6_Mid5 = 65u,
		Player_6_Mid6 = 66u,
		Player_6_Mid7 = 67u,
		Player_6_Mid8 = 68u,
		Player_6_Tail = 69u,

		Player_7_Head = 70u,
		Player_7_Mid1 = 71u,
		Player_7_Mid2 = 72u,
		Player_7_Mid3 = 73u,
		Player_7_Mid4 = 74u,
		Player_7_Mid5 = 75u,
		Player_7_Mid6 = 76u,
		Player_7_Mid7 = 77u,
		Player_7_Mid8 = 78u,
		Player_7_Tail = 79u,

		Player_8_Head = 80u,
		Player_8_Mid1 = 81u,
		Player_8_Mid2 = 82u,
		Player_8_Mid3 = 83u,
		Player_8_Mid4 = 84u,
		Player_8_Mid5 = 85u,
		Player_8_Mid6 = 86u,
		Player_8_Mid7 = 87u,
		Player_8_Mid8 = 88u,
		Player_8_Tail = 89u,

		SpawnHeadingNorth = 90u, // ==> Heading::North
		SpawnHeadingWest = 91u,
		SpawnHeadingSouth = 92u,
		SpawnHeadingEast = 93u,

		// 94..99: Reserved tombstones?

		BonusApple = 100u,
		BonusAppleNW = BonusApple,
		BonusAppleNE = 101u,
		BonusAppleSE = 102u,
		BonusAppleSW = 103u,
		// 104..109 Reserved decaying Bonus
		BonusCherry = 110u,
		BonusCherryNW = BonusCherry,
		BonusCherryNE = 111u,
		BonusCherrySE = 112u,
		BonusCherrySW = 113u,
		// 114..119 Reserved decaying Bonus
		BonusBanana = 120u,
		BonusBananaNW = BonusBanana,
		BonusBananaNE = 121u,
		BonusBananaSE = 122u,
		BonusBananaSW = 123u,
		// 124..129 Reserved decaying Bonus
		BonusHeart = 130u,
		BonusHeartNW = BonusHeart,
		BonusHeartNE = 131u,
		BonusHeartSE = 132u,
		BonusHeartSW = 133u,
		// 134..139 Reserved decaying Bonus
		BonusDiamond = 140u,
		BonusDiamondNW = BonusDiamond,
		BonusDiamondNE = 141u,
		BonusDiamondSE = 142u,
		BonusDiamondSW = 143u,
		// 144..149 Reserved decaying Bonus

		// 150..199 Reserved for other bonuses

		WallVertical = 200u,
		WallHorizontal = 201u,
		WallCornerSW = 202u,
		WallCornerSE = 203u,
		WallCornerNW = 204u,
		WallCornerNE = 205u,
		WallTnorth = 206u,
		WallTeast = 207u,
		WallTwest = 208u,
		WallTsouth = 209u,
		WallCross = 210u,

		WallTup = WallTnorth,
		WallTright = WallTeast,
		WallTleft = WallTwest,
		WallTdown = WallTsouth,

		// 211..219 Reserved for other walls

		TeleporterInA = 220u,
		TeleporterOutA = 221u,
		TeleporterInB = 222u,
		TeleporterOutB = 223u,
		TeleporterInC = 224u,
		TeleporterOutC = 225u,
		TeleporterInD = 226u,
		TeleporterOutD = 227u,
		TeleporterInE = 228u,
		TeleporterOutE = 229u,
		TeleporterInF = 230u,
		TeleporterOutF = 231u,
		TeleporterInG = 232u,
		TeleporterOutG = 233u,
		TeleporterInH = 234u,
		TeleporterOutH = 235u,
		TeleporterInI = 236u,
		TeleporterOutI = 237u,
		TeleporterInJ = 238u,
		TeleporterOutJ = 239u,

		// 240..249 Reserved for some future do-dad

		// 250..254 Reserved for some extra stuff

		NullTile = 255u,
		NullCell = NullTile

	};
	Q_ENUM(Tiles)


	inline static Heading headingOfUint(const quint8 ubDirection) {
		switch (ubDirection) {
			case North: return North;
			case South: return South;
			case West: return West;
			case East: return East;
			case NorthWest: return NorthWest;
			case NorthEast: return NorthEast;
			case SouthWest: return SouthWest;
			case SouthEast: return SouthEast;
		} // switch ubDirection
		return Nowhere;
	} // headingOfUint


	// used to determine the heading when snake is reversed
	// first point is new head and 2nd point is the new neck
	inline static Heading oppositeHeading(const QPoint oPointLast,
										  const QPoint oPoint2ndLast) {
		int iDx = oPointLast.x() - oPoint2ndLast.x();
		int iDy = oPointLast.y() - oPoint2ndLast.y();
		if (0 == iDx) {
			// same column
			if (oPointLast.y() < oPoint2ndLast.y()) return North;
			else return South;
		} else if (0 == iDy) {
			// same row
			if (oPointLast.x() < oPoint2ndLast.x()) return West;
			else return East;
		} else {
			// teleporter (or diagonal which doesn't happen)
			return Nowhere;
		} // switch horizontal, vertical or other
	} // oppositeHeading


	inline static Heading oppositeHeading(const Heading eDirection) {
		switch (eDirection) {
			case North: return South;
			case South: return North;
			case West: return East;
			case East: return West;
			case NorthWest: return SouthEast;
			case NorthEast: return SouthWest;
			case SouthWest: return NorthEast;
			case SouthEast: return NorthWest;
			default: break;
		} // switch eDirection
		return Nowhere;
	} // oppositeHeading


	inline static quint8 uintOfHeading(const Heading eDirection) {

		return quint8(eDirection);

	} // uintOfHeading


	inline static QPoint warpPoint(const quint8 ubColumn,
								   const quint8 ubRow,
								   const Heading eDirection) {

		return L::warpPoint(QPoint(ubColumn, ubRow), eDirection);

	} // warpPoint(x,y,Heading)


	inline static QPoint warpPoint(const quint8 ubColumn,
								   const quint8 ubRow,
								   const quint8 ubDirection) {

		return L::warpPoint(QPoint(ubColumn, ubRow),
							L::headingOfUint(ubDirection));

	} // warpPoint(x,y,quint8)


	inline static QPoint warpPoint(const QPoint oOrigin,
								   const quint8 ubDirection) {

		return L::warpPoint(oOrigin, L::headingOfUint(ubDirection));

	} // warpPoint(QPoint,quint8)


	inline static QPoint warpPoint(const QPoint oOrigin,
									   const Heading eDirection) {

		if (oOrigin.x() < 0 || oOrigin.y() < 0
				|| 255 <= oOrigin.x() || 255 <= oOrigin.y())
			return QPoint();

		static const quint8 ubMaxX = SssS_Nibblers_Surface_Width - 1u;
		static const quint8 ubMaxY = SssS_Nibblers_Surface_Height - 1u;

		quint8 ubX = oOrigin.x();
		quint8 ubY = oOrigin.y();

		switch (eDirection) {

			case L::North:

				ubY = (0u == ubY) ? ubMaxY : ubY - 1u;

			break;

			case L::South:

				ubY++;
				if (ubMaxY < ubY) ubY = 0u;

			break;

			case L::West:

				ubX = (0u == ubX) ? ubMaxX : ubX - 1u;

			break;

			case L::East:

				ubX++;
				if (ubMaxX < ubX) ubX = 0u;

			break;
			default:break;

		} // switch eDirection

		return QPoint(ubX, ubY);

	} // warpX(QPoint,Heading)


}; // L



}	} // namespace SwissalpS::QtNibblers



#endif // LINGO_H
