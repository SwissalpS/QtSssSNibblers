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

private slots:

	void on_buttonHP0_clicked();
	void on_buttonHP1_clicked();
	void on_buttonHP2_clicked();
	void on_buttonHP3_clicked();
	void on_buttonHP4_clicked();
	void on_buttonAP0_clicked();
	void on_buttonAP1_clicked();
	void on_buttonAP2_clicked();
	void on_buttonAP3_clicked();
	void on_buttonAP4_clicked();
	void on_buttonAP5_clicked();
	void on_buttonAP6_clicked();
	void on_buttonAP7_clicked();
	void on_buttonAP8_clicked();
	void on_cbFakeBonuses_stateChanged(int iState);
	void on_cbSound_stateChanged(int iState);
	void on_selectSpeed_currentIndexChanged(int iIndex);
	void on_selectStartLevel_currentIndexChanged(int iIndex);

protected:
	AppSettings *pAS;

	void changeEvent(QEvent *pEvent);
	void closeEvent(QCloseEvent *pEvent);
	virtual void initBuilder();
	virtual void initSettings();
	virtual void settingsUpdatePlayerCount();

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
