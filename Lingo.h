#ifndef LINGO_H
#define LINGO_H

#include <QObject>

namespace SwissalpS { namespace QtNibblers {



class L : public QObject {
	Q_OBJECT

public:

	enum Heading {
		North = 90u,
		West = 91u,
		South = 92u,
		East = 93u,
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

		SpawnHeadingNorth = 90u,
		SpawnHeadingWest = 91u,
		SpawnHeadingSouth = 92u,
		SpawnHeadingEast = 93u,

		NullTile = 255u

	};
	Q_ENUM(Tiles)

}; // L



}	} // namespace SwissalpS::QtNibblers



#endif // LINGO_H
