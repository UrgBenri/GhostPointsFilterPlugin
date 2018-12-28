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

#include "GhostPointsFilterPlugin.h"
#include "ui_GhostPointsFilterPlugin.h"

#include <QDebug>
#include <QtMath>

using namespace std;

const QString LogHeader = "GhostPointsFilterPlugin";

GhostPointsFilterPlugin::GhostPointsFilterPlugin(QWidget* parent)
    : FilterPluginInterface(parent)
    , ui(new Ui::GhostPointsFilterPlugin)
    , m_currentCount(0)
{
    ui->setupUi(this);
}

void GhostPointsFilterPlugin::retranslateUI()
{
    if(ui->sources->count() == m_souces.size()){
        for(int i = 0; i < ui->sources->count(); ++i){
            ui->sources->setItemText(i, m_souces[i].title());
        }
    }
}

void GhostPointsFilterPlugin::setupConnections()
{

}

GhostPointsFilterPlugin::~GhostPointsFilterPlugin()
{

    delete ui;
}

void GhostPointsFilterPlugin::saveState(QSettings &settings)
{
    settings.setValue("currentIndex", ui->sources->currentIndex());
}

void GhostPointsFilterPlugin::restoreState(QSettings &settings)
{
    ui->sources->setCurrentIndex(settings.value("currentIndex", 0).toInt());
}

void GhostPointsFilterPlugin::loadTranslator(const QString &locale)
{
    qApp->removeTranslator(&m_translator);
    if (m_translator.load(QString("plugin.%1").arg(locale), ":/GhostPointsFilterPlugin")) {
        qApp->installTranslator(&m_translator);
    }
}

void GhostPointsFilterPlugin::updateConnectorsList(const QVector<PluginInformation> &infos)
{
    m_souces = infos;
    QString index = ui->sources->itemData(ui->sources->currentIndex(), Qt::UserRole).toString();
    ui->sources->blockSignals(true);
    ui->sources->clear();
    foreach (const PluginInformation &item, m_souces) {
        ui->sources->addItem(item.icon(), item.title(), item.id());
        m_sourceDictionaty[item.id()] = ui->sources->count() -1;
    }
    ui->sources->blockSignals(false);

    int find = -1;
    for(int i = 0; i < ui->sources->count(); ++i){
        if(ui->sources->itemData(i, Qt::UserRole).toString() == index){
            find = i;
            break;
        }
    }
    if(find < 0){
        ui->sources->setCurrentIndex(0);
    }else{
        ui->sources->setCurrentIndex(find);
    }
}

bool GhostPointsFilterPlugin::isGhostLine(const QVector<QVector<long> > &ranges, int start, int end, long maxDiff) const
{
    long diff = std::abs(ranges[end -1][0] - ranges[start][0]);
    bool result = maxDiff > ui->minGap->value() && diff > ui->minJump->value();
    return result;
}

bool GhostPointsFilterPlugin::isLine(const QVector<QVector<long> > &ranges, int start, int &end, long &maxDiff) const
{
    long last = ranges[start][0];
    bool positive = false;
    bool first = true;
    bool result = false;
    maxDiff = 0;
    for(int i = start+1; i < ranges.size(); ++i){
        long diff = ranges[i][0] - last;
        last = ranges[i][0];
        bool diffSign = diff > 0;
        if(first){
            positive = diffSign;
            first = false;
        }

        diff = std::abs(diff);
        if(diff > maxDiff) maxDiff = diff;

        if(diffSign != positive || diff < ui->continuity->value()){
            maxDiff -= diff;
            int groupSize = i - start;
            if(groupSize > 3){
                end = i;
                result = true;
            }
            break;
        }
    }
    return result;
}

void GhostPointsFilterPlugin::addMeasurementData(const QString &id, const PluginDataStructure &data)
{
    if(m_sourceDictionaty[id] == ui->sources->currentIndex()){
        PluginDataStructure filteredData = data;

        QVector<QVector<long > > ranges = filteredData.ranges;
        int cpt = 0;
        while(cpt < ranges.size()){
            int end = 0;
            long max;
            bool l = isLine(ranges, cpt, end, max);
            if(l){
                bool ghost = isGhostLine(ranges, cpt, end, max);
                if(ghost){
                    for(int i = cpt; i < end; ++i){
                        ranges[i][0] = 0;
                    }
                }
                cpt = end;
            }
            else{
                cpt++;
            }
        }
        filteredData.ranges = ranges;
        emit measurementDataReady(filteredData);
    }
}

void GhostPointsFilterPlugin::onLoad(PluginManagerInterface *manager)
{

}

void GhostPointsFilterPlugin::onUnload()
{

}

void GhostPointsFilterPlugin::changeEvent(QEvent* e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            if(ui) ui->retranslateUi(this);
            retranslateUI();
        break;
        default:
        break;
    }
}

