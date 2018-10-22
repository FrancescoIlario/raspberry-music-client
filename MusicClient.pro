#-------------------------------------------------
#
# Project created by QtCreator 2018-10-16T22:26:04
#
#-------------------------------------------------

QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicClient
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

CONFIG += c++17

LIBS += -lpistache -lpthread -ljson11

SOURCES += \
        main.cpp \
        MainWindow.cpp \
        rest/ArtistsApi.cpp \
        classes/Artist.cpp \
        classes/ArtistInfo.cpp \
        classes/Album.cpp \
        classes/AlbumInfo.cpp \
        classes/Song.cpp \
        models/ArtistViewModel.cpp \
        models/AlbumViewModel.cpp \
        models/SongViewModel.cpp

HEADERS += \
        MainWindow.hpp \
        rest/ArtistsApi.hpp \
        classes/Artist.hpp \
        classes/ArtistInfo.hpp \
        classes/Album.hpp \
        classes/AlbumInfo.hpp \
        classes/Song.hpp \
        rest/MusicServer.hpp \
        models/ArtistViewModel.hpp \
        models/AlbumViewModel.hpp \
        models/SongViewModel.hpp

FORMS += \
        MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
