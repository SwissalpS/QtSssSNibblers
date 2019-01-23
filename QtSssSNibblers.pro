#-------------------------------------------------
#
# Project created by QtCreator 2018-12-30T18:10:13
#
#-------------------------------------------------

QT		+= core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

qtHaveModule(multimedia): QT += multimedia

TARGET = QtSssSNibblers
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
	AppSettings.cpp \
	Bonus.cpp \
	DialogLoad.cpp \
	DialogSave.cpp \
	FrameStartCountdown.cpp \
	Fx.cpp \
	Game.cpp \
	IconEngine.cpp \
	History.cpp \
	HistoryItem.cpp \
	main.cpp \
	MainWindow.cpp \
	Map.cpp \
	MapGame.cpp \
	PersistantObject.cpp \
	ScoreBoard.cpp \
	SurfaceBuilder.cpp \
	SurfaceCell.cpp \
	SurfaceFrame.cpp \
	SurfaceGame.cpp \
	Worm.cpp \
	WormAI.cpp

HEADERS += \
	AppSettings.h \
	Bonus.h \
	definitions.h \
	DialogLoad.h \
	DialogSave.h \
	FrameStartCountdown.h \
	Fx.h \
	Game.h \
	History.h \
	HistoryItem.h \
	IconEngine.h \
	Lingo.h \
	MainWindow.h \
	Map.h \
	MapGame.h \
	PersistantObject.h \
	ScoreBoard.h \
	SurfaceBuilder.h \
	SurfaceCell.h \
	SurfaceFrame.h \
	SurfaceGame.h \
	Worm.h \
	WormAI.h

FORMS += \
	DialogLoad.ui \
	DialogSave.ui \
	FrameStartCountdown.ui \
	MainWindow.ui \
	ScoreBoard.ui \
	SurfaceBuilder.ui \
	SurfaceCell.ui \
	SurfaceGame.ui

RESOURCES += QtSssSNibblers.qrc
