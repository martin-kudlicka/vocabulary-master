#ifndef EXP_HTML_H
#define EXP_HTML_H

#include "htmlexportwidget.h"

class ExpHtml : public ExpInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IID_EXPINTERFACE FILE "exp-html.json")
	Q_INTERFACES(ExpInterface)

	private:
        HtmlExportWidget *_widget;

		virtual ~ExpHtml();

        virtual const void    beginExport() const;
		virtual const QString pluginName () const;
		virtual const void    setupUi    (QWidget *parent);

    private slots:
        const void on_widget_progressExportSetMax     (const quint32 &max)                                              const;
        const void on_widget_progressExportSetValue   (const quint32 &value)                                            const;
        const void on_widget_vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                       const;
        const void on_widget_vocabularyGetCategoryName(const quint8 &categoryId, QString *name)                         const;
        const void on_widget_vocabularyGetMarks       (QStringList *marks)                                              const;
        const void on_widget_vocabularyGetMarkText    (const quint32 &recordId, const QString &mark, QString *text)     const;
        const void on_widget_vocabularyGetRecordCount (const quint8 &categoryId, quint32 *count)                        const;
        const void on_widget_vocabularyGetRecordIds   (const quint8 &categoryId, ExpInterface::RecordIdList *recordIds) const;
}; // ExpHtml

#endif // EXP_HTML_H