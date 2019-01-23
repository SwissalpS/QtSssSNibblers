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
#include "Fx.h"
#include "AppSettings.h"



namespace SwissalpS { namespace QtNibblers {



QHash<const Fx::Sounds, QSoundEffect*> Fx::aoSounds;


Fx::Fx(QObject *pParent) :
	QObject(pParent) {

} // construct


Fx::~Fx() {

} // dealloc


// static
void Fx::play(const Sounds eSound) {
#ifdef QT_MULTIMEDIA_LIB
	static AppSettings *pAS = AppSettings::pAppSettings();

	// don't bother if muted
	if (!pAS->get(AppSettings::sSettingGameSound).toBool()) return;

//	// works too, but complicated. must first copy files then make playlist.....
//	QMediaPlayer *pMP = new QMediaPlayer();
//	pMP->setMedia(QMediaContent(QUrl::fromLocalFile("/home/luke/gitSwissalpS/QtSssSNibblers/Sounds/appear.ogg")));
//	pMP->play();

	// QSoundEffect can't read .ogg files so I had to convert them to .wav
	// I made the teleporter sound less aggresive while I was at it.
	// plus QMediaPlayer is too 'eeh' for this simple task
	if (!Fx::aoSounds.contains(eSound)) {

		QSoundEffect *pSound = new QSoundEffect();
		switch (eSound) {

			case Appear: pSound->setSource(QUrl::fromLocalFile(":/Sounds/appear.wav"));
			break;

			case Bonus: pSound->setSource(QUrl::fromLocalFile(":/Sounds/bonus.wav"));
			break;

			case Crash: pSound->setSource(QUrl::fromLocalFile(":/Sounds/crash.wav"));
			break;

			case GameOver: pSound->setSource(QUrl::fromLocalFile(":/Sounds/gameover.wav"));
			break;

			case Gobble: pSound->setSource(QUrl::fromLocalFile(":/Sounds/gobble.wav"));
			break;

			case Life: pSound->setSource(QUrl::fromLocalFile(":/Sounds/life.wav"));
			break;

			case Reverse: pSound->setSource(QUrl::fromLocalFile(":/Sounds/reverse.wav"));
			break;

			case Teleport: pSound->setSource(QUrl::fromLocalFile(":/Sounds/teleport.wav"));
			break;

			case None:
			break;

		} // switch eSound

		Fx::aoSounds.insert(eSound, pSound);

	} // if first request for this sound

	Fx::aoSounds.value(eSound)->play();

#endif
} // play



}	} // namespace SwissalpS::QtNibblers
