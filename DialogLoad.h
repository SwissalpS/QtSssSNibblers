#ifndef DIALOGLOAD_H
#define DIALOGLOAD_H

#include <QDialog>



namespace Ui {



class DialogLoad;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class DialogLoad : public QDialog {
	Q_OBJECT

private:
	Ui::DialogLoad *pUi;

protected:
	void changeEvent(QEvent *pEvent);

public:
	explicit DialogLoad(QWidget *parent = 0);
	~DialogLoad();

	virtual int getSelected() const;
	virtual void setSelected(const int iIndex) const;

}; // DialogLoad



}	} // namespace SwissalpS::QtNibblers




#endif // DIALOGLOAD_H
