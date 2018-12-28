/*
	This file is part of the UrgBenri application.

	Copyright (c) 2016 Mehrez Kristou.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Please contact kristou@hokuyo-aut.jp for more details.

*/

#ifndef GHOSTPOINTSFILTERPLUGIN_H
#define GHOSTPOINTSFILTERPLUGIN_H

#include "FilterPluginInterface.h"

#include <QTranslator>
#include <QMap>

namespace Ui
{
class GhostPointsFilterPlugin;
}

class GhostPointsFilterPlugin : public FilterPluginInterface
{
    Q_OBJECT    
    Q_INTERFACES(FilterPluginInterface)
    Q_PLUGIN_METADATA(IID "org.kristou.UrgBenri.GhostPointsFilterPlugin")
    PLUGIN_FACTORY(GhostPointsFilterPlugin)
public:
    explicit GhostPointsFilterPlugin(QWidget* parent = 0);
    virtual ~GhostPointsFilterPlugin();

    QString pluginName() const{return tr("GhostPoints");}
    QIcon pluginIcon() const {return QIcon(":/GhostPointsFilterPlugin/tabIcon");}
    QString pluginDescription() const {return tr("GhostPoints Steps");}
    PluginVersion pluginVersion() const {return PluginVersion(1, 0, 0);}
    QString pluginAuthorName() const {return "Mehrez Kristou";}
    QString pluginAuthorContact() const {return "mehrez@kristou.com";}
    int pluginLoadOrder() const {return 2;}
    bool pluginIsExperimental() const { return false; }

    QString pluginLicense() const { return "GPL"; }
    QString pluginLicenseDescription() const { return "GPL"; }

    void saveState(QSettings &settings);
    void restoreState(QSettings &settings);

    void loadTranslator(const QString &locale);

    void updateConnectorsList(const QVector<PluginInformation> &infos);

public slots:
    void addMeasurementData(const QString &id
                                    , const PluginDataStructure &data);
    void onLoad(PluginManagerInterface *manager);
    void onUnload();

protected:
    void changeEvent(QEvent* e);

private:
    void setupConnections();

    Ui::GhostPointsFilterPlugin* ui;
    QVector<PluginInformation> m_souces;
    QMap<QString, int> m_sourceDictionaty;
//    QVector<QVector<long > > m_ranges;
//    QVector<QVector<long > > m_levels;
    QTranslator m_translator;
    int m_currentCount;

    void retranslateUI();
    bool isLine(const QVector<QVector<long > > &ranges, int start, int &end, long &maxDiff) const;
    bool isGhostLine(const QVector<QVector<long > > &ranges, int start, int end, long maxDiff) const;


private slots:

};

#endif // GHOSTPOINTSFILTERPLUGIN_H

