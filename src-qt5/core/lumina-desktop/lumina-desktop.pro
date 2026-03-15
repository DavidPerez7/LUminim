PRECOMPILED_HEADER = ../../lumina_pch.h
include($${PWD}/../../OS-detect.pri)

lessThan(QT_MAJOR_VERSION, 5) {
  message("[ERROR] Qt 5.11+ is required to use the Lumina Desktop!")
  exit
}
lessThan(QT_MINOR_VERSION, 11){
  message("[ERROR] Qt 5.11+ is required to use the Lumina Desktop!")
  exit
}
QT       += core gui network widgets x11extras svg

# Compilation optimization flags - Maximized for speed (Aggressive Level 2)
CONFIG += release
QMAKE_CXXFLAGS_RELEASE = -O3 -march=native -Ofast -flto=16 -fvisibility=hidden -s
QMAKE_STRIP = strip


TARGET = lumina-desktop
target.path = $${L_BINDIR}

#include all the special classes from the Lumina tree
include(../libLumina/ResizeMenu.pri)
include(../libLumina/LDesktopUtils.pri) #includes LUtils and LOS
include(../libLumina/LuminaXDG.pri)
include(../libLumina/LuminaX11.pri)
include(../libLumina/LuminaSingleApplication.pri)
include(../libLumina/LuminaThemes.pri)
include(../libLumina/ExternalProcess.pri)
include(../libLumina/LIconCache.pri)
include(../libLumina/DesktopSettings.pri)

TEMPLATE = app

SOURCES += main.cpp \
	WMProcess.cpp \
	LXcbEventFilter.cpp \
	LSession.cpp \
	LDesktop.cpp \
	LDesktopBackground.cpp \
	LDesktopPluginSpace.cpp \
	LPanel.cpp \
	LWinInfo.cpp \
	AppMenu.cpp \
	SettingsMenu.cpp \
	SystemWindow.cpp \
	BootSplash.cpp \
	desktop-plugins/LDPlugin.cpp


HEADERS  += Globals.h \
	global-includes.h \
	WMProcess.h \
	LXcbEventFilter.h \
	LSession.h \
	LDesktop.h \
	LDesktopBackground.h \
	LDesktopPluginSpace.h \
	LPanel.h \
	LWinInfo.h \
	AppMenu.h \
	SettingsMenu.h \
	SystemWindow.h \
	BootSplash.h \
	panel-plugins/LPPlugin.h \
	panel-plugins/NewPP.h \
	panel-plugins/LTBWidget.h \
	desktop-plugins/LDPlugin.h \
	desktop-plugins/NewDP.h \
	JsonMenu.h

FORMS    += SystemWindow.ui \
	BootSplash.ui 


#include the individual desktop components
include(../../src-cpp/plugins-base.pri)
#include(src-screensaver/screensaver.pri)  # Disabled - screensaver removed for minimal footprint

#Now include all the files for the various plugins
include(panel-plugins/panel-plugins.pri)
include(desktop-plugins/desktop-plugins.pri)

RESOURCES+= Lumina-DE.qrc

desktop.path = $${L_SESSDIR}
desktop.files = Lumina-DE.desktop

icons.files = Lumina-DE.svg
# LUminim: Removed Fluxbox config/theme installation
defaults.files = defaults/luminaDesktop.conf \
		defaults/compton.conf \
		defaults/openbox-rc.xml
defaults.path = $${L_SHAREDIR}/lumina-desktop/

conf.path = $${L_ETCDIR}

extrafiles.path = $${L_SHAREDIR}/lumina-desktop
extrafiles.files = extrafiles/*

#Now do any OS-specific defaults (if available)
#First see if there is a known OS override first
!isEmpty(DEFAULT_SETTINGS){ 
  message("Installing defaults settings for OS: $${DEFAULT_SETTINGS}")
  OS=$${DEFAULT_SETTINGS}
}
exists("$$PWD/defaults/luminaDesktop-$${OS}.conf"){
  message(" -- Found OS-specific system config file: $${OS}");
  conf.extra = cp $$PWD/defaults/luminaDesktop-$${OS}.conf $(INSTALL_ROOT)$${L_ETCDIR}/luminaDesktop.conf.dist
}else{
  conf.extra = cp $$PWD/defaults/luminaDesktop.conf $(INSTALL_ROOT)$${L_ETCDIR}/luminaDesktop.conf.dist
}
exists("$$PWD/defaults/desktop-background-$${OS}.jpg"){
  message(" -- Found OS-specific background image: $${OS}");
  defaults.extra = cp $$PWD/defaults/desktop-background-$${OS}.jpg $(INSTALL_ROOT)$${L_SHAREDIR}/lumina-desktop/desktop-background.jpg
}else{
  defaults.extra = cp $$PWD/defaults/desktop-background.jpg $(INSTALL_ROOT)$${L_SHAREDIR}/lumina-desktop/desktop-background.jpg
}

# TRANSLATIONS DISABLED - Minimalista Mode
# No i18n support for minimal footprint

manpage.path=$${L_MANDIR}/man1/
manpage.extra="$${MAN_ZIP} $$PWD/lumina-desktop.1 > $(INSTALL_ROOT)$${L_MANDIR}/man1/lumina-desktop.1.gz"

INSTALLS += target desktop icons defaults conf fluxconf fluxtheme manpage extrafiles

# i18n disabled for minimal footprint
