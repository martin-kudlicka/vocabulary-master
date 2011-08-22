#ifndef PLAINTEXTEXPORTWIDGET_H
#define PLAINTEXTEXPORTWIDGET_H

#include <ui_plaintextexportwidget.h>

#include "../common/exp-interface.h"

class PlaintextExportWidget : public QWidget
{
	Q_OBJECT

	public:
		PlaintextExportWidget(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Ui::qwPlaintextExport _qwpePlaintextExport;

    signals:
        void VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const;
        void VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const;
        void VocabularyGetMarks(QStringList *pMarks) const;
        void VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const;

    private slots:
        const void on_qpbPlainRefresh_clicked(bool checked = false) const;
}; // PlaintextExportWidget

#endif // PLAINTEXTEXPORTWIDGET_H