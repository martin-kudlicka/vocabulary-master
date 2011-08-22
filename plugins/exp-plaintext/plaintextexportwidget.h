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
        CodecsModel _cmCodecsModel;
		Ui::qwPlaintextExport _qwpePlaintextExport;

        const void PreselectCodec(const QString &pCodec) const;

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
}; // PlaintextExportWidget

#endif // PLAINTEXTEXPORTWIDGET_H