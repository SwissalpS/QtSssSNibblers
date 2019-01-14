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
