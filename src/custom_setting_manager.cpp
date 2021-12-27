#include "custom_setting_manager.h"
#include "custom_setting_serializer.h"
#include "custom_setting.h"

using namespace custom_setting;

Manager::Manager(QObject* parent) : QObject(parent)
{}

void Manager::loadConfigurations()
{
    for (auto& filename : mConfigurations.keys())
    {
        auto serializer =
            Serializer::create(getSettingsDirPath() + filename,
                               Serializer::Mode::kRead,
                               parent());
        if (serializer)
        {
            mConfigurations[filename]->load(serializer);
        }
    }

    emit signalDataLoaded();
}

void Manager::saveConfigurations()
{
    for (auto& filename : mConfigurations.keys())
    {
        auto serializer =
            Serializer::create(getSettingsDirPath() + filename,
                               Serializer::Mode::kWrite,
                               this);
        if (serializer)
        {
            mConfigurations[filename]->save(serializer);
            serializer->sync();
        }
    }
}

void Manager::deleteConfiguration(const QString& filename)
{
    mConfigurations.remove(filename);
    remove((getSettingsDirPath() + filename).toStdString().c_str());
}

QString Manager::getSettingsDirPath() const
{
    return {qApp->applicationDirPath() + QDir::separator()};
}

void Manager::setConfigurations(const Manager::ConfigurationsMap& configurations)
{
    mConfigurations = configurations;
    for (auto& setting : mConfigurations)
    {
        connect(setting, &Setting::signalDataChanged,
                this, &Manager::signalDataChanged);
    }
}
