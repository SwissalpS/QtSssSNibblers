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
#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QObject>
#include <QJsonObject>


namespace SwissalpS { namespace QtNibblers {



class HistoryItem : public QObject {

	Q_OBJECT

private:

protected:
	bool bFakes;
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
	const QString sTagFakes = QStringLiteral("fakes");
	const QString sTagLevelsDone = QStringLiteral("levelsDone");
	const QString sTagLevelStart = QStringLiteral("levelStart");
	const QString sTagLivesLost = QStringLiteral("livesLost");
	const QString sTagName = QStringLiteral("name");
	const QString sTagScore = QStringLiteral("score");
	const QString sTagSpeedIndex = QStringLiteral("speedIndex");
	const QString sTagTimeStamp = QStringLiteral("timeStamp");

	explicit HistoryItem(QObject *pParent = nullptr);
	explicit HistoryItem(const QJsonObject oJSON, QObject *pParent = nullptr);
	explicit HistoryItem(const bool bFakes, const QString sName,
						 const quint8 ubCountAI, const quint8 ubCountHuman,
						 const quint8 ubLevelsDone, const quint8 ubLevelStart,
						 const quint8 ubLivesLost, const quint8 ubSpeedIndex,
						 const quint32 ulScore, const qint64 illTimeStamp = 0,
						 QObject *pParent = nullptr);
	virtual ~HistoryItem();

	inline virtual quint8 countAI() const { return this->ubCountAI; }
	inline virtual quint8 countHuman() const { return this->ubCountHuman; }
	inline virtual bool fakes() const { return this->bFakes; }
	inline virtual bool isNull() const { return 0xFFu == this->ubSpeedIndex; }
	inline virtual quint8 levelsDone() const { return this->ubLevelsDone; }
	inline virtual quint8 levelStart() const { return this->ubLevelStart; }
	inline virtual quint8 livesLost() const { return this->ubLivesLost; }
	inline virtual QString name() const { return this->sName; }
	inline virtual quint32 score() const { return this->ulScore; }
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
