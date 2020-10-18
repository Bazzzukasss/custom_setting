#include "custom_setting.h"

using namespace custom_setting;

Setting::Setting(const QString& key, const QString& caption, const QString& description, QObject* parent) :
    QObject(parent),
    mKey(key),
    mCaption(caption),
    mDescription(description)
{}

void Setting::addSettings(const Vec &settings)
{
    mSettings.append(settings);
    for (auto& setting : settings)
    {
        setting->setParent(this);
        connect(setting, &Setting::signalDataChanged, this, &Setting::signalDataChanged);
    }
}

void Setting::load(Loader* loader, const QString& parentKey)
{
    const auto& key = parentKey + "/" + mKey;
    auto value      = loader->getValue(key, getDefaultValue());
    if (value.isValid())
    {
        setValue(value);
    }

    for (auto& customSetting : mSettings)
    {
        customSetting->load(loader, key);
    }
}

void Setting::save(Loader* loader, const QString& parentKey)
{
    const auto& key   = parentKey + "/" + mKey;
    const auto& value = getValue();
    if (value.isValid())
    {
        loader->setValue(key, value);
    }

    for (auto& customSetting : mSettings)
    {
        customSetting->save(loader, key);
    }
}

void Setting::bindTo(std::function<void (const QVariant &)> procedure)
{
    connect(this, &Setting::signalDataChanged, [=](const QVariant& value) { procedure(value); });
}

void Setting::bindTo(std::function<void ()> procedure)
{
    connect(this, &Setting::signalDataChanged, [=]() { procedure(); });
}

bool Setting::isAnyChecked() const
{
    bool res{false};
    for (auto setting : getSettings())
    {
        auto boolSetting = dynamic_cast<SettingExt<DataBool>*>(setting);
        if (boolSetting)
        {
            res |= *boolSetting;
        }
    }
    return res;
}
