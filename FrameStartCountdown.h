#ifndef FRAMESTARTCOUNTDOWN_H
#define FRAMESTARTCOUNTDOWN_H

#include <QFrame>
#include <QKeyEvent>



namespace Ui {



class FrameStartCountdown;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class FrameStartCountdown : public QFrame {

	Q_OBJECT

private:
	Ui::FrameStartCountdown *pUi;

private slots:
	void on_button_clicked();

protected:
	virtual void changeEvent(QEvent *pEvent) override;
	virtual void keyPressEvent(QKeyEvent *pEvent) override;


public:
	explicit FrameStartCountdown(QWidget *pParent = nullptr);
	~FrameStartCountdown() override;

signals:
	void debugMessage(const QString &sMessage) const;
	void done() const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("FSC:" + sMessage); }

	virtual void onSetText(const QString sText,
						   const QString sTextButton = QString());

}; // FrameStartCountdown



}	} // namespace SwissalpS::QtNibblers

#endif // FRAMESTARTCOUNTDOWN_H
