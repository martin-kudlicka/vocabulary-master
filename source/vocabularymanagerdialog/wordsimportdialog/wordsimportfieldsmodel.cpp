#include "vocabularymanagerdialog/wordsimportdialog/wordsimportfieldsmodel.h"

#include "vocabularymanagerdialog/prioritydelegate.h"

WordsImportFieldsModel::WordsImportFieldsModel(const Vocabulary *pVocabulary, QObject *pParent /* NULL */) : QAbstractItemModel(pParent)
{
    _vVocabulary = pVocabulary;

	for (int iI = 0; iI < _vVocabulary->fieldCount(); iI++) {
		_qslEditorData.append(QString());
	} // for
} // WordsImportFieldsModel

int WordsImportFieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant WordsImportFieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	switch (index.column()) {
		case ColumnLanguage:
			switch (role) {
				case Qt::DisplayRole:
					{
						int iFieldId = _vVocabulary->fieldId(index.row());
						VocabularyDatabase::FieldLanguage eflLanguage = _vVocabulary->fieldLanguage(iFieldId);
						return _vVocabulary->GetLanguageName(eflLanguage);
					}
				default:
					return QVariant();
			} // switch
		case ColumnName:
			switch (role) {
				case Qt::DisplayRole:
					{
						int iFieldId = _vVocabulary->fieldId(index.row());
						return _vVocabulary->fieldName(iFieldId);
					}
				default:
					return QVariant();
			} // switch
		case ColumnEditor:
			{
				int iFieldId = _vVocabulary->fieldId(index.row());

				if (_vVocabulary->fieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
					VocabularyDatabase::FieldBuiltIn efbBuiltIn = _vVocabulary->fieldBuiltIn(iFieldId);
					switch (efbBuiltIn) {
						case VocabularyDatabase::FieldBuiltInEnabled:
							switch (role) {
								case Qt::EditRole:
								case Qt::CheckStateRole:
									if (_qslEditorData.at(index.row()).isEmpty()) {
										return Qt::Checked;
									} else {
										return _qslEditorData.at(index.row()).toInt();
									} // if else
								default:
									return QVariant();
							} // switch
						case VocabularyDatabase::FieldBuiltInPriority:
							switch (role) {
								case Qt::EditRole:
									if (_qslEditorData.at(index.row()).isEmpty()) {
										return PriorityDelegate::RECORD_PRIORITY_MIN;
									} else {
										return _qslEditorData.at(index.row()).toInt();
									} // if else
								default:
									return QVariant();
							} // switch
					} // switch
				} else {
					switch (role) {
						case Qt::EditRole:
							return _qslEditorData.at(index.row());
						default:
							return QVariant();
					} // switch
				} // if else
			}
		default:
			return QVariant();
	} // switch
} // data

Qt::ItemFlags WordsImportFieldsModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags ifFlags = QAbstractItemModel::flags(index);

	if (index.column() == ColumnEditor) {
		int iFieldId = _vVocabulary->fieldId(index.row());
		VocabularyDatabase::FieldType eftType = _vVocabulary->fieldType(iFieldId);
		if (eftType == VocabularyDatabase::FieldTypeCheckBox) {
			ifFlags |= Qt::ItemIsUserCheckable;
		} // if
	} // if

	return ifFlags;
} // flags

QVariant WordsImportFieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
				case ColumnLanguage:
					return tr("Language");
                case ColumnName:
                    return tr("Name");
                case ColumnEditor:
                    return tr("Pattern");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex WordsImportFieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex WordsImportFieldsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int WordsImportFieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        return _vVocabulary->fieldCount();
    } else {
        return 0;
    } // if else
} // rowCount

bool WordsImportFieldsModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
	_qslEditorData[index.row()] = value.toString();

	emit dataChanged(index, index);
	return true;
} // setData