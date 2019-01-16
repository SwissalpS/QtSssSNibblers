#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AppSettings.h"
#include "History.h"
#include "Lingo.h"



namespace Ui {



class MainWindow;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class MainWindow : public QMainWindow {

	Q_OBJECT

private:
	Ui::MainWindow *pUi;

private slots:

	void on_buttonAP0_clicked();
	void on_buttonAP1_clicked();
	void on_buttonAP2_clicked();
	void on_buttonAP3_clicked();
	void on_buttonAP4_clicked();
	void on_buttonAP5_clicked();
	void on_buttonAP6_clicked();
	void on_buttonAP7_clicked();
	void on_buttonAP8_clicked();

	void on_buttonClearHistory_clicked();
	void on_buttonClearHistorySelected_clicked();

	void on_buttonHP0_clicked();
	void on_buttonHP1_clicked();
	void on_buttonHP2_clicked();
	void on_buttonHP3_clicked();
	void on_buttonHP4_clicked();

	void on_buttonZap0_clicked();
	void on_buttonZap128_clicked();
	void on_buttonZap255_clicked();

	void on_cbFakeBonuses_stateChanged(int iState);
	void on_cbGameOverOnLastDead_stateChanged(int iState);
	void on_cbLimitLives_toggled(bool bChecked);
	void on_cbLoadSetsStartLevel_stateChanged(int iState);

	void on_cbRelative1_toggled(bool bChecked);
	void on_cbRelative2_toggled(bool bChecked);
	void on_cbRelative3_toggled(bool bChecked);
	void on_cbRelative4_toggled(bool bChecked);

	void on_cbSound_stateChanged(int iState);

	void on_cbUseMouse1_toggled(bool bChecked);
	void on_cbUseMouse2_toggled(bool bChecked);
	void on_cbUseMouse3_toggled(bool bChecked);
	void on_cbUseMouse4_toggled(bool bChecked);

	void on_kseDown1_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseDown2_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseDown3_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseDown4_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseLeft1_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseLeft2_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseLeft3_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseLeft4_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseRight1_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseRight2_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseRight3_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseRight4_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseUp1_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseUp2_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseUp3_keySequenceChanged(const QKeySequence &oKeySequence);
	void on_kseUp4_keySequenceChanged(const QKeySequence &oKeySequence);

	void on_leName1_editingFinished();
	void on_leName2_editingFinished();
	void on_leName3_editingFinished();
	void on_leName4_editingFinished();

	void on_radioBLloop_toggled(bool bChecked);
	void on_radioBLskip_toggled(bool bChecked);
	void on_radioBLwin_toggled(bool bChecked);
	void on_radioNetClient_toggled(bool bChecked);
	void on_radioNetOff_toggled(bool bChecked);
	void on_radioNetServer_toggled(bool bChecked);

	void on_selectColour1_currentIndexChanged(int iIndex);
	void on_selectColour2_currentIndexChanged(int iIndex);
	void on_selectColour3_currentIndexChanged(int iIndex);
	void on_selectColour4_currentIndexChanged(int iIndex);
	void on_selectColour5_currentIndexChanged(int iIndex);
	void on_selectColour6_currentIndexChanged(int iIndex);
	void on_selectColour7_currentIndexChanged(int iIndex);
	void on_selectColour8_currentIndexChanged(int iIndex);

	void on_selectSpeed_currentIndexChanged(int iIndex);
	void on_selectStartLevel_currentIndexChanged(int iIndex);
	void on_selectStartLives_currentIndexChanged(int iIndex);
	void on_tabWidgetMain_currentChanged(int iIndex);

protected:
	AppSettings *pAS;
	History *pHistory;

	void changeEvent(QEvent *pEvent);
	void closeEvent(QCloseEvent *pEvent);
	virtual void initBuilder();
	virtual void initGame();
	virtual void initHistory();
	virtual void initSettings();
	virtual void onPlayerColourChanged(const quint8 ubWorm, const quint8 ubIndex);
	virtual void onPlayerKeyDownChanged(const quint8 ubWorm, const QKeySequence &oKeySequence);
	virtual void onPlayerKeyLeftChanged(const quint8 ubWorm, const QKeySequence &oKeySequence);
	virtual void onPlayerKeyRightChanged(const quint8 ubWorm, const QKeySequence &oKeySequence);
	virtual void onPlayerKeyUpChanged(const quint8 ubWorm, const QKeySequence &oKeySequence);
	virtual void onPlayerNameChanged(const quint8 ubWorm, const QString sName);
	virtual void onPlayerRelativeToggled(const quint8 ubWorm, const bool bChecked);
	virtual void onPlayerUseMouseToggled(const quint8 ubWorm, const bool bChecked);
	virtual void settingsUpdatePlayerColours();
	virtual void settingsUpdatePlayerCount();
	virtual void settingsUpdatePlayerKeys();
	virtual void settingsUpdatePlayerNames();
	virtual void settingsUpdatePlayerMouseAndRelative();

public:
	explicit MainWindow(QWidget *pParent = nullptr);
	~MainWindow();

signals:
	void debugMessage(const QString &sMessage) const;
	void mainTabIndexChanged(const int iIndex) const;
	void quitting() const;
	void settingsPlayerColoursChanged(const QVector<quint8> aubColours) const;
	void settingsPlayerCountChanged(const quint8 ubCountHumans,
									const quint8 ubCountAIs) const;
	void settingsPlayerKeyChanged(const quint8 ubWorm,
						  const QKeySequence &oKeySequence,
						  const L::Heading eHeading) const;
	void settingsPlayerNameChanged(const quint8 ubWorm, const QString sName) const;
	void settingsSpeedChanged(const int iIndex) const;
	void settingsRelativeChanged(const quint8 ubWorm, const bool bRelative) const;


public slots:
	void onDebugMessage(const QString &sMessage) const;
	void onStatusMessage(const QString &sMessage) const;
	virtual void onUpdateHistory();

	void run();
}; // MainWindow



}	} // namespace SwissalpS::QtNibblers



#endif // MAINWINDOW_H
