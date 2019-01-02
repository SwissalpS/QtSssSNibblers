#include "AppSettings.h"
#include "definitions.h"

#include <QCoreApplication>
#include <QDir>
#include <QMutex>
#include <QStandardPaths>



namespace SwissalpS { namespace QtNibblers {



AppSettings *AppSettings::pSingelton = 0;

const QString AppSettings::sSettingBuilderLastBrushIndex = "builderLastBrushIndex";
const QString AppSettings::sSettingBuilderLastLevel = "builderLastLevel";
const QString AppSettings::sSettingPowerUser = "PU";
const QString AppSettings::sSettingTabMain = "mainTabIndex";
const QString AppSettings::sSettingWindowMainPosition = "windowMainPosition";
const QString AppSettings::sSettingWindowMainSize = "windowMainSize";

const quint8 AppSettings::ubSettingBuilderLastBrushIndexDefault = 0x0u;
const quint8 AppSettings::ubSettingBuilderLastLevelDefault = 0x0u;
const bool AppSettings::bSettingPowerUserDefault = false;
const qint8 AppSettings::iSettingTabMainDefault = 1u;
const QPoint AppSettings::sSettingWindowMainPositionDefault = QPoint(94, 94);
const QSize AppSettings::sSettingWindowMainSizeDefault = QSize(800, 600);


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

	pS->setValue(sSettingBuilderLastBrushIndex, this->get(sSettingBuilderLastBrushIndex));
	pS->setValue(sSettingBuilderLastLevel, this->get(sSettingBuilderLastLevel));
	pS->setValue(sSettingTabMain, this->get(sSettingTabMain));
	pS->setValue(sSettingWindowMainPosition, this->get(sSettingWindowMainPosition));
	pS->setValue(sSettingWindowMainSize, this->get(sSettingWindowMainSize));

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

	if (sSettingBuilderLastBrushIndex == sKey) {

		return this->pSettings->value(sKey, ubSettingBuilderLastBrushIndexDefault);

	} else if (sSettingBuilderLastLevel == sKey) {

		return this->pSettings->value(sKey, ubSettingBuilderLastLevelDefault);

	} else if (sSettingTabMain == sKey) {

		return this->pSettings->value(sKey, iSettingTabMainDefault);

	} else if (sSettingWindowMainPosition == sKey) {

		this->onDebugMessage("@Coder: You may want to use getWindowMainPosition()");

		return this->pSettings->value(sKey, sSettingWindowMainPositionDefault);

	} else if (sSettingWindowMainSize == sKey) {

		this->onDebugMessage("@Coder: You may want to use getWindowMainSize()");

		return this->pSettings->value(sKey, sSettingWindowMainSizeDefault);

	} else {

		return this->pSettings->value(sKey);

	} // switch sKey

} // get


QString AppSettings::getDataPath() const {

	return this->sPathDataBase;

} // getDataPath


QSettings *AppSettings::getSettings() const {

	return this->pSettings;

} // getSettings


void AppSettings::setSettings(QSettings *pQSettings) {

	this->pSettings = pQSettings;

} // setSettings


QPoint AppSettings::getWindowMainPosition() const {

	return this->get(sSettingWindowMainPosition).toPoint();

} // getWindowMainPosition


QSize AppSettings::getWindowMainSize() const {

	return this->get(sSettingWindowMainSize).toSize();

} // getWindowMainSize


void AppSettings::setWindowMainPosition(const QPoint oPos) {

	this->pSettings->setValue(sSettingWindowMainPosition, oPos);

} // setWindowMainPosition


void AppSettings::setWindowMainSize(const QSize oSize) {

	this->pSettings->setValue(sSettingWindowMainSize, oSize);

} // setWindowMainSize



}	} // namespace SwissalpS::QtNibblers
