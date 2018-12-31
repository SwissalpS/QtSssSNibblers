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
	static const QString sSettingBTautostart;
	static const QString sSettingChartTab;
	static const QString sSettingChartView;
	static const QString sSettingPowerUser;
	static const QString sSettingWindowShowCharts;
	static const QString sSettingWindowShowDash;
	static const QString sSettingWindowShowDevices;
	static const QString sSettingWindowShowExplorer;
	static const QString sSettingWindowShowMain;
	static const QString sSettingTargetMAC;
	static const QString sSettingTargetMAChey;
	static const QString sSettingWindowSizeCharts;
	static const QString sSettingWindowPositionCharts;
	static const QString sSettingWindowSizeDash;
	static const QString sSettingWindowPositionDash;
	static const QString sSettingWindowSizeDevices;
	static const QString sSettingWindowPositionDevices;
	static const QString sSettingWindowSizeExplorer;
	static const QString sSettingWindowPositionExplorer;
	static const QString sSettingWindowSizeLobby;
	static const QString sSettingWindowPositionLobby;
	static const QString sSettingWindowSizeMain;
	static const QString sSettingWindowPositionMain;

	static const bool bSettingBTautostartDefault;
	static const qint8 iSettingChartTabDefault;
	static const qint8 iSettingChartViewDefault;
	static const bool bSettingPowerUserDefault;
	static const bool bSettingWindowShowChartsDefault;
	static const bool bSettingWindowShowDashDefault;
	static const bool bSettingWindowShowDevicesDefault;
	static const bool bSettingWindowShowExplorerDefault;
	static const bool bSettingWindowShowMainDefault;
	static const QString sSettingTargetMACdefault;
	static const QSize sSettingWindowSizeChartsDefault;
	static const QPoint sSettingWindowPositionChartsDefault;
	static const QSize sSettingWindowSizeDashDefault;
	static const QPoint sSettingWindowPositionDashDefault;
	static const QSize sSettingWindowSizeDevicesDefault;
	static const QPoint sSettingWindowPositionDevicesDefault;
	static const QSize sSettingWindowSizeExplorerDefault;
	static const QPoint sSettingWindowPositionExplorerDefault;
	static const QSize sSettingWindowSizeLobbyDefault;
	static const QPoint sSettingWindowPositionLobbyDefault;
	static const QSize sSettingWindowSizeMainDefault;
	static const QPoint sSettingWindowPositionMainDefault;

	// destroy singelton
	static void drop();
	// public access to singelton instance
	static AppSettings *pAppSettings();
	virtual ~AppSettings();

	QVariant get(const QString sKey) const;
	int getChartTab(const QString sMAC) const;
	int getChartView(const QString sMAC) const;
	QString getDataPath() const;
	bool getShowCharts(const QString sMAC) const;
	bool getShowDash(const QString sMAC) const;

	QSettings *getSettings() const;
	void setSettings(QSettings *pQSettings);

	QPoint getWindowPositionCharts(const QString sMAC) const;
	QPoint getWindowPositionDash(const QString sMAC) const;

	QSize getWindowSizeCharts(const QString sMAC) const;
	QSize getWindowSizeDash(const QString sMAC) const;

	void setChartTab(const int iPos, const QString sMAC) const;
	void setChartView(const int iPos, const QString sMAC) const;

	void setShowCharts(const bool bShow, const QString sMAC);
	void setShowDash(const bool bShow, const QString sMAC);

	void setWindowPositionCharts(const QPoint oPos, const QString sMAC);
	void setWindowPositionDash(const QPoint oPos, const QString sMAC);

	void setWindowSizeCharts(const QSize oSize, const QString sMAC);
	void setWindowSizeDash(const QSize oSize, const QString sMAC);

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
