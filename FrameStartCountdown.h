#ifndef FRAMESTARTCOUNTDOWN_H
#define FRAMESTARTCOUNTDOWN_H

#include <QFrame>



namespace Ui {



class FrameStartCountdown;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class FrameStartCountdown : public QFrame {
	Q_OBJECT

private:
	Ui::FrameStartCountdown *pUi;

protected:
	void changeEvent(QEvent *pEvent);

public:
	explicit FrameStartCountdown(QWidget *pParent = 0);
	~FrameStartCountdown();

public slots:
	virtual void onSetText(const QString sText);

}; // FrameStartCountdown



}	} // namespace SwissalpS::QtNibblers

#endif // FRAMESTARTCOUNTDOWN_H
