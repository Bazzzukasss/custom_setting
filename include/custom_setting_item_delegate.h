#pragma once

#include <QStyledItemDelegate>
#include <QPointer>

class CustomSettingWidget;
class QLabel;

namespace custom_setting {

class Setting;
class Item;

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT

    static const int kDefaultItemHeight{26};
    static const int kDefaultItemWidth{-1};
    static const int kDefaultRowsPerItem{4};

public:
    Delegate(QObject* parent =0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void setItemsHeight(int height);
    void setItemsWidth(int width);
    void setItemsRowsCount(int count);
    void setItemsSizeHint(int item_width, int item_height, int item_rows_count);

public slots:
    void slotCommit();

private:
    int mItemHeight{kDefaultItemHeight};
    int mItemWidth{kDefaultItemWidth};
    int mItemsRowsCount{kDefaultRowsPerItem};

    Setting *getSetting(const QModelIndex &index) const;    
    Item* getItem(const QModelIndex &index) const;
};
}
