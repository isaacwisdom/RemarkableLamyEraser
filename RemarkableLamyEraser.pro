TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /opt/codex/rm11x/2.5.2/sysroots/x86_64-codexsdk-linux/usr/lib/arm-remarkable-linux-gnueabi/gcc/arm-remarkable-linux-gnueabi/7.3.0/include

SOURCES += \
        main.c

service.path = /home/isaacwisdom/reMarkbleLamyEraser/reMarkableLamyEraser/LamyEraser.service
service.file = home/isaacwisdom/reMarkbleLamyEraser/reMarkableLamyEraser/LamyEraser.service
DISTFILES += \
    RemarkableLamyEraser/LamyEraser.service

target.path = /home/root/reMarkableLamyEraser

INSTALLS += \
        target
        service
