#include "AppSettings.h"
#include "definitions.h"

#include <QCoreApplication>
#include <QDir>
#include <QMutex>
#include <QStandardPaths>
#include <QJsonObject>



namespace SwissalpS { namespace QtNibblers {



AppSettings *AppSettings::pSingelton = 0;

const QString AppSettings::sSettingBTautostart = "autoStartScanning";
const QString AppSettings::sSettingChartTab = "chartTabIndex";
const QString AppSettings::sSettingChartView = "chartViewIndex";
const QString AppSettings::sSettingPowerUser = "PU";
const QString AppSettings::sSettingTargetMAC = "targetMAC";
const QString AppSettings::sSettingWindowPositionCharts = "windowPositionCharts";
const QString AppSettings::sSettingWindowPositionDash = "windowPositionDash";
const QString AppSettings::sSettingWindowPositionDevices = "windowPositionDevices";
const QString AppSettings::sSettingWindowPositionExplorer = "windowPositionExplorer";
const QString AppSettings::sSettingWindowPositionLobby = "windowPositionLobby";
const QString AppSettings::sSettingWindowPositionMain = "windowPositionDebug";
const QString AppSettings::sSettingWindowShowCharts = "windowShowCharts";
const QString AppSettings::sSettingWindowShowDash = "windowShowDash";
const QString AppSettings::sSettingWindowShowDevices = "windowShowDevices";
const QString AppSettings::sSettingWindowShowExplorer = "windowShowExplorer";
const QString AppSettings::sSettingWindowShowMain = "windowShowDebug";
const QString AppSettings::sSettingWindowSizeCharts = "windowSizeCharts";
const QString AppSettings::sSettingWindowSizeDash = "windowSizeDash";
const QString AppSettings::sSettingWindowSizeDevices = "windowSizeDevices";
const QString AppSettings::sSettingWindowSizeExplorer = "windowSizeExplorer";
const QString AppSettings::sSettingWindowSizeLobby = "windowSizeLobby";
const QString AppSettings::sSettingWindowSizeMain = "windowSizeDebug";

const bool AppSettings::bSettingBTautostartDefault = true;
const qint8 AppSettings::iSettingChartTabDefault = 0;
const qint8 AppSettings::iSettingChartViewDefault = 0;
const bool AppSettings::bSettingPowerUserDefault = false;
const QString AppSettings::sSettingTargetMACdefault = "";
const QPoint AppSettings::sSettingWindowPositionChartsDefault = QPoint(1821, 151);
const QPoint AppSettings::sSettingWindowPositionDashDefault = QPoint(1780, 208);
const QPoint AppSettings::sSettingWindowPositionDevicesDefault = QPoint(222, 94);
const QPoint AppSettings::sSettingWindowPositionExplorerDefault = QPoint(1815, 178);
const QPoint AppSettings::sSettingWindowPositionLobbyDefault = QPoint(94, 94);
const QPoint AppSettings::sSettingWindowPositionMainDefault = QPoint(1788, 28);
const bool AppSettings::bSettingWindowShowChartsDefault = false;
const bool AppSettings::bSettingWindowShowDashDefault = true;
const bool AppSettings::bSettingWindowShowDevicesDefault = true;
const bool AppSettings::bSettingWindowShowExplorerDefault = false;
const bool AppSettings::bSettingWindowShowMainDefault = false;
const QSize AppSettings::sSettingWindowSizeChartsDefault = QSize(800, 600);
const QSize AppSettings::sSettingWindowSizeDashDefault = QSize(800, 560);
const QSize AppSettings::sSettingWindowSizeDevicesDefault = QSize(648, 316);
const QSize AppSettings::sSettingWindowSizeExplorerDefault = QSize(800, 483);
const QSize AppSettings::sSettingWindowSizeLobbyDefault = QSize(98, 152);
const QSize AppSettings::sSettingWindowSizeMainDefault = QSize(790, 771);


AppSettings::AppSettings(QObject *parent) :
	QObject(parent) {

	// init data path
	QStringList aPaths = QStandardPaths::standardLocations(
							 QStandardPaths::AppDataLocation); //DocumentsLocation);//

	this->sPathDataBase = aPaths.first() + "/"; // + SssS_Nibblers_App_Name_QString + "/";

	// init settings
	QCoreApplication::setOrganizationName(SssS_Nibblers_App_Org_QString);
	QCoreApplication::setOrganizationDomain(SssS_Nibblers_App_Domain_QString);
	QCoreApplication::setApplicationName(SssS_Nibblers_App_Name_QString);

	this->pSettings = new QSettings(this->getDataPath() + "Settings.ini",
							  QSettings::IniFormat, this);

	QSettings *pS = this->pSettings;

	//pS->setValue(sSettingBTautostart, this->get(sSettingBTautostart));
	//pS->setValue(sSettingChartTab, this->get(sSettingChartTab));
	//pS->setValue(sSettingChartView, this->get(sSettingChartView));
	//pS->setValue(sSettingTargetMAC, this->get(sSettingTargetMAC));
	//pS->setValue(sSettingWindowPositionCharts, this->get(sSettingWindowPositionCharts));
	//pS->setValue(sSettingWindowPositionDash, this->get(sSettingWindowPositionDash));
	//pS->setValue(sSettingWindowPositionDevices, this->get(sSettingWindowPositionDevices));
	//pS->setValue(sSettingWindowPositionExplorer, this->get(sSettingWindowPositionExplorer));
	//pS->setValue(sSettingWindowPositionLobby, this->get(sSettingWindowPositionLobby));
	//pS->setValue(sSettingWindowPositionMain, this->get(sSettingWindowPositionMain));
	//pS->setValue(sSettingWindowShowCharts, this->get(sSettingWindowShowCharts));
	//pS->setValue(sSettingWindowShowDash, this->get(sSettingWindowShowDash));
	//pS->setValue(sSettingWindowShowDevices, this->get(sSettingWindowShowDevices));
	//pS->setValue(sSettingWindowShowExplorer, this->get(sSettingWindowShowExplorer));
	//pS->setValue(sSettingWindowShowMain, this->get(sSettingWindowShowMain));
	//pS->setValue(sSettingWindowSizeCharts, this->get(sSettingWindowSizeCharts));
	//pS->setValue(sSettingWindowSizeDash, this->get(sSettingWindowSizeDash));
	//pS->setValue(sSettingWindowSizeDevices, this->get(sSettingWindowSizeDevices));
	//pS->setValue(sSettingWindowSizeExplorer, this->get(sSettingWindowSizeExplorer));
	//pS->setValue(sSettingWindowSizeLobby, this->get(sSettingWindowSizeLobby));
	//pS->setValue(sSettingWindowSizeMain, this->get(sSettingWindowSizeMain));

	this->pSettings->sync();

} // construct


AppSettings::~AppSettings() {

	//pSettings->sync();

} // dealloc


AppSettings *AppSettings::pAppSettings() {

	///
	/// \brief oMutex
	///
	static QMutex oMutex;

	///////////////////////////////
	/* NOTE from: http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	 * Be aware that you should check for a std::bad_alloc exception
	 * to unlock the mutex in case new fails. We are also using
	 * double-checked locking as locking and unlocking a mutex
	 * is expensive and would significantly slow down a call to
	 *  Singleton::instance().
	 */
	if (!AppSettings::pSingelton) {

		oMutex.lock();

		if (!pSingelton) {

			pSingelton = new AppSettings();

		} // if first call

		oMutex.unlock();

	} // if first call

	return pSingelton;

} // singelton access


void AppSettings::drop() {

	static QMutex oMutex;

	oMutex.lock();

	delete pSingelton;
	pSingelton = 0;

	oMutex.unlock();

} // drop singelton


QVariant AppSettings::get(const QString sKey) const {

	if (sSettingBTautostart == sKey) {

		return this->pSettings->value(sKey, bSettingBTautostartDefault);

	} else if (sSettingChartTab == sKey) {

		this->onDebugMessage("@Coder: You may want to use getChartTab(QString sMAC)");

		return this->pSettings->value(sKey, "{}");

	} else if (sSettingChartView == sKey) {

		this->onDebugMessage("@Coder: You may want to use getChartView(QString sMAC)");

		return this->pSettings->value(sKey, "{}");

	} else if (sSettingTargetMAC == sKey) {

		return this->pSettings->value(sKey, sSettingTargetMACdefault);

	} else if (sSettingWindowPositionCharts == sKey) {

		this->onDebugMessage("@Coder: You may want to use getWindowPositionCharts(QString sMAC)");

		return this->pSettings->value(sKey, "{}");

	} else if (sSettingWindowPositionDash == sKey) {

		this->onDebugMessage("@Coder: You may want to use getWindowPositionDash(QString sMAC)");

		return this->pSettings->value(sKey, "{}");

	} else if (sSettingWindowPositionDevices == sKey) {

		return this->pSettings->value(sKey, sSettingWindowPositionDevicesDefault);

	} else if (sSettingWindowPositionExplorer == sKey) {

		return this->pSettings->value(sKey, sSettingWindowPositionExplorerDefault);

	} else if (sSettingWindowPositionLobby == sKey) {

		return this->pSettings->value(sKey, sSettingWindowPositionLobbyDefault);

	} else if (sSettingWindowPositionMain == sKey) {

		return this->pSettings->value(sKey, sSettingWindowPositionMainDefault);

	} else if (sSettingWindowShowCharts == sKey) {

		this->onDebugMessage("@Coder: You may want to use getShowCharts(QString sMAC)");

		return this->pSettings->value(sKey, "{}");

	} else if (sSettingWindowShowDash == sKey) {

		this->onDebugMessage("@Coder: You may want to use getShowDash(QString sMAC)");

		return this->pSettings->value(sKey, "{}");

	} else if (sSettingWindowShowDevices == sKey) {

		return this->pSettings->value(sKey, bSettingWindowShowDevicesDefault);

	} else if (sSettingWindowShowExplorer == sKey) {

		return this->pSettings->value(sKey, bSettingWindowShowExplorerDefault);

	} else if (sSettingWindowShowMain == sKey) {

		return this->pSettings->value(sKey, bSettingWindowShowMainDefault);

	} else if (sSettingWindowSizeCharts == sKey) {

		this->onDebugMessage("@Coder: You may want to use getWindowSizeCharts(QString sMAC)");

		return this->pSettings->value(sKey, "{}");

	} else if (sSettingWindowSizeDash == sKey) {

		this->onDebugMessage("@Coder: You may want to use getWindowSizeDash(QString sMAC)");

		return this->pSettings->value(sKey, "{}");

	} else if (sSettingWindowSizeDevices == sKey) {

		return this->pSettings->value(sKey, sSettingWindowSizeDevicesDefault);

	} else if (sSettingWindowSizeExplorer == sKey) {

		return this->pSettings->value(sKey, sSettingWindowSizeExplorerDefault);

	} else if (sSettingWindowSizeLobby == sKey) {

		return this->pSettings->value(sKey, sSettingWindowSizeLobbyDefault);

	} else if (sSettingWindowSizeMain == sKey) {

		return this->pSettings->value(sKey, sSettingWindowSizeMainDefault);

	} else {

		return this->pSettings->value(sKey);

	} // switch sKey

} // get


int AppSettings::getChartTab(const QString sMAC) const {

	const QJsonObject oJo = this->pSettings->value(
						  sSettingChartTab, "{}").toJsonObject();

	return oJo.value(sMAC).toInt(iSettingChartTabDefault);

} // getChartTab


int AppSettings::getChartView(const QString sMAC) const {

	const QJsonObject oJo = this->pSettings->value(
						  sSettingChartView, "{}").toJsonObject();

	return oJo.value(sMAC).toInt(iSettingChartViewDefault);

} // getChartView


QString AppSettings::getDataPath() const {

	return this->sPathDataBase;

} // getDataPath


bool AppSettings::getShowCharts(const QString sMAC) const {

	const QJsonObject oJo = this->pSettings->value(
						  sSettingWindowShowCharts, "{}").toJsonObject();

	return oJo.value(sMAC).toBool(bSettingWindowShowChartsDefault);

} // getShowCharts


bool AppSettings::getShowDash(const QString sMAC) const {

	const QJsonObject oJo = this->pSettings->value(
						  sSettingWindowShowDash, "{}").toJsonObject();

	return oJo.value(sMAC).toBool(bSettingWindowShowDashDefault);

} // getShowDash


QSettings *AppSettings::getSettings() const {

	return this->pSettings;

} // getSettings


void AppSettings::setSettings(QSettings *pQSettings) {

	this->pSettings = pQSettings;

} // setSettings


QPoint AppSettings::getWindowPositionCharts(const QString sMAC) const {

	const QJsonObject oJo = this->pSettings->value(
						  sSettingWindowPositionCharts, "{}").toJsonObject();

	const QJsonObject oJp = oJo.value(sMAC).toObject();
	const int x = oJp.value("x").toDouble(sSettingWindowPositionChartsDefault.x());
	const int y = oJp.value("y").toDouble(sSettingWindowPositionChartsDefault.y());

	return QPoint(x, y);

} // getWindowPositionCharts


QPoint AppSettings::getWindowPositionDash(const QString sMAC) const {

	const QJsonObject oJo = this->pSettings->value(
						  sSettingWindowPositionDash, "{}").toJsonObject();

	const QJsonObject oJp = oJo.value(sMAC).toObject();
	const int x = oJp.value("x").toDouble(sSettingWindowPositionDashDefault.x());
	const int y = oJp.value("y").toDouble(sSettingWindowPositionDashDefault.y());

	return QPoint(x, y);

} // getWindowPositionDash


QSize AppSettings::getWindowSizeCharts(const QString sMAC) const {

	const QJsonObject oJo = this->pSettings->value(
						  sSettingWindowSizeCharts, "{}").toJsonObject();

	const QJsonObject oJs = oJo.value(sMAC).toObject();
	const int w = oJs.value("w").toDouble(sSettingWindowSizeChartsDefault.width());
	const int h = oJs.value("h").toDouble(sSettingWindowSizeChartsDefault.height());

	return QSize(w, h);

} // getWindowSizeCharts


QSize AppSettings::getWindowSizeDash(const QString sMAC) const {

	const QJsonObject oJo = this->pSettings->value(
						  sSettingWindowSizeDash, "{}").toJsonObject();

	const QJsonObject oJs = oJo.value(sMAC).toObject();
	const int w = oJs.value("w").toDouble(sSettingWindowSizeDashDefault.width());
	const int h = oJs.value("h").toDouble(sSettingWindowSizeDashDefault.height());

	return QSize(w, h);

} // getWindowSizeDash


void AppSettings::setChartTab(const int iPos, const QString sMAC) const {

	QJsonObject oJo = this->pSettings->value(
						  sSettingChartTab, "{}").toJsonObject();

	oJo.insert(sMAC, iPos);

	this->pSettings->setValue(sSettingChartTab, oJo);

} // setChartTab


void AppSettings::setChartView(const int iPos, const QString sMAC) const {

	QJsonObject oJo = this->pSettings->value(
						  sSettingChartView, "{}").toJsonObject();

	oJo.insert(sMAC, iPos);

	this->pSettings->setValue(sSettingChartView, oJo);

} // setChartView


void AppSettings::setShowCharts(const bool bShow, const QString sMAC) {

	QJsonObject oJo = this->pSettings->value(
						  sSettingWindowShowCharts, "{}").toJsonObject();

	oJo.insert(sMAC, bShow);

	this->pSettings->setValue(sSettingWindowShowCharts, oJo);

} // setShowCharts


void AppSettings::setShowDash(const bool bShow, const QString sMAC) {

	QJsonObject oJo = this->pSettings->value(
						  sSettingWindowShowDash, "{}").toJsonObject();

	oJo.insert(sMAC, bShow);

	this->pSettings->setValue(sSettingWindowShowDash, oJo);

} // setShowDash


void AppSettings::setWindowPositionCharts(const QPoint oPos, const QString sMAC) {

	QJsonObject oJo = this->pSettings->value(
						  sSettingWindowPositionCharts, "{}").toJsonObject();

	QJsonObject oJp = oJo.value(sMAC).toObject();

	oJp.insert("x", oPos.x());
	oJp.insert("y", oPos.y());

	oJo.insert(sMAC, oJp);

	this->pSettings->setValue(sSettingWindowPositionCharts, oJo);

} // setWindowPositionCharts


void AppSettings::setWindowPositionDash(const QPoint oPos, const QString sMAC) {

	QJsonObject oJo = this->pSettings->value(
						  sSettingWindowPositionDash, "{}").toJsonObject();

	QJsonObject oJp = oJo.value(sMAC).toObject();

	oJp.insert("x", oPos.x());
	oJp.insert("y", oPos.y());

	oJo.insert(sMAC, oJp);

	this->pSettings->setValue(sSettingWindowPositionDash, oJo);

} // setWindowPositionDash


void AppSettings::setWindowSizeCharts(const QSize oSize, const QString sMAC) {

	QJsonObject oJo = this->pSettings->value(
						  sSettingWindowSizeCharts, "{}").toJsonObject();

	QJsonObject oJs = oJo.value(sMAC).toObject();

	oJs.insert("w", oSize.width());
	oJs.insert("h", oSize.height());

	oJo.insert(sMAC, oJs);

	this->pSettings->setValue(sSettingWindowShowCharts, oJo);

} // setWindowSizeCharts


void AppSettings::setWindowSizeDash(const QSize oSize, const QString sMAC) {

	QJsonObject oJo = this->pSettings->value(
						  sSettingWindowSizeDash, "{}").toJsonObject();

	QJsonObject oJs = oJo.value(sMAC).toObject();

	oJs.insert("w", oSize.width());
	oJs.insert("h", oSize.height());

	oJo.insert(sMAC, oJs);

	this->pSettings->setValue(sSettingWindowShowDash, oJo);

} // setWindowSizeDash



}	} // namespace SwissalpS::QtNibblers
