#pragma once

#include <QVariant>
#include <QFileInfo>
#include <QObject>
#include <QSettings>
#include <QJsonObject>

namespace custom_setting {

class SerializerIni;
class SerializerJson;

class Serializer : public QObject
{
    Q_OBJECT
public:
    enum class Mode{kRead, kWrite};
    Serializer(const QString &filename, Mode mode, QObject* parent = nullptr);
    virtual ~Serializer() = default;

    virtual void setValue(const QString& key, const QVariant& value, bool as_plain_value = true) = 0;
    virtual QVariant getValue(const QString& key, const QVariant& default_value, bool as_plain_value) = 0;
    virtual void sync() = 0;

    static Serializer* create(const QString& filename, Mode mode, QObject* parent = nullptr);

protected:
    QString mFilename;
    Mode mMode;
};

class SerializerIni : public Serializer
{
    Q_OBJECT
public:
    SerializerIni(const QString& filename, Mode mode, QObject* parent = nullptr);

    void setValue(const QString& key, const QVariant& value, bool as_plain_value) override;
    QVariant getValue(const QString &key, const QVariant& default_value, bool as_plain_value) override;
    void sync() override;

private:
    QSettings* mSettings;
};

class SerializerJson : public Serializer
{
    Q_OBJECT
public:
    SerializerJson(const QString& filename, Mode mode, QObject* parent = nullptr);

    void setValue(const QString& key, const QVariant& value, bool as_plain_value) override;
    QVariant getValue(const QString& key, const QVariant& default_value, bool as_plain_value) override;
    void sync() override;

private:
    QJsonObject mJsonObject;

    void write(QJsonObject& obj, const QString& key, const QVariant &value);
    QVariant read(QJsonObject obj, const QString& key, const QVariant &default_value);
};

}

