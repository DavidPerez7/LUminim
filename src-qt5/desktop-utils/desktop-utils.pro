PRECOMPILED_HEADER = ../../lumina_pch.h
include("../OS-detect.pri")

TEMPLATE = subdirs
CONFIG += recursive

SUBDIRS += lumina-fm \
           lumina-screenshot \
           lumina-textedit \
           lumina-photo \
           lumina-sudo
