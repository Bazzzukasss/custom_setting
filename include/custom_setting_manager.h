#pragma once

#include "custom_setting.h"
#include <QDir>
#include <QApplication>

namespace custom_setting
{
/**
 * Class Configurator
 * @brief Base class for application settings managing
 */
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
    virtual QString getSettingsDirPath() const { return {qApp->applicationDirPath() + QDir::separator()}; }
    void setConfigurations(const ConfigurationsMap& configurations);

    ConfigurationsMap mConfigurations;
};

}  // namespace custom_setting
