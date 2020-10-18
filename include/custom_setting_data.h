#pragma once

#include <QString>
#include <QStringList>
#include <QMatrix>
#include <QVector>
#include <limits>
#include <QFont>
#include <QColor>

namespace custom_setting
{

/**
 * Template structure CustomDataTypeTraits
 * @brief Struct of type traits. Gives possibility to identify CustomDataExt<T> template parameters type.
 */
template <typename T>
struct DataTypeTraits
{
    using ValueType = T;
};

/**
 * Template structure CustomDataExt
 * @brief Base Structure for custom data types family extends by template.
 * Member resetValue can be used to revert value changes by reset() method.
 */
template <typename T>
struct Data : public DataTypeTraits<T>
{
    Data(T val = {}, T default_val = {});

    T value;
    T defaultValue;
    T resetValue;

    inline void toDefault() { value = defaultValue; }
    inline void reset() { value = resetValue; }
};

/**
 * Template structure CustomDataDigit
 * @brief Base Structure for numerical custom data types family.
 */
template <typename T>
struct DataDigit : public Data<T>
{
    DataDigit(T val = 0, T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max(),
                    T default_val = 0);

    T minimum;
    T maximum;
};

/**
 * Template structure CustomDataList
 * @brief Base Structure for list custom data types family. Should be edited via CustomComboBox (depends on T type)
 * @param val contains value of one of the item of list
 */
template <typename T>
struct DataList : public Data<T>
{
    DataList(const QList<T>& lst = {}, T val = {}, T default_val = {});

    QList<T> list;
};

/**
 * Template structure CustomDataEditableList
 * @brief Base Structure for editable list custom data types family. Should be edited via CustomTextEdit
 * @param val contains value of one of the item of list
 */
struct DataEditableList : public Data<QStringList>
{
    DataEditableList(const QStringList& val = {}, const QStringList& default_val = {}) :
        Data<QStringList>(val, default_val)
    {}
};

/**
 * Template structure CustomDataChangeableList
 * @brief Base Structure for editable list custom data types family. Should be edited via CustomListBox
 * @param val contains value of one of the item of list
 */
struct DataChangeableList : public Data<QStringList>
{
    enum ListType
    {
        kStringList,
        kFileList,
        kDirList
    };
    DataChangeableList(const QStringList& val = {}, const QStringList& default_val = {},
                             ListType list_type = ListType::kStringList, bool is_uniq_items = false,
                             const QString& file_filters = {});

    ListType listType;
    bool isUniqItems;
    QString filters;
};

/**
 * Structure CustomDataInteger
 * @brief Struct represents integer application setting. Should be edited via CustomSpinBox or CustomSlider (or any
 * integer value editing custom widget). Member suffix contains string value of suffix which could be shown in widget.
 */
struct DataInteger : public DataDigit<int>
{
    DataInteger(int val = 0, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max(),
                      int default_val = std::numeric_limits<int>::min(), const QString& value_suffix = {});

    QString suffix;
};

/**
 * Structure CustomDataDouble
 * @brief Struct represents double application setting. Should be edited via CustomDoubleSpinBox (or any double value
 * editing custom widget). Member suffix contains string value of suffix which could be shown in widget.
 * Member decimals set number of shown digits after dot.
 */
struct DataDouble : public DataDigit<double>
{
    DataDouble(double val = 0, double min = -std::numeric_limits<double>::max(),
                     double max         = std::numeric_limits<double>::max(),
                     double default_val = std::numeric_limits<double>::min(), int value_decimals = 4,
                     const QString& value_suffix = {});

    int decimals;
    QString suffix;
};

/**
 * Structure CustomDataPath
 * @brief The struct represents the files path application setting. Should be edited via QFileDialog.
 * Member filters contains string value of filters for QFileOpenDialog.
 */
struct DataSource : public Data<QString>
{
    enum SourceType
    {
        kFile,
        kDir
    };
    DataSource(QString val = {}, QString default_val = {}, SourceType source_type = SourceType::kFile,
                     QString filter = {});

    SourceType sourceType;
    QString filters;
};

/**
 * Structure CustomDataStringMask
 * @brief  Struct represents integer application setting. Should be edited via CustomLineEdit
 * Member mask contains string value the input mask for the widget.
 */
struct DataStringMask : public Data<QString>
{
    DataStringMask(QString val = {}, QString default_val = {}, QString inputMask = {});

    QString regexValidatorString;
};

template <typename T>
Data<T>::Data(T val, T default_val) : value(val), defaultValue(default_val), resetValue(val)
{}

template <typename T>
DataDigit<T>::DataDigit(T val, T min, T max, T default_val) :
    Data<T>(val, default_val),
    minimum(min),
    maximum(max)
{}

template <typename T>
DataList<T>::DataList(const QList<T>& lst, T val, T default_val) :
    Data<T>(val, default_val),
    list(lst)
{}


using DataByteArray  = Data<QByteArray>;
using DataMatrix     = Data<QMatrix>;
using DataUnsigned   = Data<unsigned int>;
using DataString     = Data<QString>;
using DataBool       = Data<bool>;
using DataFont       = Data<QFont>;
using DataColor      = Data<QColor>;
using DataStringList = DataList<QString>;
using DataCheckList  = DataList<bool>;
using DataIntList    = DataList<int>;
using DataDoubleList = DataList<double>;
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
Q_DECLARE_METATYPE(custom_setting::DataEditableList)
Q_DECLARE_METATYPE(custom_setting::DataChangeableList)
