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
#ifndef SRP_RSM_PERSISTANTOBJECT_H
#define SRP_RSM_PERSISTANTOBJECT_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>



namespace SwissalpS { namespace QtNibblers {



class PersistantObject : public QObject {

	Q_OBJECT
	Q_DISABLE_COPY(PersistantObject)

protected:
	bool bChanged;
	bool bOK;
	QString sPathJSON;
	QJsonDocument oJdoc;

public:
	explicit PersistantObject(const QString sPath, QObject *pParent = nullptr);
	virtual ~PersistantObject();

	virtual inline bool hasChanged() const { return this->bChanged; }
	virtual inline bool isOK() const { return this->bOK; }
	virtual bool save();
	virtual inline QByteArray toJSON() const { return this->oJdoc.toJson(); }
	virtual inline QJsonDocument toJSONdocument() const { return this->oJdoc; }
	virtual inline QJsonObject toJSONobject() const { return this->oJdoc.object(); }
	// make sure you pass valid QJsonDocument
	virtual inline void replaceJSONdocument(QJsonDocument oNewDoc) { this->oJdoc = oNewDoc; this->bChanged = true; }

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("PersistantObject:" + sMessage); }

}; // PersistantObject



}	} // namespace SwissalpS::QtNibblers



#endif // SRP_RSM_PERSISTANTOBJECT_H
