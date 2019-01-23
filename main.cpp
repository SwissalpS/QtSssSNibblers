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
#include "MainWindow.h"
#include <QApplication>
#include <QTimer>

int main(int iArgCount, char *aArguments[]) {

	QApplication oApp(iArgCount, aArguments);

	SwissalpS::QtNibblers::MainWindow oMainWindow;
	oMainWindow.show();

	QTimer::singleShot(1, &oMainWindow, SLOT(run()));

	return oApp.exec();

} // main
