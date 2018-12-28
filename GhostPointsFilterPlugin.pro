TEMPLATE    = lib
CONFIG      += plugin
#DESTDIR     = $$PWD/../bin/plugins
DESTDIR     = $$PWD/../bin/UrgBenriPlus.app/PlugIns
CONFIG      += c++11
TARGET      = GhostPointsFilterPlugin

!include(GhostPointsFilterPlugin.pri) {
        error("Unable to include Ghost Points Filter Plugin.")
}

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
