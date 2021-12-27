#include "custom_setting_data.h"

using namespace custom_setting;

DataInteger::DataInteger(int val,
                         int min,
                         int max,
                         int defaultVal,
                         const QString& valueSuffix):
    DataDigit<int>(val, min, max, defaultVal),
    suffix(valueSuffix)
{}

DataDouble::DataDouble(double val,
                       double min,
                       double max,
                       double defaultVal,
                       int valueDecimals,
                       const QString& valueSuffix) :
    DataDigit<double>(val, min, max, defaultVal),
    decimals(valueDecimals),
    suffix(valueSuffix)
{}

DataSource::DataSource(QString val,
                       QString defaultVal,
                       SourceType sourceType,
                       QString filter) :
    Data<QString>(val, defaultVal),
    sourceType(sourceType),
    filters(filter)
{}

DataChangeableStringList::DataChangeableStringList(const QStringList& val,
                                                   const QStringList& defaultValue,
                                                   ListType listType,
                                                   bool isUniqItems,
                                                   const QString& fileFilters) :
    Data<QStringList>(val, defaultValue),
    listType(listType),
    isUniqItems(isUniqItems),
    filters(fileFilters)
{}

DataStringMask::DataStringMask(QString val,
                               QString defaultVal,
                               QString inputMask) :
    Data<QString>(val, defaultVal),
    regexValidatorString(inputMask)
{}

DataCheckableStringList::DataCheckableStringList(const QStringList& lst,
                                                 const QStringList& val,
                                                 const QStringList& defaultVal) :
    Data<QStringList>(val, defaultVal),
    list(lst)
{
}
