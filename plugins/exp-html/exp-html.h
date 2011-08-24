#ifndef EXP_HTML_H
#define EXP_HTML_H

#include "htmlexportwidget.h"

class ExpHtml : public ExpInterface
{
	Q_OBJECT
	Q_INTERFACES(ExpInterface)

	private:
        HtmlExportWidget *_hewWidget;

        virtual const bool BeginExport() const;
		virtual const QString GetPluginName() const;
		virtual const void SetupUI(QWidget *pParent);

    private slots:
        const void on_hewWidget_ProgressExportSetMax(const int &pMax) const;
        const void on_hewWidget_ProgressExportSetValue(const int &pValue) const;
        const void on_hewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const;
        const void on_hewWidget_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const;
        const void on_hewWidget_VocabularyGetMarks(QStringList *pMarks) const;
        const void on_hewWidget_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const;
        const void on_hewWidget_VocabularyGetRecordCount(const int &pCategoryId, int *pCount) const;
        const void on_hewWidget_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const;
}; // ExpHtml

#endif // EXP_HTML_H
