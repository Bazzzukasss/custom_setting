#include "custom_setting_data.h"

custom_setting::DataInteger::DataInteger(int val, int min, int max, int default_val,
                                                 const QString& value_suffix) :
    DataDigit<int>(val, min, max, default_val),
    suffix(value_suffix)
{}

custom_setting::DataDouble::DataDouble(double val, double min, double max, double default_val,
                                               int value_decimals, const QString& value_suffix) :
    DataDigit<double>(val, min, max, default_val),
    decimals(value_decimals),
    suffix(value_suffix)
{}

custom_setting::DataSource::DataSource(QString val, QString default_val, SourceType source_type,
                                               QString filter) :
    Data<QString>(val, default_val),
    sourceType(source_type),
    filters(filter)
{}

custom_setting::DataChangeableList::DataChangeableList(const QStringList& val, const QStringList& default_val,
                                                               ListType list_type, bool is_uniq_items,
                                                               const QString& file_filters) :
    Data<QStringList>(val, default_val),
    listType(list_type),
    isUniqItems(is_uniq_items),
    filters(file_filters)
{}

custom_setting::DataStringMask::DataStringMask(QString val, QString default_val, QString inputMask) :
    Data<QString>(val, default_val),
    regexValidatorString(inputMask)
{}
