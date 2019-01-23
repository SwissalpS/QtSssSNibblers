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
#include "ScoreBoard.h"
#include "ui_ScoreBoard.h"
#include "definitions.h"

#include <QLabel>



namespace SwissalpS { namespace QtNibblers {



ScoreBoard::ScoreBoard(const quint8 ubColour, QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::ScoreBoard) {

	this->pUi->setupUi(this);

	this->setColour(ubColour);

	QLabel *pL;
	QPixmap oPM = QPixmap(":/Images/scoreboard_life.svg");
	QGridLayout *pLayout = qobject_cast<QGridLayout *>(this->pUi->frameLives->layout());
	this->apLabels.clear();
	for (int j = 0; j < 2; ++j) {

		for (int i = 0; i < SssS_Nibblers_Max_Lives_Per_Row; ++i) {

			pL = new QLabel(this);
			pL->setScaledContents(true);
			pL->setPixmap(oPM);
			pL->setMaximumSize(32,32);
			pL->setVisible(false);

			this->apLabels.append(pL);

			pLayout->addWidget(pL, j, i);

		} // loop columns

	} // loop rows

} // construct


ScoreBoard::~ScoreBoard() {

	delete this->pUi;

} // dealloc


void ScoreBoard::changeEvent(QEvent *eEvent) {

	QFrame::changeEvent(eEvent);

	switch (eEvent->type()) {

		case QEvent::LanguageChange:

			this->pUi->retranslateUi(this);

		break;

		default:
		break;

	} // switch

} // changeEvent


void ScoreBoard::setColour(const quint8 ubColour) {

	QColor oColour;
	QColor oColourText;
	switch (ubColour) {

		case 0u:
			oColour = SssS_Nibblers_Colour_0;
			oColourText = SssS_Nibblers_Colour_Text_0;
		break;
		case 1u:
			oColour = SssS_Nibblers_Colour_1;
			oColourText = SssS_Nibblers_Colour_Text_1;
		break;
		case 2u:
			oColour = SssS_Nibblers_Colour_2;
			oColourText = SssS_Nibblers_Colour_Text_2;
		break;
		case 3u:
			oColour = SssS_Nibblers_Colour_3;
			oColourText = SssS_Nibblers_Colour_Text_3;
		break;
		case 4u:
			oColour = SssS_Nibblers_Colour_4;
			oColourText = SssS_Nibblers_Colour_Text_4;
		break;
		case 5u:
			oColour = SssS_Nibblers_Colour_5;
			oColourText = SssS_Nibblers_Colour_Text_5;
		break;
		case 6u:
			oColour = SssS_Nibblers_Colour_6;
			oColourText = SssS_Nibblers_Colour_Text_6;
		break;
		case 7u:
			oColour = SssS_Nibblers_Colour_7;
			oColourText = SssS_Nibblers_Colour_Text_7;
		break;

		default:
			oColour = Qt::white;
			oColourText = Qt::black;
		break;

	} // switch ubColour
	QString sBackgroundStyle = "background-color: " + oColour.name() + ";";
	QString sTextStyle = "color: " + oColourText.name() + ";";

	this->pUi->frameTop->setStyleSheet(sBackgroundStyle + sTextStyle);
	this->pUi->frameLives->setStyleSheet(sBackgroundStyle);

} // setColour


void ScoreBoard::setName(const QString sName) {

	this->pUi->labelName->setText(sName);

} // setName


void ScoreBoard::setScore(const quint32 ulScore) {

	this->pUi->labelScore->setText(QString::number(ulScore));

} // setScore


void ScoreBoard::setLives(const quint8 ubLives) {

	for (int i = 0; i < this->apLabels.count(); ++i) {

		this->apLabels.at(i)->setVisible(i < ubLives);

	} // loop

} // setLives



}	} // namespace SwissalpS::QtNibblers
