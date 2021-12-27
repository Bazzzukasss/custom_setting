#pragma once

#include <QString>
#include <QStringList>
#include <QMatrix>
#include <QVector>
#include <limits>
#include <QFont>
#include <QColor>
#include <QDateTime>

namespace custom_setting
{

template <typename T>
struct DataTypeTraits
{
    using ValueType = T;
};

template <typename T>
struct Data : public DataTypeTraits<T>
{
    Data(T val = {}, T defaultVal = {});

    T value;
    T defaultValue;
    T resetValue;

    inline void toDefault() { value = defaultValue; }
    inline void reset() { value = resetValue; }
};

template <typename T>
struct DataDigit : public Data<T>
{
    DataDigit(T val = 0,
              T min = std::numeric_limits<T>::min(),
              T max = std::numeric_limits<T>::max(),
              T defaultValue = 0);

    T minimum;
    T maximum;
};

template <typename T>
struct DataList : public Data<T>
{
    DataList(const QList<T>& lst = {}, T val = {}, T defaultValue = {});

    inline int indexOf(T value) { return list.indexOf(value); }

    QList<T> list;
};

struct DataCheckableStringList : public Data<QStringList>
{
    DataCheckableStringList(const QStringList& lst = {},
                            const QStringList &val = {},
                            const QStringList &defaultVal = {});

    QStringList list;
};

struct DataChangeableStringList : public Data<QStringList>
{
    enum ListType
    {
        kStringList,
        kFileList,
        kDirList
    };
    DataChangeableStringList(const QStringList& val = {},
                             const QStringList& defaultValue = {},
                             ListType listType = ListType::kStringList,
                             bool isUniqItems = false,
                             const QString& fileFilters = {});

    ListType listType;
    bool isUniqItems;
    QString filters;
};

struct DataInteger : public DataDigit<int>
{
    DataInteger(int val = 0,
                int min = std::numeric_limits<int>::min(),
                int max = std::numeric_limits<int>::max(),
                int defaultVal = std::numeric_limits<int>::min(),
                const QString& valueSuffix = {});

    QString suffix;
};

struct DataDouble : public DataDigit<double>
{
    DataDouble(double val = 0,
               double min = -std::numeric_limits<double>::max(),
               double max = std::numeric_limits<double>::max(),
               double defaultVal = std::numeric_limits<double>::min(),
               int valueDecimals = 4,
               const QString& valueSuffix = {});

    int decimals;
    QString suffix;
};

struct DataSource : public Data<QString>
{
    enum SourceType
    {
        kFile,
        kDir
    };
    DataSource(QString val = {},
               QString defaultVal = {},
               SourceType sourceType = SourceType::kFile,
               QString filter = {});

    SourceType sourceType;
    QString filters;
};

struct DataStringMask : public Data<QString>
{
    DataStringMask(QString val = {}, QString defaultVal = {}, QString inputMask = {});

    QString regexValidatorString;
};

template <typename T>
Data<T>::Data(T val, T defaultVal) :
    value(val),
    defaultValue(defaultVal),
    resetValue(val)
{}

template <typename T>
DataDigit<T>::DataDigit(T val, T min, T max, T defaultValue) :
    Data<T>(val, defaultValue),
    minimum(min),
    maximum(max)
{}

template <typename T>
DataList<T>::DataList(const QList<T>& lst, T val, T defaultValue) :
    Data<T>(val, defaultValue),
    list(lst)
{}


using DataByteArray             = Data<QByteArray>;
using DataMatrix                = Data<QMatrix>;
using DataUnsigned              = Data<unsigned int>;
using DataString                = Data<QString>;
using DataBool                  = Data<bool>;
using DataFont                  = Data<QFont>;
using DataColor                 = Data<QColor>;
using DataDateTime              = Data<QDateTime>;
using DataEditableStringList    = Data<QStringList>;
using DataStringList            = DataList<QString>;
using DataCheckList             = DataList<bool>;
using DataIntList               = DataList<int>;
using DataDoubleList            = DataList<double>;

}  // namespace custom_setting

Q_DECLARE_METATYPE(custom_setting::DataByteArray)
Q_DECLARE_METATYPE(custom_setting::DataUnsigned)
Q_DECLARE_METATYPE(custom_setting::DataBool)
Q_DECLARE_METATYPE(custom_setting::DataInteger)
Q_DECLARE_METATYPE(custom_setting::DataDouble)
Q_DECLARE_METATYPE(custom_setting::DataString)
Q_DECLARE_METATYPE(custom_setting::DataStringMask)
Q_DECLARE_METATYPE(custom_setting::DataStringList)
Q_DECLARE_METATYPE(custom_setting::DataCheckList)
Q_DECLARE_METATYPE(custom_setting::DataIntList)
Q_DECLARE_METATYPE(custom_setting::DataDoubleList)
Q_DECLARE_METATYPE(custom_setting::DataFont)
Q_DECLARE_METATYPE(custom_setting::DataColor)
Q_DECLARE_METATYPE(custom_setting::DataDateTime)
Q_DECLARE_METATYPE(custom_setting::DataEditableStringList)
Q_DECLARE_METATYPE(custom_setting::DataChangeableStringList)
Q_DECLARE_METATYPE(custom_setting::DataCheckableStringList)
