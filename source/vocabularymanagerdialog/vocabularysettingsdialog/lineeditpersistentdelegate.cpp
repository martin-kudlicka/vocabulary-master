#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate.h"

#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldidentifiervalidator.h"

QWidget *LineEditPersistentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	FieldLineEdit *fleEditor = new FieldLineEdit(parent);
	fleEditor->setValidator(new FieldIdentifierValidator(index.row(), qobject_cast<const FieldsModel *>(index.model()), fleEditor));

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