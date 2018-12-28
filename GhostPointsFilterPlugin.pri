!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    QT += core gui widgets

    !include($$PWD/PluginInterface/UrgBenriPluginInterface.pri) {
            error("Unable to include Viewer Plugin Interface.")
    }

    DEPENDPATH += $$PWD
    INCLUDEPATH += $$PWD

    SOURCES += \
            $$PWD/GhostPointsFilterPlugin.cpp

    HEADERS  += \
            $$PWD/GhostPointsFilterPlugin.h

    FORMS    += \
            $$PWD/GhostPointsFilterPlugin.ui

    RESOURCES += \
            $$PWD/GhostPointsFilterPlugin.qrc

    TRANSLATIONS = $$PWD/i18n/plugin_fr.ts \
            $$PWD/i18n/plugin_en.ts \
            $$PWD/i18n/plugin_ja.ts
}
