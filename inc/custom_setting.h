#pragma once

#include <QSettings>
#include <functional>
#include "custom_setting_data.h"
#include "custom_setting_manager.h"

namespace custom_setting
{

class Serializer;

class Setting : public QObject
{
    Q_OBJECT

 public:
    using Vector = QVector<Setting*>;

    Setting(const QString& key,
            const QString& caption,
            const QString& description,
            bool readOnly = false,
            QObject* parent = nullptr);

    virtual ~Setting() = default;

    void addSettings(const Vector& settings);
    void bindTo(std::function<void(const QVariant&)> handler);
    void bindTo(std::function<void(void)> handler);
    void blockHandler(bool isBlocked);

    const QString& getCaption() const;
    void setCaption(const QString& caption);

    const QString& getDescription() const;
    void setDescription(const QString& description);

    virtual QVariant getValue() const;
    virtual void setValue(const QVariant&);
    virtual void notifyValue(const QVariant&);

    virtual QVariant getDefaultValue() const;

    const Vector& getSettings() const;
    bool isReadOnly() const;
    virtual bool isAnyChecked() const;

signals:
    void signalDataChanged(const QVariant&);

protected:
    void emitSignalDataChanged(const QVariant& value);

protected:
    Vector mSettings;
    QString mKey;
    QString mCaption;
    QString mDescription;
    bool mReadOnly;
    bool mIsHandlerBlocked{false};

private:
    virtual void load(Serializer* serializer, const QString& parentKey = {});
    virtual void save(Serializer* serializer, const QString& parentKey = {});
    bool getIsHandlerBlocked() const;

friend Manager;
};

template <typename T>
class SettingExt : public Setting
{
    using DataValueType = typename T::ValueType;

public:
    SettingExt(const QString& key,
               const QString& caption,
               const QString& description,
               const T& data = {},
               bool readOnly = false,
               QObject* parent = nullptr) :
        Setting(key, caption, description, readOnly, parent),
        mData(data)
    {}

    SettingExt(const SettingExt<T>& setting, QObject* parent = nullptr) :
        Setting(setting.mKey,
                setting.mCaption,
                setting.mDescription,
                setting.mReadOnly,
                parent),
        mData(setting.mData)
    {}

    SettingExt(const T& data, bool readOnly = false, QObject* parent = nullptr)
        : SettingExt<T>::SettingExt("", "", "", data, readOnly, parent)
    {}

    SettingExt(bool readOnly = false, QObject* parent = nullptr)
        : SettingExt<T>::SettingExt("", "", "", {}, readOnly, parent)
    {}

    void setData(const T& data)
    {
        mData = data;
    }

    T& getData()
    {
        return mData;
    }

    void setDataValue(DataValueType value)
    {
        if (mData.value != value)
        {
            mData.value = value;
            emitSignalDataChanged(value);
        }
    }

    DataValueType getDataValue() const
    {
        return mData.value;
    }

    DataValueType getDataDefaultValue() const
    {
        return mData.defaultValue;
    }

    void setValue(const QVariant& variant) override
    {
        setDataValue(variant.value<DataValueType>());
    }

    void notifyValue(const QVariant& variant) override
    {
        emitSignalDataChanged(variant);
    }

    QVariant getValue() const override
    {
        return getDataValue();
    }

    QVariant getDefaultValue() const override
    {
        return getDataDefaultValue();
    }

    operator DataValueType() const
    {
        return getDataValue();
    }

    SettingExt<T>& operator=(const DataValueType& value)
    {
        setDataValue(value);
        return *this;
    }

    SettingExt<T>& operator=(const SettingExt<T>& setting)
    {
        mKey = setting.mKey;
        mCaption = setting.mCaption;
        mDescription = setting.mDescription;
        mReadOnly = setting.mReadOnly;
        setDataValue(setting.mData);
        return *this;
    }

private:
    T mData;
};

using SettingInt                  = SettingExt<DataInteger>;
using SettingUInt                 = SettingExt<DataUnsigned>;
using SettingDouble               = SettingExt<DataDouble>;
using SettingBool                 = SettingExt<DataBool>;
using SettingString               = SettingExt<DataStringMask>;
using SettingStringList           = SettingExt<DataStringList>;
using SettingCheckList            = SettingExt<DataCheckList>;
using SettingByteArray            = SettingExt<DataByteArray>;
using SettingFont                 = SettingExt<DataFont>;
using SettingColor                = SettingExt<DataColor>;
using SettingSource               = SettingExt<DataSource>;
using SettingDateTime             = SettingExt<DataDateTime>;
using SettingEditableStringList   = SettingExt<DataEditableStringList>;
using SettingCheckableStringList  = SettingExt<DataCheckableStringList>;
using SettingChangeableStringList = SettingExt<DataChangeableStringList>;

}  // namespace custom_setting
