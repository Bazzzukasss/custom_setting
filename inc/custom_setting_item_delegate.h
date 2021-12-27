#pragma once

#include <QStyledItemDelegate>

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

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const;

    QWidget *createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const;

    void setEditorData(QWidget* editor, const QModelIndex& index) const;

    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const;

    void setItemsHeight(int height);
    void setItemsWidth(int width);
    void setItemsRowsCount(int count);
    void setItemsSizeHint(int itemWidth, int itemHeight, int itemRowsCount);
    void setItemsInactiveShowMode(bool showInactiveItemsAsReadOnly);

signals:
    void itemEditionStarted() const;
    void itemEditionFinished() const;

public slots:
    void slotCommit();

private:
    void onEditorClosed(QWidget* editor) const;

private:
    int mItemHeight{kDefaultItemHeight};
    int mItemWidth{kDefaultItemWidth};
    int mItemsRowsCount{kDefaultRowsPerItem};
    bool mShowInactiveItemsAsReadOnly{false};
    mutable QList<QWidget*> mEditors;
};

} // namespace custom_setting
