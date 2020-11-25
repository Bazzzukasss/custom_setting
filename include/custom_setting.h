#pragma once

#include <QSettings>
#include <functional>
#include "custom_setting_data.h"
#include "custom_setting_manager.h"

namespace custom_setting
{

class Serializer;
/**
 * Class CustomSetting
 * @brief General base class for any kind of application settings subclasses. Provides unific interface for
 * AppConfigurator routines.
 */
class Setting : public QObject
{
    Q_OBJECT
 public:
    using Vec = QVector<Setting*>;

    Setting(const QString& key, const QString& caption, const QString& description, bool read_only = false, QObject* parent = nullptr);
    virtual ~Setting() = default;

    void addSettings(const Vec& settings);
    void bindTo(std::function<void(const QVariant&)> procedure);
    void bindTo(std::function<void(void)> procedure);

    /*Getters*/
    inline const QString& getCaption() const { return mCaption; }
    inline const QString& getDescription() const { return mDescription; }
    inline const Vec& getSettings() const { return mSettings; }
    inline bool isReadOnly() const { return mReadOnly; }
    virtual bool isAnyChecked() const;    
    virtual QVariant getValue() const { return {}; }
    virtual QVariant getDefaultValue() const { return {}; }

    /*Setters*/
    inline void setDescription(const QString& description) { mDescription = description; }
    virtual void setValue(const QVariant&) {}

signals:
    void signalDataChanged(const QVariant&);

 protected:
    inline void emitSignalDataChanged() { emit signalDataChanged(getValue()); }

    Vec mSettings;
    QString mKey;
    QString mCaption;
    QString mDescription;
    bool mReadOnly;

private:
    virtual void load(Serializer* serializer, const QString& parentKey = {});
    virtual void save(Serializer* serializer, const QString& parentKey = {});

friend Manager;
};

/**
 * Tepmlate class CustomSettingExt<T>
 * @brief Base temlate class for application settings. Provides unific interface for manipulation of setting data (T
 * Data) via CustomWidgets family classes
 */
template <typename T>
class SettingExt : public Setting
{
    using DataValueType = typename T::ValueType;

 public:
    SettingExt(const QString& key, const QString& caption, const QString& description, const T& data = {},
                     bool read_only = false, QObject* parent = nullptr);
    SettingExt(const SettingExt<T>& setting, QObject* parent = nullptr);

    void setData(const T& data) { mData = data; }
    T& getData() { return mData; }

    DataValueType getDataValue() const { return mData.value; }
    DataValueType getDataDefaultValue() const { return mData.defaultValue; }
    void setDataValue(DataValueType value);

    operator DataValueType() const;
    SettingExt<T>& operator=(const DataValueType& value);
    SettingExt<T>& operator=(const SettingExt<T>& setting);

 protected:
    void setValue(const QVariant& variant) override { setDataValue(variant.value<DataValueType>()); }
    QVariant getValue() const override { return getDataValue(); }
    QVariant getDefaultValue() const override { return getDataDefaultValue(); }

 private:
    T mData;
};

template <typename T>
SettingExt<T>::SettingExt(const QString& key, const QString& caption, const QString& description,
                                      const T& data, bool read_only, QObject* parent) :
    Setting(key, caption, description, read_only, parent),
    mData(data)
{}

template<typename T>
SettingExt<T>::SettingExt(const SettingExt<T> &setting, QObject* parent) :
    Setting(setting.mKey, setting.mCaption, setting.mDescription, setting.mReadOnly, parent),
    mData(setting.mData)
{}

template <typename T>
void SettingExt<T>::setDataValue(SettingExt<T>::DataValueType value)
{
    if (mData.value != value)
    {
        mData.value = value;
        emitSignalDataChanged();
    }
}

template <typename T>
SettingExt<T>& SettingExt<T>::operator=(const SettingExt::DataValueType& value)
{
    setDataValue(value);
    return *this;
}

template <typename T>
SettingExt<T>& SettingExt<T>::operator=(const SettingExt<T>& setting)
{
    mKey = setting.mKey;
    mCaption = setting.mCaption;
    mDescription = setting.mDescription;
    mReadOnly = setting.mReadOnly;
    setDataValue(setting.mData);
    return *this;
}

template <typename T>
SettingExt<T>::operator DataValueType() const
{
    return getDataValue();
}

/**
 * Template class CustomSettingArray<T>
 * @brief Class represents array of CustomData objects
 */
template <typename T>
class SettingArray : public Setting
{
    using DataValueType = typename T::ValueType;

 public:
    SettingArray(const QString& key, const QString& caption, const QString& description,
                       const QVector<T>& data = {}, QObject* parent = nullptr);

