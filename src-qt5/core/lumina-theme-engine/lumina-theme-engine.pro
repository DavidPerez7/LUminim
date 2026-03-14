include(../../OS-detect.pri)

TEMPLATE = subdirs
# Minimal footprint: keep only essential components
# Removed: lthemeengine (GUI editor - not essential), lthemeengine-sstest (screensaver test)
SUBDIRS += src/lthemeengine-qtplugin \
		src/lthemeengine-style
#		src/lthemeengine \
#		src/lthemeengine-sstest

colors.files = colors/*.conf
colors.path = $${L_SHAREDIR}/lthemeengine/colors

qss.files = qss/*.qss
qss.path = $${L_SHAREDIR}/lthemeengine/qss

dqss.files = desktop_qss/*.qss
dqss.path = $${L_SHAREDIR}/lthemeengine/desktop_qss

INSTALLS += colors qss dqss
