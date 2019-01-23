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
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define SssS_Nibblers_App_Domain_QString QStringLiteral("ws.swissalps.qtssssnibblers")
#define SssS_Nibblers_App_Name_QString QStringLiteral("QtSssSNibblers")
#define SssS_Nibblers_App_Org_QString QStringLiteral("SwissalpS")
#define SssS_Nibblers_App_Version_QString QStringLiteral("0_1")

#define SssS_Nibblers_Bonus_Apple_Lifetime quint16(300u)
#define SssS_Nibblers_Bonus_Apple_Grow float(1.0f)
#define SssS_Nibblers_Bonus_Apple_Points quint16(8u)

#define SssS_Nibblers_Bonus_Banana_Lifetime quint16(150u)
#define SssS_Nibblers_Bonus_Banana_Grow float(1.2f)
#define SssS_Nibblers_Bonus_Banana_Points quint16(33u)

#define SssS_Nibblers_Bonus_Cherry_Lifetime quint16(200u)
#define SssS_Nibblers_Bonus_Cherry_Grow float(0.5f)
#define SssS_Nibblers_Bonus_Cherry_Points quint16(32u)

#define SssS_Nibblers_Bonus_Delay_Ticks quint8(7u)

#define SssS_Nibblers_Bonus_Diamond_Lifetime quint16(150u)
#define SssS_Nibblers_Bonus_Diamond_Grow float(1.0f)
#define SssS_Nibblers_Bonus_Diamond_Points quint16(0u)

#define SssS_Nibblers_Bonus_Heart_Lifetime quint16(100u)
#define SssS_Nibblers_Bonus_Heart_Grow float(1.0f)
#define SssS_Nibblers_Bonus_Heart_Points quint16(0u)

#define SssS_Nibblers_Bonus_Penalty_Miss quint8(1u)
#define SssS_Nibblers_Bonus_Max_Missed quint8(2u)

#define SssS_Nibblers_Colour_0 Qt::blue
#define SssS_Nibblers_Colour_1 Qt::cyan
#define SssS_Nibblers_Colour_2 Qt::darkCyan
#define SssS_Nibblers_Colour_3 Qt::green
#define SssS_Nibblers_Colour_4 Qt::magenta
#define SssS_Nibblers_Colour_5 Qt::red
#define SssS_Nibblers_Colour_6 Qt::yellow
#define SssS_Nibblers_Colour_7 Qt::darkYellow

#define SssS_Nibblers_Colour_Text_0 Qt::white
#define SssS_Nibblers_Colour_Text_1 Qt::black
#define SssS_Nibblers_Colour_Text_2 Qt::white
#define SssS_Nibblers_Colour_Text_3 Qt::black
#define SssS_Nibblers_Colour_Text_4 Qt::white
#define SssS_Nibblers_Colour_Text_5 Qt::white
#define SssS_Nibblers_Colour_Text_6 Qt::black
#define SssS_Nibblers_Colour_Text_7 Qt::white

#define SssS_Nibblers_Game_Start_Countdown quint8(3u)

#define SssS_Nibblers_Max_Key_Cache quint8(64u)
#define SssS_Nibblers_Max_Lives_Per_Row quint8(8u)
#define SssS_Nibblers_Max_Players quint8(8u)
#define SssS_Nibblers_Max_Teleporters quint8(10u)

#define SssS_Nibblers_Speed_Base int(35)
#define SssS_Nibblers_Speed_Bonus_Base int(100)

#define SssS_Nibblers_Surface_Height quint8(66u)
#define SssS_Nibblers_Surface_Minimum_Cell_Side quint8(7u)
#define SssS_Nibblers_Surface_Width quint8(92u)

#endif // DEFINITIONS_H
