#ifndef PLAINTEXTEXPORTWIDGET_H
#define PLAINTEXTEXPORTWIDGET_H

#include <ui_plaintextexportwidget.h>

#include "../common/exp-interface.h"
#include "../common/codecsmodel.h"

class PlaintextExportWidget : public QWidget
{
	Q_OBJECT

	public:
		PlaintextExportWidget(QWidget *parent = NULL, Qt::WindowFlags flags = 0);

        const QString codec  () const;
        const QString text   () const;
        const void    refresh() const;

	private:
        CodecsModel               _codecsModel;
		Ui::PlaintextExportWidget _ui;

		virtual ~PlaintextExportWidget();

        const void preselectCodec(const QString &pCodec) const;

    signals:
        void progressExportSetMax     (const quint32 &max)                                              const;
        void progressExportSetValue   (const quint32 &value)                                            const;
        void vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                       const;
        void vocabularyGetCategoryName(const quint8 &categoryId, QString *name)                         const;
        void vocabularyGetMarks       (QStringList *marks)                                              const;
        void vocabularyGetMarkText    (const quint32 &recordId, const QString &mark, QString *text)     const;
        void vocabularyGetRecordCount (const quint8 &categoryId, quint32 *count)                        const;
        void vocabularyGetRecordIds   (const quint8 &categoryId, ExpInterface::RecordIdList *recordIds) const;

    private slots:
        const void on_refresh_clicked(bool checked = false) const;
}; // PlaintextExportWidget

#endif // PLAINTEXTEXPORTWIDGET_H