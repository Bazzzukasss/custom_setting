#include "custom_setting_serializer.h"
#include <QJsonDocument>

using namespace custom_setting;

Serializer::Serializer(const QString& filename, Mode mode, QObject* parent) :
    QObject(parent),
    mFilename(filename),
    mMode(mode)
{}

Serializer* Serializer::create(const QString& filename, Mode mode, QObject* parent)
{
    QFileInfo finfo(filename);
    auto ext = finfo.suffix();

    if (ext == "ini")
    {
        return new SerializerIni(filename, mode, parent);
    }
    else if (ext == "json")
    {
        return new SerializerJson(filename, mode, parent);
    }

    return nullptr;
}

SerializerIni::SerializerIni(const QString& filename, Mode mode, QObject* parent)
    : Serializer(filename, mode, parent)
    , mSettings(new QSettings(filename, QSettings::IniFormat, parent))
{}

void SerializerIni::setValue(const QString& key, const QVariant& value, bool)
{
    mSettings->setValue(key, value);
}

QVariant SerializerIni::getValue(const QString& key,
                                 const QVariant& defaultValue,
                                 bool)
{
    return mSettings->value(key, defaultValue);
}

void SerializerIni::sync()
{
    mSettings->sync();
}

SerializerJson::SerializerJson(const QString& filename, Mode mode, QObject* parent)
    : Serializer(filename, mode, parent)
{
    if (mMode == Serializer::Mode::kRead)
    {
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly))
        {
           qWarning("Couldn't open file.");
           return;
        }

        QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
        mJsonObject = doc.object();
    }
}

void SerializerJson::setValue(const QString& key,
                              const QVariant& value,
                              bool asPlainValue)
{
    int n = key.indexOf("/");
    QString lastKey = key.right(key.length() - (n + 1));

    if (asPlainValue)
    {
        mJsonObject[lastKey] = value.toJsonValue();
    }

    write(mJsonObject, lastKey, value);
}

QVariant SerializerJson::getValue(const QString& key,
                                  const QVariant& defaultValue,
                                  bool asPlainValue)
{
    int n = key.indexOf("/");
    QString lastKey = key.right(key.length() - (n + 1));

    if (asPlainValue)
    {
        return mJsonObject.contains(lastKey) ? mJsonObject[lastKey].toVariant()
                                             : defaultValue;
    }

    return read(mJsonObject, lastKey, defaultValue);
}

void SerializerJson::sync()
{
    QFile file(mFilename);

    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open file.");
        return;
    }

    QJsonDocument doc(mJsonObject);
    file.write(doc.toJson());
}

void SerializerJson::write(QJsonObject& obj,
                           const QString& key,
                           const QVariant& value)
{
    int n = key.indexOf("/");

    if (n == -1)
    {
        obj[key] = QJsonValue::fromVariant(value);
    }
    else
    {
        QJsonObject subObj;
        QString lastKey = key.right(key.length() - (n + 1));
        QString curKey = key.left(n);

        if (!obj[curKey].isNull())
        {
            subObj = obj[curKey].toObject();
        }

        write(subObj, lastKey, value);
        obj[curKey] = subObj;
    }
}

QVariant SerializerJson::read(QJsonObject obj,
                              const QString& key,
                              const QVariant& defaultValue)
{
    int n = key.indexOf("/");

    if (n == -1)
    {
        return obj.contains(key) ? obj[key].toVariant()
                                 : defaultValue;
    }
    else
    {
        QJsonObject subObj;
        QString lastKey = key.right(key.length() - (n + 1));
        QString curKey = key.left(n);

        return read(obj[curKey].toObject(), lastKey, defaultValue);
    }
}
