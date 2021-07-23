TEMPLATE = app
CONFIG += console
CONFIG += file_copies
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /opt/codex/rm11x/2.5.2/sysroots/x86_64-codexsdk-linux/usr/lib/arm-remarkable-linux-gnueabi/gcc/arm-remarkable-linux-gnueabi/7.3.0/include

HEADERS += \
    configuration.h \
    effects.h \
    effects_data.h \
    orientation.h \
    triggers.h \

SOURCES += \
    configuration.c \
    effects.c \
    orientation.c \
    triggers.c \
    main.c \


OTHER_FILES += \
    RemarkableLamyEraser.conf\
    RemarkableLamyEraser/LamyEraser.service \
    README.md \

conf.files = RemarkableLamyEraser/LamyEraser.conf
conf.path = /home/root/.config/LamyEraser

service.files = RemarkableLamyEraser/LamyEraser.service
service.path = /home/root/RemarkableLamyEraser

target.path = /home/root/RemarkableLamyEraser


INSTALLS += \
        target \
        service \
        conf \

DISTFILES += \
    RemarkableLamyEraser/LamyEraser.conf
