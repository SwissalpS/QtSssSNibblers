#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QFrame>
#include <QLabel>


namespace Ui {



class ScoreBoard;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class ScoreBoard : public QFrame {
	Q_OBJECT

private:
	Ui::ScoreBoard *pUi;

protected:
	QVector<QLabel *> apLabels;

	void changeEvent(QEvent *eEvent);

public:
	explicit ScoreBoard(const quint8 ubColour, QWidget *pParent = 0);
	~ScoreBoard();

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("SB:" + sMessage); }

	virtual void setName(const QString sName);
	virtual void setScore(const quint32 ulScore);
	virtual void setLives(const quint8 ubLives);

}; // ScoreBoard



}	} // namespace SwissalpS::QtNibblers



#endif // SCOREBOARD_H
