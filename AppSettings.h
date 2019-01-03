#ifndef SssS_FDB_APPSETTINGS_H
#define SssS_FDB_APPSETTINGS_H

#include <QObject>
#include <QPoint>
#include <QSettings>
#include <QSize>



namespace SwissalpS { namespace QtNibblers {



class AppSettings : public QObject {

	Q_OBJECT
	Q_DISABLE_COPY(AppSettings)

private:

	QString sPathDataBase;
	QSettings *pSettings;

	static AppSettings *pSingelton;

	// keep this private as we want only one instance
	// read; http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	explicit AppSettings(QObject *pParent = 0);

	// hide copy constructor or use Q_DISABLE_COPY(AppSettings)
	//explicit AppSettings(const AppSettings &other);
	// hide assign op
	// we leave just the declarations, so the compiler will warn us
	// if we try to use those two functions by accident
	//AppSettings& operator=(const AppSettings &other);

public:
	static const QString sSettingBuilderLastBrushIndex;
	static const QString sSettingBuilderLastLevel;
	static const QString sSettingGameColours;
	static const QString sSettingGameCountAIs;
	static const QString sSettingGameCountHumans;
	static const QString sSettingGameKeyDown;
	static const QString sSettingGameKeyLeft;
	static const QString sSettingGameKeyRight;
	static const QString sSettingGameKeyUp;
	static const QString sSettingGameFakeBonuses;
	static const QString sSettingGameRelative;
	static const QString sSettingGameSound;
	static const QString sSettingGameSpeed;
	static const QString sSettingGameStartLevel;
	static const QString sSettingGameUseMouse;
	static const QString sSettingPowerUser;
	static const QString sSettingTabMainIndex;
	static const QString sSettingTabSettingIndex;
	static const QString sSettingWindowMainPosition;
	static const QString sSettingWindowMainSize;


	static const quint8 ubSettingBuilderLastBrushIndexDefault;
	static const quint8 ubSettingBuilderLastLevelDefault;
	static const quint8 ubSettingGameCountAIsDefault;
	static const quint8 ubSettingGameCountHumansDefault;
	static const bool bSettingGameFakeBonusesDefault;
	static const bool bSettingGameSoundDefault;
	static const quint8 ubSettingGameStartLevelDefault;
	static const qint8 iSettingGameSpeedDefault;
	static const bool bSettingPowerUserDefault;
	static const qint8 iSettingTabMainIndexDefault;
	static const qint8 iSettingTabSettingIndexDefault;
	static const QPoint sSettingWindowMainPositionDefault;
	static const QSize sSettingWindowMainSizeDefault;


	// destroy singelton
	static void drop();
	// public access to singelton instance
	static AppSettings *pAppSettings();
	virtual ~AppSettings();

	QVariant get(const QString sKey) const;
	QString getDataPath() const;
	QSettings *getSettings() const;
	void setSettings(QSettings *pQSettings);

	quint8 getPlayerColour(const quint8 ubWorm) const;
	quint8 getPlayerByColour(const quint8 ubColour) const;
	QString getPlayerKeyDown(const quint8 ubWorm) const;
	QString getPlayerKeyLeft(const quint8 ubWorm) const;
	QString getPlayerKeyRight(const quint8 ubWorm) const;
	QString getPlayerKeyUp(const quint8 ubWorm) const;
	bool getPlayerRelative(const quint8 ubWorm) const;
	bool getPlayerUseMouse(const quint8 ubWorm) const;
	QPoint getWindowMainPosition() const;
	QSize getWindowMainSize() const;

	void setPlayerColour(const quint8 ubWorm, const quint8 ubColour);
	void setPlayerKeyDown(const quint8 ubWorm, const QString sKey) const;
	void setPlayerKeyLeft(const quint8 ubWorm, const QString sKey) const;
	void setPlayerKeyRight(const quint8 ubWorm, const QString sKey) const;
	void setPlayerKeyUp(const quint8 ubWorm, const QString sKey) const;
	void setPlayerRelative(const quint8 ubWorm, const bool bChecked) const;
	void setPlayerUseMouse(const quint8 ubWorm, const bool bChecked) const;
	void setWindowMainPosition(const QPoint oPos);
	void setWindowMainSize(const QSize oSize);

	inline void setValue(const QString &sKey, const QVariant &mValue) {
		this->pSettings->setValue(sKey, mValue); }

	inline void sync() { this->pSettings->sync(); }

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline virtual void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("AS:" + sMessage); }


}; // AppSettings



}	} // namespace SwissalpS::QtNibblers



#endif // SssS_FDB_APPSETTINGS_H