    inline void addSettings(QVector<Setting*> settings) = delete;

    void setData(const QVector<T>& data) { mData = data; }
    QVector<T>& getData() { return mData; }
    void clearData();

    DataValueType getDataValue() const;
    DataValueType getDataDefaultValue() const;
    void setDataValue(DataValueType value);
    void addDataValue(DataValueType value);

    void setArrayIndex(int index);

    operator DataValueType() const;

    SettingArray<T>& operator=(const DataValueType& value);
    SettingArray<T>& operator=(const QVector<T>& data);

 protected:
    void setValue(const QVariant& variant) override { setDataValue(variant.value<DataValueType>()); }
    QVariant getValue() const override { return QVariant::fromValue(getDataValue()); }
    QVariant getDefaultValue() const override { return QVariant::fromValue(getDataDefaultValue()); }

 private:
    int mArrayIndex;
    QVector<T> mData;

    void load(Serializer* loader, const QString& parentKey) override;
    void save(Serializer* loader, const QString& parentKey) override;
};

template <typename T>
SettingArray<T>::SettingArray(const QString& key, const QString& caption, const QString& description,
                                          const QVector<T>& data, QObject* parent) :
    Setting(key, caption, description, parent),
    mArrayIndex(-1),
    mData(data)
{}

template <typename T>
void SettingArray<T>::load(Serializer* loader, const QString& parentKey)
{/*
    const auto& key = parentKey + "/" + mKey;

    int size = settings.beginReadArray(key);
    for (int i = 0; i < size; ++i)
    {
        if (i >= mData.size())
            mData.push_back({});

        setArrayIndex(i);
        settings.setArrayIndex(i);
        auto value = settings.value("value", getDefaultValue());
        if (value.isValid())
        {
            setValue(value);
        }
    }
    settings.endArray();*/
}

template <typename T>
void SettingArray<T>::save(Serializer* loader, const QString& parentKey)
{/*
    const auto& key = parentKey + "/" + mKey;

    settings.beginWriteArray(key);
    for (int i = 0; i < mData.size(); ++i)
    {
        setArrayIndex(i);
        settings.setArrayIndex(i);

        const auto& value = getValue();
        if (value.isValid())
        {
            settings.setValue("value", value);
        }
    }
    settings.endArray();*/
}

template <typename T>
typename SettingArray<T>::DataValueType SettingArray<T>::getDataValue() const
{
    return mArrayIndex >= 0 ? mData[mArrayIndex].value : DataValueType();
}

template <typename T>
typename SettingArray<T>::DataValueType SettingArray<T>::getDataDefaultValue() const
{
    return mArrayIndex >= 0 ? mData[mArrayIndex].defaultValue : DataValueType();
}

template <typename T>
void SettingArray<T>::setDataValue(SettingArray::DataValueType value)
{
    if (mArrayIndex >= 0)
    {
        if (mData[mArrayIndex].value != value)
        {
            mData[mArrayIndex].value = value;
            emitSignalDataChanged();
        }
    }
}

template <typename T>
void SettingArray<T>::addDataValue(SettingArray::DataValueType value)
{
    mData.push_back(value);
}

template <typename T>
void SettingArray<T>::setArrayIndex(int index)
{
    if (index < mData.size() && index >= 0)
        mArrayIndex = index;
}

template <typename T>
SettingArray<T>& SettingArray<T>::operator=(const SettingArray::DataValueType& value)
{
    setDataValue(value);
    return *this;
}

template <typename T>
SettingArray<T>& SettingArray<T>::operator=(const QVector<T>& data)
{
    setData(data);
    return *this;
}

template <typename T>
void SettingArray<T>::clearData()
{
    mData.clear();
}

template <typename T>
SettingArray<T>::operator DataValueType() const
{
    return getDataValue();
}

using SettingArrayString    = SettingArray<DataString>;
using SettingInt            = SettingExt<DataInteger>;
using SettingUInt           = SettingExt<DataUnsigned>;
using SettingDouble         = SettingExt<DataDouble>;
using SettingBool           = SettingExt<DataBool>;
using SettingString         = SettingExt<DataStringMask>;
using SettingStringList     = SettingExt<DataStringList>;
using SettingCheckList      = SettingExt<DataCheckList>;
using SettingByteArray      = SettingExt<DataByteArray>;
using SettingFont           = SettingExt<DataFont>;
using SettingColor          = SettingExt<DataColor>;
using SettingSource         = SettingExt<DataSource>;
using SettingEditableList   = SettingExt<DataEditableList>;
using SettingChangeableList = SettingExt<DataChangeableList>;
}  // namespace custom_setting
