#include <QPainter>
#include <QFontDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QRegExpValidator>
#include <QVBoxLayout>

#include "custom_widgets.h"

using namespace custom_setting;

CustomSpinBox::CustomSpinBox(QWidget* parent) : QSpinBox(parent), mSetting(nullptr)
{}

void CustomSpinBox::bindToSetting(CustomSpinBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomSpinBox::editingFinished,
                this, &CustomSpinBox::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomSpinBox::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomSpinBox::setSetting(SettingType* setting)
{
    mSetting = setting;

    setMinimum(mSetting->getData().minimum);
    setMaximum(mSetting->getData().maximum);
    setSuffix(mSetting->getData().suffix);
    setValue(mSetting->getData().value);
    setReadOnly(mSetting->isReadOnly());

    if (mSetting->isReadOnly())
    {
        setButtonSymbols(ButtonSymbols::NoButtons);
    }
}

void CustomSpinBox::onEditingFinished()
{
    mSetting->setDataValue(value());
    emit signalEditingFinished();
}

void CustomSpinBox::onSettingDataChanged()
{
    blockSignals(true);
    setValue(mSetting->getData().value);
    blockSignals(false);
}

CustomSlider::CustomSlider(QWidget* parent) : QSlider(parent), mSetting(nullptr)
{}

void CustomSlider::bindToSetting(CustomSlider::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomSlider::valueChanged,
                this, &CustomSlider::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomSlider::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomSlider::setSetting(SettingType* setting)
{
    mSetting = setting;

    setMinimum(mSetting->getData().minimum);
    setMaximum(mSetting->getData().maximum);
    setEnabled(!mSetting->isReadOnly());
}

void CustomSlider::onEditingFinished()
{
    mSetting->setDataValue(value());
    emit signalEditingFinished();
}

void CustomSlider::onSettingDataChanged()
{
    blockSignals(true);
    setValue(mSetting->getData().value);
    blockSignals(false);
}

CustomDoubleSpinBox::CustomDoubleSpinBox(QWidget* parent) :
    QDoubleSpinBox(parent),
    mSetting(nullptr)
{}

void CustomDoubleSpinBox::bindToSetting(CustomDoubleSpinBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        if (mSetting->isReadOnly())
        {
            setButtonSymbols(ButtonSymbols::NoButtons);
        }

        connect(this, &CustomDoubleSpinBox::editingFinished,
                this, &CustomDoubleSpinBox::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomDoubleSpinBox::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomDoubleSpinBox::setSetting(SettingType* setting)
{
    mSetting = setting;

    setMinimum(mSetting->getData().minimum);
    setMaximum(mSetting->getData().maximum);
    setDecimals(mSetting->getData().decimals);
    setSuffix(mSetting->getData().suffix);
    setValue(mSetting->getData().value);
    setReadOnly(mSetting->isReadOnly());
}

void CustomDoubleSpinBox::onEditingFinished()
{
    mSetting->setDataValue(value());
    emit signalEditingFinished();
}

void CustomDoubleSpinBox::onSettingDataChanged()
{
    blockSignals(true);
    setValue(mSetting->getData().value);
    blockSignals(false);
}

CustomCheckBox::CustomCheckBox(QWidget* parent) :
    QCheckBox(parent),
    mSetting(nullptr)
{}

void CustomCheckBox::bindToSetting(CustomCheckBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomCheckBox::stateChanged,
                this, &CustomCheckBox::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomCheckBox::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomCheckBox::setSetting(SettingType* setting)
{
    mSetting = setting;
    setEnabled(!mSetting->isReadOnly());
}

void CustomCheckBox::onEditingFinished()
{
    mSetting->setDataValue(checkState() == Qt::CheckState::Checked);
    emit signalEditingFinished();
}

void CustomCheckBox::onSettingDataChanged()
{
    blockSignals(true);
    setChecked(mSetting->getDataValue());
    blockSignals(false);
}

CustomLineEdit::CustomLineEdit(QWidget* parent) :
    QLineEdit(parent),
    mSetting(nullptr)
{
    connect(this, &QLineEdit::returnPressed, this, &QLineEdit::clearFocus);
}

void CustomLineEdit::bindToSetting(CustomLineEdit::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomLineEdit::editingFinished,
                this, &CustomLineEdit::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomLineEdit::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomLineEdit::setSetting(SettingType* setting)
{
    mSetting = setting;

    setReadOnly(mSetting->isReadOnly());

    const auto& regexString = mSetting->getData().regexValidatorString;
    if (!regexString.isEmpty())
    {
        auto validator = new QRegExpValidator(QRegExp(regexString));
        setValidator(validator);
    }
}

void CustomLineEdit::onEditingFinished()
{
    mSetting->setDataValue(text());
    emit signalEditingFinished();
}

void CustomLineEdit::onSettingDataChanged()
{
    blockSignals(true);
    setText(mSetting->getData().value);
    blockSignals(false);
}

CustomComboBox::CustomComboBox(QWidget* parent) :
    QComboBox(parent),
    mSetting(nullptr)
{}

void CustomComboBox::bindToSetting(CustomComboBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomComboBox::currentTextChanged,
                this, &CustomComboBox::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomComboBox::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomComboBox::setSetting(SettingType* setting)
{
    mSetting = setting;

    clear();

    for (const auto& item : mSetting->getData().list)
    {
        addItem(item);
    }

    setEnabled(!mSetting->isReadOnly());
}

void CustomComboBox::onEditingFinished()
{
    mSetting->setDataValue(currentText());
    emit signalEditingFinished();
}

void CustomComboBox::onSettingDataChanged()
{
    blockSignals(true);
    setCurrentText(mSetting->getData().value);
    blockSignals(false);
}

CustomFontButton::CustomFontButton(QWidget* parent) :
    QPushButton(parent),
    mSetting(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void CustomFontButton::bindToSetting(CustomFontButton::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomFontButton::clicked,
                this, &CustomFontButton::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomFontButton::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomFontButton::setSetting(SettingType* setting)
{
    mSetting = setting;
    setEnabled(!mSetting->isReadOnly());
    setText("Font");
}

void CustomFontButton::onEditingFinished()
{
    bool ok{false};
    QFont fnt{QFontDialog::getFont(&ok, mSetting->getData().value)};

    if (ok)
    {
        mSetting->setDataValue(fnt);
    }

    emit signalEditingFinished();
}

void CustomFontButton::onSettingDataChanged()
{
    setFont(mSetting->getData().value);
}

CustomColorButton::CustomColorButton(QWidget* parent) :
    QPushButton(parent),
    mSetting(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void CustomColorButton::bindToSetting(CustomColorButton::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomColorButton::clicked,
                this, &CustomColorButton::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomColorButton::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomColorButton::setSetting(SettingType* setting)
{
    mSetting = setting;

    setEnabled(!mSetting->isReadOnly());
}

void CustomColorButton::onEditingFinished()
{
    QColor clr{QColorDialog::getColor(mSetting->getDataValue())};

    if (clr.isValid())
    {
        mSetting->setDataValue(clr);
    }

    emit signalEditingFinished();
}

void CustomColorButton::onSettingDataChanged()
{
    setStyleSheet(
        QString("background-color: %1;").arg(mSetting->getData().value.name()));
}

CustomSourceButton::CustomSourceButton(QWidget* parent) :
    QPushButton(parent),
    mSetting(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void CustomSourceButton::bindToSetting(CustomSourceButton::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomFontButton::clicked,
                this, &CustomSourceButton::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomSourceButton::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomSourceButton::setSetting(SettingType* setting)
{
    mSetting = setting;
    setEnabled(!mSetting->isReadOnly());
}

void CustomSourceButton::onEditingFinished()
{
    QString filename;

    switch (mSetting->getData().sourceType)
    {
        case DataSource::kDir:
            filename =
                QFileDialog::getExistingDirectory(
                    this,
                    tr("Select directory"),
                    "~/",
                    QFileDialog::ShowDirsOnly |
                        QFileDialog::DontResolveSymlinks);
            break;
        case DataSource::kFile:
            filename =
                QFileDialog::getOpenFileName(
                    this,
                    tr("Select file"),
                    "~/",
                    mSetting->getData().filters);
            break;
    }

    if (!filename.isEmpty())
    {
        mSetting->setDataValue(filename);
    }

    emit signalEditingFinished();
}

void CustomSourceButton::onSettingDataChanged()
{
    setText(mSetting->getData().value);
}

CustomListBox::CustomListBox(QWidget* parent) : QComboBox(parent), mSetting(nullptr)
{}

void CustomListBox::bindToSetting(CustomListBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomComboBox::currentTextChanged,
                this, &CustomListBox::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomListBox::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomListBox::setSetting(SettingType* setting)
{
    mSetting = setting;

    setEnabled(!mSetting->isReadOnly());
}

void CustomListBox::onEditingFinished()
{
    const QString addItemText{"..."};
    const QString clearItemText{"Clear"};
    QString curText = currentText();
    QString newCurrentText{curText};
    QStringList list;

    if (curText != clearItemText)
    {
        if (curText == addItemText)
        {
            switch (mSetting->getData().listType)
            {
                case DataChangeableStringList::kStringList:
                    newCurrentText =
                        QInputDialog::getText(
                            this,
                            tr("New item"),
                            tr("Item:"),
                            QLineEdit::Normal);
                    break;
                case DataChangeableStringList::kFileList:
                    newCurrentText =
                        QFileDialog::getOpenFileName(
                            this,
                            tr("Select file"),
                            "~/",
                            mSetting->getData().filters);
                    break;
                case DataChangeableStringList::kDirList:
                    newCurrentText = QFileDialog::getExistingDirectory(
                        this,
                        tr("Select directory"),
                        "~/",
                        QFileDialog::ShowDirsOnly |
                            QFileDialog::DontResolveSymlinks);
                    break;
            }

            if (mSetting->getDataValue().contains(newCurrentText) &&
                mSetting->getData().isUniqItems)
            {
                newCurrentText.clear();
            }
        }

        if (!newCurrentText.isEmpty())
        {
            list << newCurrentText;
        }

        for (const auto& text : mSetting->getDataValue())
        {
            if (text != curText)
            {
                list << text;
            }
        }

        onSettingDataChanged();
    }

    mSetting->setDataValue(list);

    emit signalEditingFinished();
}

void CustomListBox::onSettingDataChanged()
{
    const QString addItemText{"..."};
    const QString clearItemText{"Clear"};

    blockSignals(true);

    clear();

    for (auto item : mSetting->getDataValue())
    {
        addItem(item);
    }

    if (mSetting->getDataValue().empty())
    {
        addItem("");
    }

    addItem(addItemText);
    addItem(clearItemText);
    setCurrentIndex(0);

    blockSignals(false);
}

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent), mSetting(nullptr)
{
    setAlignment(Qt::AlignVCenter);
}

void CustomLabel::bindToSetting(SettingType *setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomLabel::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomLabel::setSetting(SettingType* setting)
{
    mSetting = setting;

    setEnabled(!mSetting->isReadOnly());
}

void CustomLabel::onSettingDataChanged()
{
    if (auto boolSetting = dynamic_cast<SettingBool*>(mSetting))
    {
        setText(boolSetting->getData().value ? "+" : "-");
    }
    else if (auto intSetting = dynamic_cast<SettingInt*>(mSetting))
    {
        setText(QString("%1%2")
                    .arg(intSetting->getData().value)
                    .arg(intSetting->getData().suffix));
    }
    else if (auto doubleSetting = dynamic_cast<SettingDouble*>(mSetting))
    {
        setText(QString("%1%2")
                    .arg(doubleSetting->getData().value)
                    .arg(doubleSetting->getData().suffix));
    }
    else if (auto stringSetting = dynamic_cast<SettingString*>(mSetting))
    {
        setText(stringSetting->getData().value);
    }
    else if (auto listSetting = dynamic_cast<SettingStringList*>(mSetting))
    {
        setText(listSetting->getData().value);
    }
    else if (auto sourceSetting = dynamic_cast<SettingSource*>(mSetting))
    {
        setText(sourceSetting->getData().value);
    }
    else if (auto changeableListSetting =
                 dynamic_cast<SettingChangeableStringList*>(mSetting))
    {
        setText(changeableListSetting->getData().value.at(0));
    }
    else if (auto fontSetting = dynamic_cast<SettingFont*>(mSetting))
    {
        setFont(fontSetting->getData().value);
        setText("Font");
    }
    else if(auto colorSetting = dynamic_cast<SettingColor*>(mSetting))
    {
        setStyleSheet(QString("background-color: %1;")
                          .arg(colorSetting->getData().value.name()));
    }
    else if (auto editableListSetting =
                 dynamic_cast<SettingEditableStringList*>(mSetting))
    {
        QString str;
        auto count = editableListSetting->getData().value.size();
        for (const auto& value : editableListSetting->getData().value)
        {
            str += value + (count > 1 ? "\n" : "");
            count--;
        }
        setText(str);
    }
    else if (auto checkablebleListSetting =
                 dynamic_cast<SettingCheckableStringList*>(mSetting))
    {
        QString str;
        auto count = checkablebleListSetting->getData().list.size();
        for (const auto& value : checkablebleListSetting->getData().list)
        {
            str += checkablebleListSetting->getDataValue().contains(value)
                       ? "[+] "
                       : "[-] ";
            str += value + (count > 1 ? "\n" : "");
            count--;
        }
        setText(str);
    }
    else if (auto dateTimeSetting = dynamic_cast<SettingDateTime*>(mSetting))
    {
        setText(dateTimeSetting->getDataValue().toString());
    }
}

CustomEditableListWidget::CustomEditableListWidget(QWidget* parent) : QFrame(parent)
{
    mListWidget = new QListWidget(this);
    mApplyButton = new QPushButton("Apply", this);
    mAddButton = new QPushButton("+", this);
    mRemoveButton = new QPushButton("-", this);

    auto hLayout = new QHBoxLayout();
    hLayout->addWidget(mAddButton);
    hLayout->addWidget(mRemoveButton);
    hLayout->addWidget(mApplyButton);

    auto vLayout = new QVBoxLayout();
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->addWidget(mListWidget);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void CustomEditableListWidget::bindToSetting(SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(mApplyButton, &QPushButton::clicked,
                this, &CustomEditableListWidget::onEditingFinished);

        connect(mAddButton, &QPushButton::clicked,
                this, &CustomEditableListWidget::onAddItem);

        connect(mRemoveButton, &QPushButton::clicked,
                this, &CustomEditableListWidget::onRemoveItem);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomEditableListWidget::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomEditableListWidget::setSetting(SettingType* setting)
{
    mSetting = setting;

    setEnabled(!mSetting->isReadOnly());
    mRemoveButton->setVisible(!mSetting->isReadOnly());
    mAddButton->setVisible(!mSetting->isReadOnly());
    mApplyButton->setVisible(!mSetting->isReadOnly());
}

void CustomEditableListWidget::onAddItem()
{
    auto item = new QListWidgetItem("value");

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    mListWidget->addItem(item);
}

void CustomEditableListWidget::onRemoveItem()
{
    mListWidget->removeItemWidget(
        mListWidget->takeItem(
            mListWidget->currentRow()));
}

void CustomEditableListWidget::onEditingFinished()
{
    QStringList lst;

    for(int i = 0; i < mListWidget->count(); ++i)
    {
        lst << mListWidget->item(i)->text();
    }

    mSetting->setDataValue(lst);

    emit signalEditingFinished();
}

void CustomEditableListWidget::onSettingDataChanged()
{
    blockSignals(true);

    mListWidget->clear();
    mListWidget->addItems(mSetting->getDataValue());

    for (int i = 0; i < mListWidget->count(); ++i)
    {
        auto it = mListWidget->item(i);
        it->setFlags(it->flags() | Qt::ItemIsEditable);
    }

    blockSignals(false);
}

CustomCheckableListWidget::CustomCheckableListWidget(QWidget *parent) : QFrame(parent)
{
    mListWidget = new QListWidget(this);
    mApplyButton = new QPushButton("Apply", this);

    auto hLayout = new QHBoxLayout();
    hLayout->addWidget(mApplyButton);

    auto vLayout = new QVBoxLayout();
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->addWidget(mListWidget);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void CustomCheckableListWidget::bindToSetting(SettingType *setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(mApplyButton, &QPushButton::clicked,
                this, &CustomCheckableListWidget::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomCheckableListWidget::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomCheckableListWidget::wheelEvent(QWheelEvent*)
{
    return;
}

void CustomCheckableListWidget::setSetting(SettingType* setting)
{
    mSetting = setting;

    setEnabled(!mSetting->isReadOnly());
    mApplyButton->setVisible(!mSetting->isReadOnly());
}

void CustomCheckableListWidget::onEditingFinished()
{
    QStringList lst;

    for(int i = 0; i < mListWidget->count(); ++i)
    {
        if (mListWidget->item(i)->checkState() == Qt::Checked)
        {
            lst << mListWidget->item(i)->text();
        }
    }

    mSetting->setDataValue(lst);

    emit signalEditingFinished();
}

void CustomCheckableListWidget::onSettingDataChanged()
{
    blockSignals(true);

    mListWidget->clear();
    mListWidget->addItems(mSetting->getData().list);

    for(int i = 0; i < mListWidget->count(); ++i)
    {
        auto it = mListWidget->item(i);
        auto state = mSetting->getDataValue().contains(it->text())
                         ? Qt::Checked
                         : Qt::Unchecked;
        it->setFlags(it->flags() | Qt::ItemIsUserCheckable);
        it->setCheckState(state);
    }

    blockSignals(false);
}

CustomDateTimeEdit::CustomDateTimeEdit(QWidget* parent) :
    QDateTimeEdit(parent),
    mSetting(nullptr)
{
}

void CustomDateTimeEdit::bindToSetting(CustomDateTimeEdit::SettingType* setting)
{
    if (setting != nullptr)
    {
        setSetting(setting);

        connect(this, &CustomDateTimeEdit::editingFinished,
                this, &CustomDateTimeEdit::onEditingFinished);

        connect(mSetting, &Setting::signalDataChanged,
                this, &CustomDateTimeEdit::onSettingDataChanged);

        onSettingDataChanged();
    }
}

void CustomDateTimeEdit::setSetting(SettingType* setting)
{
    mSetting = setting;

    setReadOnly(mSetting->isReadOnly());
}

void CustomDateTimeEdit::onEditingFinished()
{
    mSetting->setDataValue(dateTime());

    emit signalEditingFinished();
}

void CustomDateTimeEdit::onSettingDataChanged()
{
    blockSignals(true);
    setDateTime(mSetting->getDataValue());
    blockSignals(false);
}
