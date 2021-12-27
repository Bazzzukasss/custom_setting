#include "custom_setting.h"
#include "custom_setting_serializer.h"

using namespace custom_setting;

Setting::Setting(const QString& key,
                 const QString& caption,
                 const QString& description,
                 bool readOnly,
                 QObject* parent) :
    QObject(parent),
    mKey(key),
    mCaption(caption),
    mDescription(description),
    mReadOnly(readOnly)
{}

void Setting::addSettings(const Vector& settings)
{
    mSettings.append(settings);

    for (auto& setting : settings)
    {
        setting->setParent(this);

        connect(setting, &Setting::signalDataChanged,
                this, &Setting::signalDataChanged);
    }
}

void Setting::load(Serializer* serializer, const QString& parentKey)
{
    const auto& key = parentKey + "/" + mKey;
    auto value = serializer->getValue(key, getDefaultValue(), !mSettings.isEmpty());

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
    const auto& key = parentKey + "/" + mKey;
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

bool Setting::getIsHandlerBlocked() const
{
    return mIsHandlerBlocked;
}

void Setting::bindTo(std::function<void (const QVariant&)> handler)
{
    connect(this, &Setting::signalDataChanged, [=](const QVariant& value) {
                if (!getIsHandlerBlocked())
                {
                    handler(value);
                }
            });
}

void Setting::bindTo(std::function<void ()> handler)
{
    connect(this, &Setting::signalDataChanged, [=]() {
                if (!getIsHandlerBlocked())
                {
                    handler();
                }
            });
}

void Setting::blockHandler(bool isBlocked)
{
    mIsHandlerBlocked = isBlocked;
}

const QString& Setting::getCaption() const
{
    return mCaption;
}

const QString& Setting::getDescription() const
{
    return mDescription;
}

const Setting::Vector& Setting::getSettings() const
{
    return mSettings;
}

bool Setting::isReadOnly() const
{
    return mReadOnly;
}

bool Setting::isAnyChecked() const
{
    for (auto& setting : getSettings())
    {
        if (auto boolSetting = dynamic_cast<SettingBool*>(setting))
        {
            if (*boolSetting)
            {
                return true;
            }
        }
    }

    return false;
}

QVariant Setting::getValue() const
{
    return {};
}

QVariant Setting::getDefaultValue() const
{
    return {};
}

void Setting::setCaption(const QString& caption)
{
    mCaption = caption;
}

void Setting::setDescription(const QString& description)
{
    mDescription = description;
}

void Setting::setValue(const QVariant&)
{
}

void Setting::notifyValue(const QVariant&)
{
}

void Setting::emitSignalDataChanged(const QVariant& value)
{
    emit signalDataChanged(value);
}
