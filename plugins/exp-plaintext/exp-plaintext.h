#ifndef EXP_PLAINTEXT_H
#define EXP_PLAINTEXT_H

#include "plaintextexportwidget.h"

class ExpPlaintext : public ExpInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IID_EXPINTERFACE FILE "exp-plaintext.json")
	Q_INTERFACES(ExpInterface)

	private:
        PlaintextExportWidget *_widget;

		virtual ~ExpPlaintext();

        virtual void    beginExport() const;
		virtual QString pluginName () const;
		virtual void    setupUi    (QWidget *parent);

    private slots:
        void on_widget_progressExportSetMax     (quint32 max)                                              const;
        void on_widget_progressExportSetValue   (quint32 value)                                            const;
        void on_widget_vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                const;
        void on_widget_vocabularyGetCategoryName(quint8 categoryId, QString *name)                         const;
        void on_widget_vocabularyGetMarks       (QStringList *marks)                                       const;
        void on_widget_vocabularyGetMarkText    (quint32 recordId, const QString &mark, QString *text)     const;
        void on_widget_vocabularyGetRecordCount (quint8 categoryId, quint32 *count)                        const;
        void on_widget_vocabularyGetRecordIds   (quint8 categoryId, ExpInterface::RecordIdList *recordIds) const;
}; // ExpPlaintext

#endif // EXP_PLAINTEXT_H
