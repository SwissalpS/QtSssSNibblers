#ifndef WORM_H
#define WORM_H

#include <QObject>
#include "SurfaceCell.h"



namespace SwissalpS { namespace QtNibblers {



class Worm : public QObject {

	Q_OBJECT

private:

public:

	enum Heading {
		North = 0x90u,
		West = 0x91u,
		South = 0x92u,
		East = 0x93u,
		Up = North,
		Left = West,
		Down = South,
		Right = East
	};
	Q_ENUM(Heading)

	explicit Worm(SurfaceCell *pCell, const quint8 ubColour, const bool bAI,
				  QObject *pParent = nullptr);
	virtual ~Worm();

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Worm:" + sMessage); }

protected:
	bool bAmAI;
	quint8 ubColourIndex;
	SurfaceCell *pCellSpawn;
	QVector<SurfaceCell *> apCells;
	Heading eCurrentHeading;

}; // Worm



}	} // namespace SwissalpS::QtNibblers



#endif // WORM_H
