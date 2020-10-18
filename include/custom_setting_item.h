#pragma once

#include <QObject>
#include "custom_setting.h"

namespace custom_setting
{

class Item : public Setting
{
    Q_OBJECT
public:
    using Vec = QVector<Item*>;

    explicit Item(const QString& key, const QString& caption, const QString& description, QObject* parent = nullptr);

    void addItems(const Vec& items);
    inline const Vec& getItems() const { return mItems; }

protected:
    Vec mItems;
};

} // namespace custom_setting
