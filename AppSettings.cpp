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
#include "AppSettings.h"
#include "definitions.h"

#include <QCoreApplication>
#include <QDir>
#include <QMutex>
#include <QStandardPaths>



namespace SwissalpS { namespace QtNibblers {



AppSettings *AppSettings::pSingelton = nullptr;

const QString AppSettings::sSettingBuilderLastBrushIndex = "iBuilderLastBrushIndex";
const QString AppSettings::sSettingBuilderLastLevel = "ubBuilderLastLevel";
const QString AppSettings::sSettingGameBadLevelMode = "ubGameBadLevelMode";
const QString AppSettings::sSettingGameColours = "aGameColours";
const QString AppSettings::sSettingGameCountAIs = "iGameCountAIs";
const QString AppSettings::sSettingGameCountHumans = "iGameCountHumans";
const QString AppSettings::sSettingGameKeyDown = "aGameKeyDown";
const QString AppSettings::sSettingGameKeyLeft = "aGameKeyLeft";
const QString AppSettings::sSettingGameKeyRight = "aGameKeyRight";
const QString AppSettings::sSettingGameKeyUp = "aGameKeyUp";
const QString AppSettings::sSettingGameLimitLives = "bGameLimitLives";
const QString AppSettings::sSettingGameNames = "aGameNames";
const QString AppSettings::sSettingGameFakeBonuses = "bGameFakeBonuses";
const QString AppSettings::sSettingGameLoadSetsStartLevel = "bGameLoadSetsStartLevel";
const QString AppSettings::sSettingHistoryEnableClearAll = "bHistoryEnableClearAll";
const QString AppSettings::sSettingGameOverOnLastDead = "bGameOverOnLastDead";
const QString AppSettings::sSettingGameRelative = "aGameRelative";
const QString AppSettings::sSettingGameSound = "bGameSound";
const QString AppSettings::sSettingGameSpeed = "iGameSpeed0-3";
const QString AppSettings::sSettingGameStartLevel = "ubGameStartLevel0-255";
const QString AppSettings::sSettingGameStartLives = "ubGameStartLives0-255";
const QString AppSettings::sSettingGameTrailLength = "uiGameTrailLength0-65535";
const QString AppSettings::sSettingGameUseMouse = "aGameUseMouse";
const QString AppSettings::sSettingPowerUser = "bPU";
const QString AppSettings::sSettingTabMainIndex = "iMainTabIndex";
const QString AppSettings::sSettingTabSettingIndex = "iSettingTabIndex";
const QString AppSettings::sSettingWindowMainPosition = "windowMainPosition";
const QString AppSettings::sSettingWindowMainSize = "windowMainSize";

const quint8 AppSettings::ubSettingBuilderLastBrushIndexDefault = 0x0u;
const quint8 AppSettings::ubSettingBuilderLastLevelDefault = 0x0u;
const quint8 AppSettings::ubSettingGameBadLevelModeDefault = 0x0u;
const quint8 AppSettings::ubSettingGameCountAIsDefault = 0x4u;
const quint8 AppSettings::ubSettingGameCountHumansDefault = 0x0u;
const bool AppSettings::bSettingGameLimitLivesDefault = true;
const QString AppSettings::sSettingGameNamesDefault = "Harry;Larry;Sarah;Trisha";
const bool AppSettings::bSettingGameFakeBonusesDefault = false;
const bool AppSettings::bSettingGameOverOnLastDeadDefault = false;
const bool AppSettings::bSettingGameSoundDefault = true;
const qint8 AppSettings::iSettingGameSpeedDefault = 0u;
const quint8 AppSettings::ubSettingGameStartLevelDefault = 0x1u;
const quint8 AppSettings::ubSettingGameStartLivesDefault = 0x6u;
const quint16 AppSettings::uiSettingGameTrailLengthDefault = 0x108u;
const bool AppSettings::bSettingGameLoadSetsStartLevelDefault = false;
const bool AppSettings::bSettingHistoryEnableClearAllDefault = false;
const bool AppSettings::bSettingPowerUserDefault = false;
const qint8 AppSettings::iSettingTabMainIndexDefault = 1u;
const qint8 AppSettings::iSettingTabSettingIndexDefault = 0u;
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
	pS->setValue(sSettingGameBadLevelMode, this->get(sSettingGameBadLevelMode));

