#include "custom_setting_loader.h"

using namespace custom_setting;

Loader::Loader(QObject *parent)
    : QObject(parent)
{}

Loader *Loader::create(const QString &filename, QObject *parent)
{
    QFileInfo finfo(filename);
    auto ext = finfo.suffix();

    if(ext == "ini")
    {
        return new LoaderIni(filename, parent);
    }
    else if(ext == "json")
    {
        return new LoaderJson(filename, parent);
    }

    return nullptr;
}

LoaderIni::LoaderIni(const QString &filename, QObject *parent)
    : Loader(parent)
    , mSettings(new QSettings(filename, QSettings::IniFormat, parent))
{}

void LoaderIni::setValue(const QString& key, const QVariant &value)
{
    mSettings->setValue(key, value);
}

QVariant LoaderIni::getValue(const QString& key, const QVariant& default_value)
{
    return mSettings->value(key, default_value);
}

LoaderJson::LoaderJson(const QString &filename, QObject *parent)
    : Loader(parent)
{
}

void LoaderJson::setValue(const QString& key, const QVariant &value)
{
}

QVariant LoaderJson::getValue(const QString& key, const QVariant &default_value)
{
}
