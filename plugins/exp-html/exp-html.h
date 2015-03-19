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

		virtual ~ExpHtml() override;

        virtual void    beginExport() const          override;
		virtual QString pluginName () const          override;
		virtual void    setupUi    (QWidget *parent) override;

    private slots:
        void on_widget_progressExportSetMax     (quint32 max)                                              const;
        void on_widget_progressExportSetValue   (quint32 value)                                            const;
        void on_widget_vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                const;
        void on_widget_vocabularyGetCategoryName(quint8 categoryId, QString *name)                         const;
        void on_widget_vocabularyGetMarks       (QStringList *marks)                                       const;
        void on_widget_vocabularyGetMarkText    (quint32 recordId, const QString &mark, QString *text)     const;
        void on_widget_vocabularyGetRecordCount (quint8 categoryId, quint32 *count)                        const;
        void on_widget_vocabularyGetRecordIds   (quint8 categoryId, ExpInterface::RecordIdList *recordIds) const;
}; // ExpHtml

#endif // EXP_HTML_H