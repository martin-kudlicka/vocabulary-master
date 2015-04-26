#include "vocabularymanagerdialog/prioritydelegate.h"

#include <QtWidgets/QSpinBox>
#include <QtWidgets/QApplication>
#include "vocabularymanagerdialog/vocabularyview.h"

PriorityDelegate::PriorityDelegate(QObject *pParent /* nullptr */) : QStyledItemDelegate(pParent)
{
} // PriorityDelegate

PriorityDelegate::~PriorityDelegate()
{
} // ~PriorityDelegate

QWidget *PriorityDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSpinBox *editor = new QSpinBox(parent);
	editor->setFrame(false);
	editor->setMinimum(RECORD_PRIORITY_MIN);
	editor->setMaximum(RECORD_PRIORITY_MAX);

	return editor;
} // createEditor

void PriorityDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);

    // general spinbox style
	QStyleOptionSpinBox styleOptionSpinBox;
	styleOptionSpinBox.rect        = option.rect;
	styleOptionSpinBox.state      |= QStyle::State_Enabled;
	styleOptionSpinBox.stepEnabled = QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;

    // check if mouse cursor is over spin button
    QRect spinButtonRect = option.rect;
    spinButtonRect.setLeft(spinButtonRect.right() - SPINBOX_BUTTONS_WIDTH);
    const VocabularyView *vocabularyView = qobject_cast<VocabularyView *>(index.model()->parent());
    const QPoint cursor                  = vocabularyView->viewport()->mapFromGlobal(QCursor::pos());
    if (spinButtonRect.contains(cursor))
	{
        styleOptionSpinBox.state |= QStyle::State_MouseOver;

        const quint8 spinButtonUpHeight = spinButtonRect.height() / 2;
        if (cursor.y() < spinButtonRect.top() + spinButtonUpHeight)
		{
            styleOptionSpinBox.activeSubControls |= QStyle::SC_SpinBoxUp;
        }
		else
		{
            styleOptionSpinBox.activeSubControls |= QStyle::SC_SpinBoxDown;
        } // if else
    } // if

	QApplication::style()->drawComplexControl(QStyle::CC_SpinBox, &styleOptionSpinBox, painter);
} // paint

void PriorityDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QSpinBox *spinBoxEditor = qobject_cast<QSpinBox *>(editor);
	spinBoxEditor->setValue(index.model()->data(index).toUInt());
} // setEditorData