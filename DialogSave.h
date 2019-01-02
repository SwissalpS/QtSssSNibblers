#ifndef DIALOGSAVE_H
#define DIALOGSAVE_H

#include <QDialog>



namespace Ui {



class DialogSave;



} // namespace Ui



namespace SwissalpS { namespace QtNibblers {



class DialogSave : public QDialog {
	Q_OBJECT

private:
	Ui::DialogSave *pUi;

private slots:
	void on_comboBox_currentIndexChanged(int iIndex);

protected:
	void changeEvent(QEvent *pEvent);

public:
	explicit DialogSave(QWidget *pParent = nullptr);
	~DialogSave();

	virtual int getSelected() const;
	virtual void setSelected(const int iIndex) const;

}; // DialogSave



}	} // namespace SwissalpS::QtNibblers



#endif // DIALOGSAVE_H
