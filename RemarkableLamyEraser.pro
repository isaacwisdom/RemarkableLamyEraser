TEMPLATE = app
CONFIG += console
CONFIG += file_copies
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /opt/codex/rm11x/2.5.2/sysroots/x86_64-codexsdk-linux/usr/lib/arm-remarkable-linux-gnueabi/gcc/arm-remarkable-linux-gnueabi/7.3.0/include

SOURCES += \
        main.c \

OTHER_FILES += \
    RemarkableLamyEraser/LamyEraser.service \
    README.md

service.files = RemarkableLamyEraser/LamyEraser.service


target.path = /home/root/RemarkableLamyEraser
service.path = /home/root/RemarkableLamyEraser

INSTALLS += \
        target \
        service

HEADERS += \
    screenlocations.h
