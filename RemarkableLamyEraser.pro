TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /opt/codex/rm11x/2.5.2/sysroots/x86_64-codexsdk-linux/usr/lib/arm-remarkable-linux-gnueabi/gcc/arm-remarkable-linux-gnueabi/7.3.0/include

SOURCES += \
        main.c

target.path = /home/root
INSTALLS += target

DISTFILES += \
    RemarkableLamyEraser/LamyEraser.service
