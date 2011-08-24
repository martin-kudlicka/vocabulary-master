#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate.h"

#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldidentifiervalidator.h"

QWidget *LineEditPersistentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	FieldLineEdit *fleEditor = new FieldLineEdit(parent);
	if (index.column() == FieldsModel::ColumnTemplateName) {
		fleEditor->setValidator(new FieldIdentifierValidator(index.row(), qobject_cast<const FieldsModel *>(index.model()), fleEditor));
	} // if

	return fleEditor;
} // createEditor

LineEditPersistentDelegate::LineEditPersistentDelegate(QObject *pParent /* NULL */) : QStyledItemDelegate(pParent)
{
} // LineEditPersistentDelegate

void LineEditPersistentDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    FieldLineEdit *qleEditor = qobject_cast<FieldLineEdit *>(editor);
    qleEditor->setText(index.model()->data(index).toString());
} // setEditorData

void LineEditPersistentDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	FieldLineEdit *qleEditor = qobject_cast<FieldLineEdit *>(editor);
	if (qleEditor->hasAcceptableInput()) {
		QStyledItemDelegate::setModelData(editor, model, index);
	} // if
} // setModelData