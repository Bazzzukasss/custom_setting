#pragma once

#include <QVariant>
#include <QFileInfo>
#include <QObject>
#include <QSettings>

namespace custom_setting {

class LoaderIni;
class LoaderJson;

class Loader : public QObject
{
    Q_OBJECT
public:
    Loader(QObject* parent = nullptr);
    virtual ~Loader() = default;

    virtual void setValue(const QString& key, const QVariant& value) = 0;
    virtual QVariant getValue(const QString& key, const QVariant& default_value) = 0;

    static Loader* create(const QString& filename, QObject* parent = nullptr);
};

class LoaderIni : public Loader
{
    Q_OBJECT
public:
    LoaderIni(const QString& filename, QObject* parent = nullptr);

    void setValue(const QString& key, const QVariant& value) override;
    QVariant getValue(const QString &key, const QVariant& default_value) override;

private:
    QSettings* mSettings;
};

class LoaderJson : public Loader
{
    Q_OBJECT
public:
    LoaderJson(const QString& filename, QObject* parent = nullptr);

    void setValue(const QString& key, const QVariant& value) override;
    QVariant getValue(const QString& key, const QVariant& default_value) override;
};

}

