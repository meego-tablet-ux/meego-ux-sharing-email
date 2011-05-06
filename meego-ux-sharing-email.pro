VERSION=0.2.4
PLUGIN_QML_TARGET_BASE_PATH = $$[QT_INSTALL_IMPORTS]/MeeGo/Sharing/plugins/
MSD_PLUGIN_PATH=/usr/lib/meego-ux-sharing/plugins
TEMPLATE = lib
QT += dbus
CONFIG += plugin link_pkgconfig

#For some reason, moc doesn't pick up the cflags provided by pkgconfig.
#So, we add them by hand. I'm probably doing something wrong here - please
#let me know if you know the Right Way to do this - james.ausmus@intel.com

INCLUDEPATH += $$system(pkg-config --cflags libmeegouxsharingserviceplugin \
| tr \' \' \'\\n\' | grep ^-I | cut -d 'I' -f 2-)

PKGCONFIG += libmeegouxsharingserviceplugin

HEADERS  = emailplugin.h \
    emailsharingservice.h

OTHER_FILES += qml/*.qml

SOURCES  = emailplugin.cpp \
    emailsharingservice.cpp

TARGET = emailsharingplugin
target.path = $$MSD_PLUGIN_PATH

qml.files = qml/*.qml
qml.path = $$PLUGIN_QML_TARGET_BASE_PATH/email/

INSTALLS += target qml

#********* Translations *********
TRANSLATIONS += $${SOURCES} $${HEADERS} $${OTHER_FILES}
PROJECT_NAME = meego-ux-sharing-email

dist.commands += rm -Rf $${PROJECT_NAME}-$${VERSION} &&
dist.commands += git clone . $${PROJECT_NAME}-$${VERSION} &&
dist.commands += rm -Rf $${PROJECT_NAME}-$${VERSION}/.git &&
dist.commands += mkdir -p $${PROJECT_NAME}-$${VERSION}/ts &&
dist.commands += lupdate $${TRANSLATIONS} -ts $${PROJECT_NAME}-$${VERSION}/ts/$${PROJECT_NAME}.ts &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION}
QMAKE_EXTRA_TARGETS += dist
#********* End Translations ******
