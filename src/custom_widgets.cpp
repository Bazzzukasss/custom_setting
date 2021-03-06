#include <QPainter>
#include <QFontDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QRegExpValidator>

#include "custom_widgets.h"

using namespace custom_setting;

CustomSpinBox::CustomSpinBox(QWidget* parent) : QSpinBox(parent), mSetting(nullptr)
{}

void CustomSpinBox::bindToSetting(CustomSpinBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setMinimum(mSetting->getData().minimum);
        setMaximum(mSetting->getData().maximum);
        setSuffix(mSetting->getData().suffix);
        setValue(mSetting->getData().value);
        setReadOnly(mSetting->isReadOnly());
        if(mSetting->isReadOnly())
        {
            setButtonSymbols(ButtonSymbols::NoButtons);
        }

        connect(this, &CustomSpinBox::editingFinished, this, [&]() {
            mSetting->setDataValue(value());
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomSpinBox::update);

        update();
    }
}

void CustomSpinBox::update()
{
    if (mSetting != nullptr)
    {
        blockSignals(true);
        setValue(mSetting->getData().value);
        blockSignals(false);

        emit signalStateChanged();
    }
}

CustomSlider::CustomSlider(QWidget* parent) : QSlider(parent), mSetting(nullptr)
{}

void CustomSlider::bindToSetting(CustomSlider::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setMinimum(mSetting->getData().minimum);
        setMaximum(mSetting->getData().maximum);
        setEnabled(!mSetting->isReadOnly());

        connect(this, &CustomSlider::valueChanged, this, [&]() {
            mSetting->setDataValue(value());
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomSlider::update);

        update();
    }
}

void CustomSlider::update()
{
    if (mSetting != nullptr)
    {
        blockSignals(true);
        setValue(mSetting->getData().value);
        blockSignals(false);
        emit signalStateChanged();
    }
}

CustomDoubleSpinBox::CustomDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent), mSetting(nullptr)
{}

void CustomDoubleSpinBox::bindToSetting(CustomDoubleSpinBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setMinimum(mSetting->getData().minimum);
        setMaximum(mSetting->getData().maximum);
        setDecimals(mSetting->getData().decimals);
        setSuffix(mSetting->getData().suffix);
        setValue(mSetting->getData().value);
        setReadOnly(mSetting->isReadOnly());
        if(mSetting->isReadOnly())
        {
            setButtonSymbols(ButtonSymbols::NoButtons);
        }

        connect(this, &CustomDoubleSpinBox::editingFinished, this, [&]() {
            mSetting->setDataValue(value());
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomDoubleSpinBox::update);

        update();
    }
}

void CustomDoubleSpinBox::update()
{
    if (mSetting != nullptr)
    {
        blockSignals(true);
        setValue(mSetting->getData().value);
        blockSignals(false);

        emit signalStateChanged();
    }
}

CustomCheckBox::CustomCheckBox(QWidget* parent) : QCheckBox(parent), mSetting(nullptr)
{}

void CustomCheckBox::bindToSetting(CustomCheckBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setEnabled(!mSetting->isReadOnly());

        connect(this, &CustomCheckBox::stateChanged, this, [&]() {
            mSetting->setDataValue(isChecked());
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomCheckBox::update);
        update();
    }
}

void CustomCheckBox::update()
{
    if (mSetting != nullptr)
    {
        blockSignals(true);
        setChecked(mSetting->getData().value);
        blockSignals(false);

        emit signalStateChanged();
    }
}

CustomLineEdit::CustomLineEdit(QWidget* parent) : QLineEdit(parent), mSetting(nullptr)
{
    connect(this, &QLineEdit::returnPressed, this, &QLineEdit::clearFocus);
}

void CustomLineEdit::bindToSetting(CustomLineEdit::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setReadOnly(mSetting->isReadOnly());

        const auto& regexString = mSetting->getData().regexValidatorString;
        if (!regexString.isEmpty())
        {
            auto validator = new QRegExpValidator(QRegExp(regexString));
            setValidator(validator);
        }

        connect(this, &CustomLineEdit::editingFinished, this, [&]() {
            mSetting->setDataValue(text());
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomLineEdit::update);
        update();
    }
}

