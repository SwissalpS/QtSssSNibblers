/*
 * QtSssSNibblers: SwissalpS Nibbles written with Qt-Framework
 * Copyright (C) 2018-2026 Luke J. Zimmermann aka SwissalpS <SwissalpS@LukeZimmermann.com>
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
#ifndef SURFACEOVERLAY_H
#define SURFACEOVERLAY_H

#include <QFrame>
#include <QKeyEvent>



namespace Ui {



class SurfaceOverlay;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class SurfaceOverlay : public QFrame {

	Q_OBJECT

private:
	Ui::SurfaceOverlay *pUi;

private slots:

protected:
	virtual void changeEvent(QEvent *pEvent) override;
	virtual void keyPressEvent(QKeyEvent *pEvent) override;
	virtual void mousePressEvent(QMouseEvent *pEvent) override;


public:
	explicit SurfaceOverlay(QWidget *pParent = nullptr);
	~SurfaceOverlay() override;

signals:
	void debugMessage(const QString &sMessage) const;
	void done() const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("FSC:" + sMessage); }

	virtual void onSetText(const QString sText,
						   const QString sTextButton = QString());

}; // SurfaceOverlay



}	} // namespace SwissalpS::QtNibblers

#endif // SURFACEOVERLAY_H
