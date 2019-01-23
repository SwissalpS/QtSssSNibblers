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
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QFrame>
#include <QLabel>


namespace Ui {



class ScoreBoard;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class ScoreBoard : public QFrame {

	Q_OBJECT

private:
	Ui::ScoreBoard *pUi;

protected:
	QVector<QLabel *> apLabels;

	void changeEvent(QEvent *eEvent);

public:
	explicit ScoreBoard(const quint8 ubColour, QWidget *pParent = 0);
	~ScoreBoard();

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SB:" + sMessage); }

	virtual void setColour(const quint8 ubColour);
	virtual void setName(const QString sName);
	virtual void setScore(const quint32 ulScore);
	virtual void setLives(const quint8 ubLives);

}; // ScoreBoard



}	} // namespace SwissalpS::QtNibblers



#endif // SCOREBOARD_H
