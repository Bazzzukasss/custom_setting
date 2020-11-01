#include "custom_setting.h"
#include "custom_setting_serializer.h"

using namespace custom_setting;

Setting::Setting(const QString& key, const QString& caption, const QString& description, bool read_only, QObject* parent) :
    QObject(parent),
    mKey(key),
    mCaption(caption),
    mDescription(description),
    mReadOnly(read_only)
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

void Setting::load(Serializer* serializer, const QString& parentKey)
{
    const auto& key = parentKey + "/" + mKey;
    auto value      = serializer->getValue(key, getDefaultValue(), !mSettings.isEmpty());
    if (value.isValid())
    {
        setValue(value);
    }

    for (auto& customSetting : mSettings)
    {
        customSetting->load(serializer, key);
    }
}

void Setting::save(Serializer* serializer, const QString& parentKey)
{
    const auto& key   = parentKey + "/" + mKey;
    const auto& value = getValue();
    if (value.isValid())
    {
        serializer->setValue(key, value, !mSettings.isEmpty());
    }

    for (auto& customSetting : mSettings)
    {
        customSetting->save(serializer, key);
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
        auto boolSetting = dynamic_cast<SettingBool*>(setting);
        if (boolSetting)
        {
            res |= *boolSetting;
        }
    }
    return res;
}
