#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QObject>
#include <QJsonObject>


namespace SwissalpS { namespace QtNibblers {



class HistoryItem : public QObject {

	Q_OBJECT

private:

protected:
	QString sName;
	quint8 ubCountAI;
	quint8 ubCountHuman;
	quint8 ubLevelsDone;
	quint8 ubLevelStart;
	quint8 ubLivesLost;
	quint8 ubSpeedIndex;
	quint32 ulScore;
	qint64 illTimeStamp;

public:
	const QString sTagCountAI = QStringLiteral("countAI");
	const QString sTagCountHuman = QStringLiteral("countHuman");
	const QString sTagLevelsDone = QStringLiteral("levelsDone");
	const QString sTagLevelStart = QStringLiteral("levelStart");
	const QString sTagLivesLost = QStringLiteral("livesLost");
	const QString sTagName = QStringLiteral("name");
	const QString sTagScore = QStringLiteral("score");
	const QString sTagSpeedIndex = QStringLiteral("speedIndex");
	const QString sTagTimeStamp = QStringLiteral("timeStamp");

	explicit HistoryItem(QObject *pParent = nullptr);
	explicit HistoryItem(const QJsonObject oJSON, QObject *pParent = nullptr);
	explicit HistoryItem(const QString sName, const quint8 ubCountAI,
						 const quint8 ubCountHuman, const quint8 ubLevelsDone,
						 const quint8 ubLevelStart, const quint8 ubLivesLost,
						 const quint8 ubSpeedIndex, const quint32 ulScore,
						 const qint64 illTimeStamp = 0, QObject *pParent = nullptr);
	virtual ~HistoryItem();

	inline virtual quint8 countAI() const { return this->ubCountAI; }
	inline virtual quint8 countHuman() const { return this->ubCountHuman; }
	inline virtual bool isNull() const { return 0xFFu == this->ubSpeedIndex; }
	inline virtual quint8 levelsDone() const { return this->ubLevelsDone; }
	inline virtual quint8 levelStart() const { return this->ubLevelStart; }
	inline virtual quint8 livesLost() const { return this->ubLivesLost; }
	inline virtual QString name() const { return this->sName; }
	inline virtual quint8 score() const { return this->ulScore; }
	inline virtual quint8 speedIndex() const { return this->ubSpeedIndex; }
	inline virtual qint64 timeStamp() const { return this->illTimeStamp; }

	virtual QJsonObject toJSON() const;

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("HI:" + sMessage); }

}; // HistoryItem



}	} // namespace SwissalpS::QtNibblers



#endif // HISTORYITEM_H
