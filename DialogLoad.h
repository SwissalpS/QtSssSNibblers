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
#ifndef DIALOGLOAD_H
#define DIALOGLOAD_H

#include <QDialog>



namespace Ui {



class DialogLoad;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class DialogLoad : public QDialog {

	Q_OBJECT

private:
	Ui::DialogLoad *pUi;

private slots:
	void on_buttonZap0_clicked();
	void on_buttonZap128_clicked();
	void on_buttonZap255_clicked();
	void on_comboBox_currentIndexChanged(int iIndex);

protected:
	void changeEvent(QEvent *pEvent);

public:
	explicit DialogLoad(QWidget *parent = 0);
	~DialogLoad();

	virtual int getSelected() const;
	virtual void setSelected(const int iIndex) const;
	virtual void setWarning(const QString sMessage) const;

}; // DialogLoad



}	} // namespace SwissalpS::QtNibblers




#endif // DIALOGLOAD_H
