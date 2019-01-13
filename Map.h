#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QVector>
#include "Lingo.h"



namespace SwissalpS { namespace QtNibblers {



class Map : public QObject {

	Q_OBJECT

private:

protected:
	quint8 ubTotalColumns;
	quint8 ubTotalRows;
	QVector<QVector<quint8>> aaubRows;

public:
	explicit Map(QObject *pParent = nullptr);
	explicit Map(const quint8 ubColumns, const quint8 ubRows, QObject *pParent = nullptr);
	virtual ~Map();

	inline bool isNull() { return (0 == this->ubTotalColumns) || (0 == this->ubTotalRows); }
	virtual void fillAll(const quint8 ubState = L::FloorClean);
	virtual QPixmap pixmap(const bool bSimple = true) const;
	virtual void setTile(const quint8 ubColumn, const quint8 ubRow, quint8 ubState);
	inline virtual void setTile(const QPoint oPoint, quint8 ubState) {
		this->setTile(quint8(oPoint.x()), quint8(oPoint.y()), ubState); }

	virtual quint8 tile(const quint8 ubColumn, const quint8 ubRow) const;
	inline virtual quint8 tile(const QPoint oPoint) const {
		return this->tile(quint8(oPoint.x()), quint8(oPoint.y())); }

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Map:" + sMessage); }

}; // Map



}	} // namespace SwissalpS::QtNibblers



#endif // MAP_H
