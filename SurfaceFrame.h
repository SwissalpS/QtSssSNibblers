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
#ifndef SURFACEFRAME_H
#define SURFACEFRAME_H

#include <QFrame>



class SurfaceFrame : public QFrame {

	Q_OBJECT

private:

protected:
	mutable int iLastHeight;

	void resizeEvent(QResizeEvent *pEvent);

public:
	explicit SurfaceFrame(QWidget *pParent = nullptr);
	virtual ~SurfaceFrame();

	virtual QSize sizeHint() const;

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SurfaceFrame:" + sMessage); }

}; // SurfaceFrame



#endif // SURFACEFRAME_H
