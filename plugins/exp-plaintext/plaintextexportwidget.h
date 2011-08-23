#ifndef PLAINTEXTEXPORTWIDGET_H
#define PLAINTEXTEXPORTWIDGET_H

#include <ui_plaintextexportwidget.h>

#include "../common/exp-interface.h"
#include "../common/codecsmodel.h"

class PlaintextExportWidget : public QWidget
{
	Q_OBJECT

	public:
		PlaintextExportWidget(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

        const QString GetCodec() const;
        const QString GetText() const;
        const void Refresh() const;

	private:
		enum eStyle {
			StylePlain,
			StyleTable
		}; // eStyle
        enum eTableRow {
            TableRowHeader,
            TableRowTemplate
        }; // eTableRow

        struct sTableColumn {
            QLineEdit *qleHeader;
            QLineEdit *qleTemplate;
        }; // sTableColumn

        CodecsModel _cmCodecsModel;
        QList<sTableColumn> _qlTableColumns;
		Ui::qwPlaintextExport _qwpePlaintextExport;

        const void AddTableColumn();
        const void InitTableColumns();
        const void PreselectCodec(const QString &pCodec) const;
        const void RemoveTableColumn();

    signals:
        void ProgressExportSetMax(const int &pMax) const;
        void ProgressExportSetValue(const int &pValue) const;
        void VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const;
        void VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const;
        void VocabularyGetMarks(QStringList *pMarks) const;
        void VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const;
        void VocabularyGetRecordCount(const int &pCategoryId, int *pCount) const;
        void VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const;

    private slots:
        const void on_qpbPlainRefresh_clicked(bool checked = false) const;
		const void on_qrbStylePlain_clicked(bool checked = false) const;
		const void on_qrbStyleTable_clicked(bool checked = false) const;
        const void on_qsbTableColums_valueChanged(int i);
}; // PlaintextExportWidget

#endif // PLAINTEXTEXPORTWIDGET_H