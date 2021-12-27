#pragma once

#include <QDir>
#include <QApplication>

namespace custom_setting
{

class Setting;

class Manager : public QObject
{
    Q_OBJECT
    using ConfigurationsMap = QMap<QString, Setting*>;

public:
    explicit Manager(QObject* parent = nullptr);
    virtual ~Manager() = default;

    virtual void loadConfigurations();
    virtual void saveConfigurations();
    virtual void deleteConfiguration(const QString& filename);

signals:
    void signalDataChanged();
    void signalDataLoaded();

protected:
    ConfigurationsMap mConfigurations;

protected:
    virtual QString getSettingsDirPath() const;
    void setConfigurations(const ConfigurationsMap& configurations);
};

}  // namespace custom_setting