	// make sure we have a valid list of colours
	QList<QVariant> aList = pS->value(sSettingGameColours).toList();
	if (8 > aList.length()) {

		aList.clear();

		for (int iWorm = 0; iWorm < 8; ++iWorm)
			aList.append(QVariant(iWorm));

		pS->setValue(sSettingGameColours, aList);

	} // if not valid length list returned, make a new one

	pS->setValue(sSettingGameCountAIs, this->get(sSettingGameCountAIs));
	pS->setValue(sSettingGameCountHumans, this->get(sSettingGameCountHumans));
	pS->setValue(sSettingGameFakeBonuses, this->get(sSettingGameFakeBonuses));
	pS->setValue(sSettingGameLimitLives, this->get(sSettingGameLimitLives));

	QStringList aNames = pS->value(sSettingGameNames).toStringList();
	if (4 > aNames.length()) {

		aNames.clear();

		aNames = sSettingGameNamesDefault.split(";");

		pS->setValue(sSettingGameNames, aNames);

	} // if not valid length list returned, make a new one

	pS->setValue(sSettingGameOverOnLastDead, this->get(sSettingGameOverOnLastDead));

	// make sure key-binding arrays exist
	// Down
	aList = pS->value(sSettingGameKeyDown).toList();
	if (4 > aList.length()) {
		aList.clear();
		aList.append(QVariant("Down"));
		aList.append(QVariant("S"));
		aList.append(QVariant("K"));
		aList.append(QVariant("5"));

		pS->setValue(sSettingGameKeyDown, aList);

	} // if not valid length

	// Left
	aList = pS->value(sSettingGameKeyLeft).toList();
	if (4 > aList.length()) {
		aList.clear();
		aList.append(QVariant("Left"));
		aList.append(QVariant("A"));
		aList.append(QVariant("J"));
		aList.append(QVariant("4"));

		pS->setValue(sSettingGameKeyLeft, aList);

	} // if not valid length

	// Right
	aList = pS->value(sSettingGameKeyRight).toList();
	if (4 > aList.length()) {
		aList.clear();
		aList.append(QVariant("Right"));
		aList.append(QVariant("D"));
		aList.append(QVariant("L"));
		aList.append(QVariant("6"));

		pS->setValue(sSettingGameKeyRight, aList);

	} // if not valid length

	// Up
	aList = pS->value(sSettingGameKeyUp).toList();
	if (4 > aList.length()) {
		aList.clear();
		aList.append(QVariant("Up"));
		aList.append(QVariant("W"));
		aList.append(QVariant("I"));
		aList.append(QVariant("8"));

		pS->setValue(sSettingGameKeyUp, aList);

	} // if not valid length

	pS->setValue(sSettingGameLoadSetsStartLevel, this->get(sSettingGameLoadSetsStartLevel));
	pS->setValue(sSettingGameTrailLength, this->get(sSettingGameTrailLength));

	// make sure the list of relative steering is long enough
	aList = pS->value(sSettingGameRelative).toList();
	if (4 > aList.length()) {
		aList.clear();
		for (int iWorm = 0; iWorm < 4; ++iWorm)
			aList.append(QVariant(false));

		pS->setValue(sSettingGameRelative, aList);

	} // if not valid length

	pS->setValue(sSettingGameSound, this->get(sSettingGameSound));
	pS->setValue(sSettingGameSpeed, this->get(sSettingGameSpeed));
	pS->setValue(sSettingGameStartLevel, this->get(sSettingGameStartLevel));
	pS->setValue(sSettingGameStartLives, this->get(sSettingGameStartLives));

	// make sure the list of mouse steering is long enough
	aList = pS->value(sSettingGameUseMouse).toList();
	if (4 > aList.length()) {
		aList.clear();
		for (int iWorm = 0; iWorm < 4; ++iWorm)
			aList.append(QVariant(false));

		pS->setValue(sSettingGameUseMouse, aList);

	} // if not valid length

