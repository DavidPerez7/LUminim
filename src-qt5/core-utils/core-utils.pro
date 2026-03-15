PRECOMPILED_HEADER = ../../lumina_pch.h

include("../OS-detect.pri")

TEMPLATE = subdirs
CONFIG += recursive

SUBDIRS+= lumina-config \
	 lumina-search \
	 lumina-xconfig

