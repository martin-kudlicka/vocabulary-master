#include "vocabularymanagerdialog/spinboxdelegate.h"

#include <QtGui/QSpinBox>
#include <QtGui/QApplication>

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

	QStyleOptionSpinBox qsosbSpinBox;
	qsosbSpinBox.rect = option.rect;
	qsosbSpinBox.state |= QStyle::State_Enabled;
	qsosbSpinBox.stepEnabled = QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;
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

/*void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	FieldLineEdit *qleEditor = qobject_cast<FieldLineEdit *>(editor);
	if (qleEditor->hasAcceptableInput()) {
		QStyledItemDelegate::setModelData(editor, model, index);
	} // if
} // setModelData*/