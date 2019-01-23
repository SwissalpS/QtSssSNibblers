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
#include "PersistantObject.h"

#include <QFile>



namespace SwissalpS { namespace QtNibblers {



PersistantObject::PersistantObject(const QString sPath, QObject *pParent) :
	QObject(pParent),
	bChanged(false),
	bOK(false),
	sPathJSON(sPath) {

	QJsonObject oJo;

	QFile oFile(this->sPathJSON);
	if (oFile.open(QIODevice::ReadOnly)) {

		QByteArray aubData = oFile.readAll();
		oFile.close();

		this->oJdoc = QJsonDocument::fromJson(aubData);

	} else {

		// file does not exist (or we have a permission issue)
		this->oJdoc = QJsonDocument();
		//oJo.insert("sDummy", "dummy");
		this->oJdoc.setObject(oJo);
		this->bChanged = true;

		// if we can't save, we really have an issue
		if (!this->save()) return;

		if (oFile.open(QIODevice::ReadOnly)) {

			QByteArray aubData = oFile.readAll();
			oFile.close();

			this->oJdoc = QJsonDocument::fromJson(aubData);

		} else {

			return;

		} // if able to read file now

	} // if able to read file

	// we really need an object
	if (!this->oJdoc.isObject()) return;

	this->bOK = true;

} // construct


PersistantObject::~PersistantObject() {

} // dealloc


bool PersistantObject::save() {

	if (!this->bChanged) return true;

	QByteArray oData = this->toJSON();

	QFile oFile(this->sPathJSON);
	if (oFile.open(QIODevice::WriteOnly)) {

		oFile.write(oData);
		oFile.close();

	} else {

		this->onDebugMessage(tr("Can NOT save to: ") + this->sPathJSON);

		return false;

	} // if can open for writing

	this->bChanged = false;

	return true;

} // save



}	} // namespace SwissalpS::QtNibblers
