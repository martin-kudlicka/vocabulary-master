#ifndef WORDSEXPORTDIALOG_H
#define WORDSEXPORTDIALOG_H

#include <ui_wordsexportdialog.h>

#include "common/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsexportdialog/wordsexportfieldsmodel.h"
#include "vocabularymanagerdialog/wordsexportdialog/exppluginsmodel.h"
#include "../../common/marklineeditpersistentdelegate.h"

class WordsExportDialog : public QDialog
{
	Q_OBJECT

	public:
		         WordsExportDialog(const Vocabulary *vocabulary, const Plugins::ExpPluginList &expPlugins, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
		virtual ~WordsExportDialog() override;

	private:
		CategoriesModel                _categoriesModel;
		ExpPluginsModel                _expPluginsModel;
        MarkLineEditPersistentDelegate _markDelegate;
		Plugins::ExpPluginList         _expPlugins;
		Ui::WordsExportDialog          _ui;
		QHash<quint8, quint8>          _expPluginPage;
		const Vocabulary              *_vocabulary;
		WordsExportFieldsModel         _fieldsModel;

	private slots:
	    const void on_exportButton_clicked                     (bool checked = false);
		const void on_expPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
        const void on_plugin_ProgressExportSetMax              (quintptr max)                                              const;
		const void on_plugin_ProgressExportSetValue            (quintptr value)                                            const;
        const void on_plugin_VocabularyGetCategoryIds          (ExpInterface::CategoryIdList *categoryIds)                const;
		const void on_plugin_VocabularyGetCategoryName         (quintptr categoryId, QString *name)                         const;
        const void on_plugin_VocabularyGetMarks                (QStringList *marks)                                       const;
		const void on_plugin_VocabularyGetMarkText             (quintptr recordId, const QString &mark, QString *text)     const;
		const void on_plugin_VocabularyGetRecordCount          (quintptr categoryId, quintptr *count)                        const;
		const void on_plugin_VocabularyGetRecordIds            (quintptr categoryId, ExpInterface::RecordIdList *recordIds) const;
}; // WordsExportDialog

#endif // WORDSEXPORTDIALOG_H