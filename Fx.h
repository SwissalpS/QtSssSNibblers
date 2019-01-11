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
	static Fx *pFx;

	QHash<const Sounds, QSoundEffect*>hSounds;
	static QHash<const Sounds, QSoundEffect*>aoSounds;


protected:

public:
	explicit Fx(QObject *pParent = nullptr);
	virtual ~Fx();

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline virtual void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Fx:" + sMessage); }

	static void play(const Sounds eSound);
	virtual void playSound(const Sounds eSound);

}; // Fx



}	} // namespace SwissalpS::QtNibblers



#endif // FX_H
