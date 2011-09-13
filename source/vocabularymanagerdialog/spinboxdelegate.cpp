#include "vocabularymanagerdialog/spinboxdelegate.h"

#include <QtGui/QSpinBox>
#include <QtGui/QApplication>
#include "vocabularymanagerdialog/vocabularyview.h"

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSpinBox *qsbEditor = new QSpinBox(parent);
	qsbEditor->setFrame(false);
	qsbEditor->setMinimum(RECORD_PRIORITY_MIN);
	qsbEditor->setMaximum(RECORD_PRIORITY_MAX);

	return qsbEditor;
} // createEditor

void SpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);

    // general spinbox style
	QStyleOptionSpinBox qsosbSpinBox;
	qsosbSpinBox.rect = option.rect;
	qsosbSpinBox.state |= QStyle::State_Enabled;
	qsosbSpinBox.stepEnabled = QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;

    // check if mouse cursor is over spin button
    QRect qrSpinButtons = option.rect;
    qrSpinButtons.setLeft(qrSpinButtons.right() - SPINBOX_BUTTONS_WIDTH);
    const VocabularyView *vvView = qobject_cast<VocabularyView *>(index.model()->parent());
    QPoint qpCursor = vvView->viewport()->mapFromGlobal(QCursor::pos());
    if (qrSpinButtons.contains(qpCursor)) {
        qsosbSpinBox.state |= QStyle::State_MouseOver;

        int iSpinButtonUpHeight = qrSpinButtons.height() / 2;
        if (qpCursor.y() < qrSpinButtons.top() + iSpinButtonUpHeight) {
            qsosbSpinBox.activeSubControls |= QStyle::SC_SpinBoxUp;
        } else {
            qsosbSpinBox.activeSubControls |= QStyle::SC_SpinBoxDown;
        } // if else
    } // if

	QApplication::style()->drawComplexControl(QStyle::CC_SpinBox, &qsosbSpinBox, painter);
} // paint

SpinBoxDelegate::SpinBoxDelegate(QObject *pParent /* NULL */) : QStyledItemDelegate(pParent)
{
} // SpinBoxDelegate

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QSpinBox *qsbEditor = qobject_cast<QSpinBox *>(editor);
	qsbEditor->setValue(index.model()->data(index).toInt());
} // setEditorData