void CustomLineEdit::update()
{
    if (mSetting != nullptr)
    {
        blockSignals(true);
        setText(mSetting->getData().value);
        blockSignals(false);

        emit signalStateChanged();
    }
}

CustomComboBox::CustomComboBox(QWidget* parent) : QComboBox(parent), mSetting(nullptr)
{}

void CustomComboBox::bindToSetting(CustomComboBox::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setEnabled(!mSetting->isReadOnly());

        connect(this, &CustomComboBox::currentTextChanged, this, [&]() {
            mSetting->setDataValue(currentText());
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomComboBox::update);
        update();
    }
}

void CustomComboBox::update()
{
    if (mSetting != nullptr)
    {
        blockSignals(true);
        clear();
        for (auto item : mSetting->getData().list)
        {
            addItem(item);
        }
        setCurrentText(mSetting->getData().value);
        blockSignals(false);

        emit signalStateChanged();
    }
}

CustomFontButton::CustomFontButton(QWidget* parent) : QPushButton(parent), mSetting(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void CustomFontButton::bindToSetting(CustomFontButton::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setEnabled(!mSetting->isReadOnly());

        connect(this, &CustomFontButton::clicked, this, [&]() {
            bool ok{false};
            QFont fnt{QFontDialog::getFont(&ok, mSetting->getData().value)};
            if (ok)
            {
                mSetting->setDataValue(fnt);
            }
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomFontButton::update);
        update();
    }
}

void CustomFontButton::update()
{
    if (mSetting != nullptr)
    {
        setFont(mSetting->getData().value);
        setText("Font");

        emit signalStateChanged();
    }
}

CustomColorButton::CustomColorButton(QWidget* parent) : QPushButton(parent), mSetting(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void CustomColorButton::bindToSetting(CustomColorButton::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setEnabled(!mSetting->isReadOnly());

        connect(this, &CustomColorButton::clicked, this, [&]() {
            QColor clr{QColorDialog::getColor(mSetting->getData().value)};
            if (clr.isValid())
            {
                mSetting->setDataValue(clr);
            }
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomColorButton::update);
        update();
    }
}

void CustomColorButton::update()
{
    if (mSetting != nullptr)
    {
        setStyleSheet(QString("background-color: %1;").arg(mSetting->getData().value.name()));
        emit signalStateChanged();
    }
}

CustomSourceButton::CustomSourceButton(QWidget* parent) : QPushButton(parent), mSetting(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void CustomSourceButton::bindToSetting(CustomSourceButton::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setEnabled(!mSetting->isReadOnly());

        connect(this, &CustomFontButton::clicked, this, [&]() {
            QString filename;

            switch (mSetting->getData().sourceType)
            {
                case DataSource::kDir:
                    filename =
                        QFileDialog::getExistingDirectory(this, tr("Select directory"), "~/",
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                    break;
                case DataSource::kFile:
                    filename = QFileDialog::getOpenFileName(this, tr("Select file"), "~/", mSetting->getData().filters);
                    break;
            }

            if (!filename.isEmpty())
            {
                mSetting->setDataValue(filename);
            }
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomSourceButton::update);
        update();
    }
}

void CustomSourceButton::update()
{
    if (mSetting != nullptr)
    {
        setText(mSetting->getData().value);

        emit signalStateChanged();
    }
}

CustomTextEdit::CustomTextEdit(QWidget* parent) : QTextEdit(parent), mSetting(nullptr)
{}

void CustomTextEdit::bindToSetting(CustomTextEdit::SettingType* setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;
        setReadOnly(mSetting->isReadOnly());

        connect(this, &CustomTextEdit::textChanged, this, [&]() {
            mSetting->setDataValue(toPlainText().split("\n"));
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomTextEdit::update);
        update();
    }
}

void CustomTextEdit::update()
{
    if (mSetting != nullptr)
    {
        blockSignals(true);
        auto cursorPosition = textCursor().position();
        clear();
        for (auto item : mSetting->getDataValue())
        {
            append(item);
        }
        auto cursor = textCursor();
        cursor.setPosition(cursorPosition);
        setTextCursor(cursor);
        blockSignals(false);

        emit signalStateChanged();
    }
}

CustomListBox::CustomListBox(QWidget* parent) : QComboBox(parent), mSetting(nullptr)
{}

void CustomListBox::bindToSetting(CustomListBox::SettingType* setting)
{
    const QString addItemText{"..."};
    const QString clearItemText{"Clear"};
    if (setting != nullptr)
    {
        mSetting = setting;
        setEnabled(!mSetting->isReadOnly());

        connect(this, &CustomComboBox::currentTextChanged, this, [=](const QString& current_text) {
            QString newCurrentText{current_text};
            QStringList list;

            if (current_text != clearItemText)
            {
                if (current_text == addItemText)
                {
                    switch (mSetting->getData().listType)
                    {
                        case DataChangeableList::kStringList:
                            newCurrentText =
                                QInputDialog::getText(this, tr("New item"), tr("Item:"), QLineEdit::Normal);
                            break;
                        case DataChangeableList::kFileList:
                            newCurrentText = QFileDialog::getOpenFileName(this, tr("Select file"), "~/",
                                                                          mSetting->getData().filters);
                            break;
                        case DataChangeableList::kDirList:
                            newCurrentText = QFileDialog::getExistingDirectory(
                                this, tr("Select directory"), "~/",
                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                            break;
                    }
                    if (mSetting->getDataValue().contains(newCurrentText) && mSetting->getData().isUniqItems)
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
                    if (text != current_text)
                    {
                        list << text;
                    }
                }
                update();
            }

            mSetting->setDataValue(list);
            emit signalEditingFinished();
        });
        connect(mSetting, &Setting::signalDataChanged, this, &CustomListBox::update);
        update();
    }
}

void CustomListBox::update()
{
    const QString addItemText{"..."};
    const QString clearItemText{"Clear"};
    if (mSetting != nullptr)
    {
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

        emit signalStateChanged();
    }
}

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent), mSetting(nullptr)
{}

void CustomLabel::bindToSetting(Setting *setting)
{
    if (setting != nullptr)
    {
        mSetting = setting;

        connect(mSetting, &Setting::signalDataChanged, this, &CustomLabel::update);
        update();
    }
}

void CustomLabel::update()
{
    auto boolSetting            = dynamic_cast<SettingBool*>(mSetting);
    auto intSetting             = dynamic_cast<SettingInt*>(mSetting);
    auto doubleSetting          = dynamic_cast<SettingDouble*>(mSetting);
    auto stringSetting          = dynamic_cast<SettingString*>(mSetting);
    auto listSetting            = dynamic_cast<SettingStringList*>(mSetting);
    auto fontSetting            = dynamic_cast<SettingFont*>(mSetting);
    auto colorSetting           = dynamic_cast<SettingColor*>(mSetting);
    auto sourceSetting          = dynamic_cast<SettingSource*>(mSetting);
    auto changeableListSetting  = dynamic_cast<SettingChangeableList*>(mSetting);
    auto editableListSetting    = dynamic_cast<SettingEditableList*>(mSetting);

    if(boolSetting)
    {
        setText(boolSetting->getData().value ? "Yes" : "No");
    }
    else if(intSetting)
    {
        setText(QString("%1%2").arg(intSetting->getData().value).arg(intSetting->getData().suffix));
    }
    else if(doubleSetting)
    {
        setText(QString("%1%2").arg(doubleSetting->getData().value).arg(doubleSetting->getData().suffix));
    }
    else if(stringSetting)
    {
        setText(stringSetting->getData().value);
    }
    else if(listSetting)
    {
        setText(listSetting->getData().value);
    }
    else if(sourceSetting)
    {
        setText(sourceSetting->getData().value);
    }
    else if(changeableListSetting)
    {
        setText(changeableListSetting->getData().value.at(0));
    }
    else if(fontSetting)
    {
        setFont(fontSetting->getData().value);
        setText("Font");
    }
    else if(colorSetting)
    {
        setStyleSheet(QString("background-color: %1;").arg(colorSetting->getData().value.name()));
    }
    else if(editableListSetting)
    {
        QString lst;
        auto count = editableListSetting->getData().value.size();
        for(const auto& value : editableListSetting->getData().value)
        {
            lst+=value + (count > 1 ? "\n" : "");
            count--;
        }
        setText(lst);
    }
}
