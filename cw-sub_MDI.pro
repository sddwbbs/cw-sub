QT       += core gui
QT       += charts
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SubcontractsClass.cpp \
    SubcontractsModel.cpp \
    about.cpp \
    empdiagram.cpp \
    main.cpp \
    mainwindow.cpp \
    mdichild.cpp \
    mdichildTable.cpp

HEADERS += \
    SubcontractsClass.h \
    SubcontractsModel.h \
    about.h \
    empdiagram.h \
    mainwindow.h \
    mdichild.h \
    mdichildTable.h

FORMS += \
    about.ui \
    empdiagram.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    app.qrc

TRANSLATIONS += translations/cw-sub_mdi_ru_RU.ts \
                translations/cw-sub_mdi_en_US.ts \
                translations/cw-sub_mdi_de_DE.ts

