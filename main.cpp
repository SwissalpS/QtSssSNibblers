#include "MainWindow.h"
#include <QApplication>
#include <QTimer>

int main(int iArgCount, char *aArguments[]) {

	QApplication oApp(iArgCount, aArguments);

	SwissalpS::QtNibblers::MainWindow oMainWindow;
	oMainWindow.show();

	QTimer::singleShot(1, &oMainWindow, SLOT(run()));

	return oApp.exec();

} // main
