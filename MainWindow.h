#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AppSettings.h"



namespace Ui {



class MainWindow;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class MainWindow : public QMainWindow {
	Q_OBJECT

private:
	Ui::MainWindow *pUi;

protected:
	AppSettings *pAS;

	void changeEvent(QEvent *pEvent);
	void closeEvent(QCloseEvent *pEvent);
	virtual void initBuilder();

public:
	explicit MainWindow(QWidget *pParent = nullptr);
	~MainWindow();

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	void onDebugMessage(const QString &sMessage) const;
	void onStatusMessage(const QString &sMessage) const;

	void run();

}; // MainWindow



}	} // namespace SwissalpS::QtNibblers



#endif // MAINWINDOW_H
