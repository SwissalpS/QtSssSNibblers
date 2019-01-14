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
