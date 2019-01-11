#include "Fx.h"
#include "AppSettings.h"

//#include <QMediaPlayer>

namespace SwissalpS { namespace QtNibblers {


Fx *Fx::pFx = nullptr;

QHash<const Fx::Sounds, QSoundEffect*> Fx::aoSounds;// = QVector<QSoundEffect>();


Fx::Fx(QObject *pParent) :
	QObject(pParent) {

	this->hSounds.clear();

} // construct


Fx::~Fx() {

} // dealloc


// static
void Fx::play(const Sounds eSound) {

	static AppSettings *pAS = AppSettings::pAppSettings();

	if (!pAS->get(AppSettings::sSettingGameSound).toBool()) return;


//	if (nullptr == pFx) pFx = new Fx();

//	pFx->playSound(eSound);

//	return;

//	// works too, but complicated. must first copy files then make playlist.....
//	QMediaPlayer *pMP = new QMediaPlayer();
//	pMP->setMedia(QMediaContent(QUrl::fromLocalFile("/home/luke/gitSwissalpS/QtSssSNibblers/Sounds/appear.ogg")));
//	pMP->play();

//	return;


	// QSoundEffect can't read .ogg files so I had to convert them to .wav
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

} // play


void Fx::playSound(const Sounds eSound) {

	if (!this->hSounds.contains(eSound)) {

		QSoundEffect *pSound = new QSoundEffect(this);
		switch (eSound) {

			case Appear: pSound->setSource(QUrl::fromLocalFile(":/Sounds/appear.ogg"));
			break;

			case Bonus: pSound->setSource(QUrl::fromLocalFile(":/Sounds/bonus.ogg"));
			break;

			case Crash: pSound->setSource(QUrl::fromLocalFile(":/Sounds/crash.ogg"));
			break;

			case GameOver: pSound->setSource(QUrl::fromLocalFile(":/Sounds/gameover.ogg"));
			break;

			case Gobble: pSound->setSource(QUrl::fromLocalFile(":/Sounds/gobble.ogg"));
			break;

			case Life: pSound->setSource(QUrl::fromLocalFile(":/Sounds/life.ogg"));
			break;

			case Reverse: pSound->setSource(QUrl::fromLocalFile(":/Sounds/reverse.ogg"));
			break;

			case Teleport: pSound->setSource(QUrl::fromLocalFile(":/Sounds/teleport.ogg"));
			break;

			case None:
			break;

		} // switch eSound

		pSound->setLoopCount(QSoundEffect::Infinite);
		this->hSounds.insert(eSound, pSound);

	} // if first request for this sound

	this->hSounds.value(eSound)->play();

} // playSound



}	} // namespace SwissalpS::QtNibblers
