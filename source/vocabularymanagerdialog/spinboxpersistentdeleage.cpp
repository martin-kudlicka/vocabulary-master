#include "vocabularymanagerdialog/spinboxpersistentdeleage.h"

#include <QtGui/QSpinBox>

QWidget *SpinBoxPersistentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSpinBox *qsbEditor = new QSpinBox(parent);
	qsbEditor->setMinimum(RECORD_PRIORITY_MIN);
	qsbEditor->setMaximum(RECORD_PRIORITY_MAX);

	return qsbEditor;
} // createEditor

SpinBoxPersistentDelegate::SpinBoxPersistentDelegate(QObject *pParent /* NULL */) : QStyledItemDelegate(pParent)
{
} // SpinBoxPersistentDelegate

void SpinBoxPersistentDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QSpinBox *qsbEditor = qobject_cast<QSpinBox *>(editor);
	qsbEditor->setValue(index.model()->data(index).toInt());
} // setEditorData

/*void SpinBoxPersistentDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	FieldLineEdit *qleEditor = qobject_cast<FieldLineEdit *>(editor);
	if (qleEditor->hasAcceptableInput()) {
		QStyledItemDelegate::setModelData(editor, model, index);
	} // if
} // setModelData*/