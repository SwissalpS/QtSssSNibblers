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