	pS->setValue(sSettingHistoryEnableClearAll, this->get(sSettingHistoryEnableClearAll));
	pS->setValue(sSettingTabMainIndex, this->get(sSettingTabMainIndex));
	pS->setValue(sSettingTabSettingIndex, this->get(sSettingTabSettingIndex));
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


void AppSettings::copyResources(QString sPathSource, QString sPathTarget) {

	QDir oPath = QDir("/");
	// first make sure target path exists
	if (!oPath.mkpath(sPathTarget)) {

		this->onDebugMessage(tr("Can not make path: ") + sPathTarget +
							 "\n"
							 + tr("Without it, I can not predict what will happen."));
		return;

	} // if path does not exist and can not be made

	// ensure a trailing slash
	if (!sPathTarget.endsWith("/")) sPathTarget += "/";
	if (!sPathSource.endsWith("/")) sPathSource += "/";
	if (!sPathSource.startsWith(":/")) {

		if (sPathSource.startsWith("/")) {

			sPathSource = ":" + sPathSource;

		} else sPathSource = ":/" + sPathSource;

	} // if not a full source path

	QString sResource;
	QString sPathFileSource, sPathFileTarget;
	foreach (sResource, QDir(sPathSource).entryList()) {

		sPathFileSource = sPathSource + sResource;
		sPathFileTarget = sPathTarget + sResource;

		if (QFileInfo(sPathFileSource).isDir()) {

			this->copyResources(sPathFileSource, sPathFileTarget);

			continue;

		} // if dir encountered

		if (!QFileInfo(sPathFileTarget).exists()) {

			QFile::copy(sPathFileSource, sPathFileTarget);

			// set correct permissions
			QFile(sPathFileTarget).setPermissions(
						QFile::ReadOther | QFile::ReadGroup | QFile::ReadOwner |
						QFile::WriteGroup | QFile::WriteOwner);

		} // if resource file does not exist in target directory

	} // loop all resources source directory

} // copyResources


void AppSettings::drop() {

	static QMutex oMutex;

	oMutex.lock();

	delete pSingelton;
	pSingelton = nullptr;

	oMutex.unlock();

} // drop singelton


QVariant AppSettings::get(const QString sKey) const {

	if (sSettingBuilderLastBrushIndex == sKey) {

		return this->pSettings->value(sKey, ubSettingBuilderLastBrushIndexDefault);

	} else if (sSettingBuilderLastLevel == sKey) {

		return this->pSettings->value(sKey, ubSettingBuilderLastLevelDefault);

	} else if (sSettingGameBadLevelMode == sKey) {

		return this->pSettings->value(sKey, ubSettingGameBadLevelModeDefault);

	} else if (sSettingGameColours == sKey) {

		return this->pSettings->value(sKey);

	} else if (sSettingGameCountAIs == sKey) {

		return this->pSettings->value(sKey, ubSettingGameCountAIsDefault);

	} else if (sSettingGameCountHumans == sKey) {

		return this->pSettings->value(sKey, ubSettingGameCountHumansDefault);

	} else if (sSettingGameFakeBonuses == sKey) {

		return this->pSettings->value(sKey, bSettingGameFakeBonusesDefault);

	} else if (sSettingGameLimitLives == sKey) {

		return this->pSettings->value(sKey, bSettingGameLimitLivesDefault);

	} else if (sSettingGameOverOnLastDead == sKey) {

		return this->pSettings->value(sKey, bSettingGameOverOnLastDeadDefault);

	} else if (sSettingGameRelative == sKey) {

		return this->pSettings->value(sKey);

	} else if (sSettingGameSound == sKey) {

		return this->pSettings->value(sKey, bSettingGameSoundDefault);

	} else if (sSettingGameSpeed == sKey) {

		return this->pSettings->value(sKey, iSettingGameSpeedDefault);

	} else if (sSettingGameStartLevel == sKey) {

		return this->pSettings->value(sKey, ubSettingGameStartLevelDefault);

	} else if (sSettingGameStartLives == sKey) {

		return this->pSettings->value(sKey, ubSettingGameStartLivesDefault);

	} else if (sSettingGameTrailLength == sKey) {

		return this->pSettings->value(sKey, uiSettingGameTrailLengthDefault);

	} else if (sSettingGameUseMouse == sKey) {

		return this->pSettings->value(sKey);

	} else if (sSettingGameLoadSetsStartLevel == sKey) {

		return this->pSettings->value(sKey, bSettingGameLoadSetsStartLevelDefault);

	} else if (sSettingHistoryEnableClearAll == sKey) {

		return this->pSettings->value(sKey, bSettingHistoryEnableClearAllDefault);

	} else if (sSettingTabMainIndex == sKey) {

		return this->pSettings->value(sKey, iSettingTabMainIndexDefault);

	} else if (sSettingTabSettingIndex == sKey) {

		return this->pSettings->value(sKey, iSettingTabSettingIndexDefault);

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


quint8 AppSettings::getPlayerColour(const quint8 ubWorm) const {

	QList<QVariant> aList = this->get(sSettingGameColours).toList();

	if (aList.length() <= ubWorm) return 0u;

	return quint8(aList.at(ubWorm).toUInt());

} // getPlayerColour


quint8 AppSettings::getPlayerByColour(const quint8 ubColour) const {

	QList<QVariant> aList = this->get(sSettingGameColours).toList();
	quint8 ubColourTemp;

	for (quint8 ubWorm = 0u; ubWorm < 8u; ++ubWorm) {

		ubColourTemp = aList.at(ubWorm).toUInt();
		if (ubColour == ubColourTemp) return ubWorm;

	} // loop

	// jic
	return 0u;

} // getPlayerByColour


QString AppSettings::getPlayerKeyDown(const quint8 ubWorm) const {

	QList<QVariant> aList = this->get(sSettingGameKeyDown).toList();

	if (aList.length() <= ubWorm) return "Down";

	return aList.at(ubWorm).toString();

} // getPlayerKeyDown


QString AppSettings::getPlayerKeyLeft(const quint8 ubWorm) const {

	QList<QVariant> aList = this->get(sSettingGameKeyLeft).toList();

	if (aList.length() <= ubWorm) return "Left";

	return aList.at(ubWorm).toString();

} // getPlayerKeyLeft


QString AppSettings::getPlayerKeyRight(const quint8 ubWorm) const {

	QList<QVariant> aList = this->get(sSettingGameKeyRight).toList();

	if (aList.length() <= ubWorm) return "Right";

	return aList.at(ubWorm).toString();

} // getPlayerKeyRight


QString AppSettings::getPlayerKeyUp(const quint8 ubWorm) const {

	QList<QVariant> aList = this->get(sSettingGameKeyUp).toList();

	if (aList.length() <= ubWorm) return "Up";

	return aList.at(ubWorm).toString();

} // getPlayerKeyUp


QString AppSettings::getPlayerName(const quint8 ubWorm) const {

	QStringList aNames = this->get(sSettingGameNames).toStringList();

	if (aNames.length() <= ubWorm) return "Player X";

	return aNames.at(ubWorm);

} // getPlayerName


bool AppSettings::getPlayerRelative(const quint8 ubWorm) const {

	QList<QVariant> aList = this->get(sSettingGameRelative).toList();

	if (aList.length() <= ubWorm) return false;

	return aList.at(ubWorm).toBool();

} // getPlayerRelative


bool AppSettings::getPlayerUseMouse(const quint8 ubWorm) const {

	QList<QVariant> aList = this->get(sSettingGameUseMouse).toList();

	if (aList.length() <= ubWorm) return false;

	return aList.at(ubWorm).toBool();

} // getPlayerUseMouse


QPoint AppSettings::getWindowMainPosition() const {

	return this->get(sSettingWindowMainPosition).toPoint();

} // getWindowMainPosition


QSize AppSettings::getWindowMainSize() const {

	return this->get(sSettingWindowMainSize).toSize();

} // getWindowMainSize


void AppSettings::setPlayerColour(const quint8 ubWorm, const quint8 ubColour) {

	QList<QVariant> aList = this->get(sSettingGameColours).toList();
	QList<QVariant> aListNew;
	quint8 ubColourOld;

	for (int iWorm = 0; iWorm < 8; ++iWorm) {

		ubColourOld = quint8(aList.at(iWorm).toUInt());
		aListNew.append(QVariant((ubWorm == quint8(iWorm)) ? ubColour : ubColourOld));

	} // loop

	this->pSettings->setValue(sSettingGameColours, aListNew);

} // setPlayerColour


void AppSettings::setPlayerKeyDown(const quint8 ubWorm, const QString sKey) const {

	this->onDebugMessage(sKey);
	QList<QVariant> aList = this->get(sSettingGameKeyDown).toList();
	QList<QVariant> aListNew;
	QString sOld;

	for (int iWorm = 0; iWorm < 4; ++iWorm) {

		sOld = aList.at(iWorm).toString();
		aListNew.append(QVariant((ubWorm == quint8(iWorm)) ? sKey : sOld));

	} // loop

	this->pSettings->setValue(sSettingGameKeyDown, aListNew);

} // setPlayerKeyDown


void AppSettings::setPlayerKeyLeft(const quint8 ubWorm, const QString sKey) const {

	this->onDebugMessage(sKey);
	QList<QVariant> aList = this->get(sSettingGameKeyLeft).toList();
	QList<QVariant> aListNew;
	QString sOld;

	for (int iWorm = 0; iWorm < 4; ++iWorm) {

		sOld = aList.at(iWorm).toString();
		aListNew.append(QVariant((ubWorm == quint8(iWorm)) ? sKey : sOld));

	} // loop

	this->pSettings->setValue(sSettingGameKeyLeft, aListNew);

} // setPlayerKeyLeft


void AppSettings::setPlayerKeyRight(const quint8 ubWorm, const QString sKey) const {

	this->onDebugMessage(sKey);
	QList<QVariant> aList = this->get(sSettingGameKeyRight).toList();
	QList<QVariant> aListNew;
	QString sOld;

	for (int iWorm = 0; iWorm < 4; ++iWorm) {

		sOld = aList.at(iWorm).toString();
		aListNew.append(QVariant((ubWorm == quint8(iWorm)) ? sKey : sOld));

	} // loop

	this->pSettings->setValue(sSettingGameKeyRight, aListNew);

} // setPlayerKeyRight


void AppSettings::setPlayerKeyUp(const quint8 ubWorm, const QString sKey) const {

	this->onDebugMessage(sKey);
	QList<QVariant> aList = this->get(sSettingGameKeyUp).toList();
	QList<QVariant> aListNew;
	QString sOld;

	for (int iWorm = 0; iWorm < 4; ++iWorm) {

		sOld = aList.at(iWorm).toString();
		aListNew.append(QVariant((ubWorm == quint8(iWorm)) ? sKey : sOld));

	} // loop

	this->pSettings->setValue(sSettingGameKeyUp, aListNew);

} // setPlayerKeyUp


void AppSettings::setPlayerName(const quint8 ubWorm, const QString sName) const {

	QStringList aNames = this->get(sSettingGameNames).toStringList();

	if (aNames.length() <= ubWorm) return;

	aNames.replace(ubWorm, sName);

	this->pSettings->setValue(sSettingGameNames, aNames);

} // setPlayerName


void AppSettings::setPlayerRelative(const quint8 ubWorm, const bool bChecked) const {

	QList<QVariant> aList = this->get(sSettingGameRelative).toList();
	QList<QVariant> aListNew;
	bool bOld;

	for (int iWorm = 0; iWorm < 4; ++iWorm) {

		bOld = aList.at(iWorm).toBool();
		aListNew.append(QVariant((ubWorm == quint8(iWorm)) ? bChecked : bOld));

	} // loop

	this->pSettings->setValue(sSettingGameRelative, aListNew);

} // getPlayerRelative


void AppSettings::setPlayerUseMouse(const quint8 ubWorm, const bool bChecked) const {

	QList<QVariant> aList = this->get(sSettingGameUseMouse).toList();
	QList<QVariant> aListNew;
	bool bOld;

	for (int iWorm = 0; iWorm < 4; ++iWorm) {

		bOld = aList.at(iWorm).toBool();
		aListNew.append(QVariant((ubWorm == quint8(iWorm)) ? bChecked : bOld));

	} // loop

	this->pSettings->setValue(sSettingGameUseMouse, aListNew);

} // getPlayerUseMouse


void AppSettings::setWindowMainPosition(const QPoint oPos) {

	this->pSettings->setValue(sSettingWindowMainPosition, oPos);

} // setWindowMainPosition


void AppSettings::setWindowMainSize(const QSize oSize) {

	this->pSettings->setValue(sSettingWindowMainSize, oSize);

} // setWindowMainSize



}	} // namespace SwissalpS::QtNibblers
