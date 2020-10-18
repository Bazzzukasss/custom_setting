#include "custom_setting_manager.h"
#include "custom_setting_loader.h"

using namespace custom_setting;

Manager::Manager(QObject* parent) : QObject(parent)
{}

void Manager::loadConfigurations()
{
    for (auto& filename : mConfigurations.keys())
    {
        auto loader = Loader::create(getSettingsDirPath() + filename, parent());
        if(loader)
        {
            mConfigurations[filename]->load(loader);
        }
    }

    emit signalDataLoaded();
}

void Manager::saveConfigurations()
{
    for (auto& filename : mConfigurations.keys())
    {
        auto loader = Loader::create(getSettingsDirPath() + filename, this);
        if(loader)
        {
            mConfigurations[filename]->save(loader);
        }
    }
}

void Manager::deleteConfiguration(const QString &filename)
{
    mConfigurations.remove(filename);
    remove((getSettingsDirPath() + filename).toStdString().c_str());
}

void Manager::setConfigurations(const Manager::ConfigurationsMap& configurations)
{
    mConfigurations = configurations;
    for (auto& setting : mConfigurations)
    {
        connect(setting, &Setting::signalDataChanged, this, &Manager::signalDataChanged);
    }
}
