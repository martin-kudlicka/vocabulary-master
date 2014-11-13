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
		         WordsExportDialog(const Vocabulary *vocabulary, const Plugins::ExpPluginList &expPlugins, QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~WordsExportDialog();

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
        const void on_plugin_ProgressExportSetMax              (quint32 max)                                              const;
		const void on_plugin_ProgressExportSetValue            (quint32 value)                                            const;
        const void on_plugin_VocabularyGetCategoryIds          (ExpInterface::CategoryIdList *categoryIds)                const;
		const void on_plugin_VocabularyGetCategoryName         (quint8 categoryId, QString *name)                         const;
        const void on_plugin_VocabularyGetMarks                (QStringList *marks)                                       const;
		const void on_plugin_VocabularyGetMarkText             (quint32 recordId, const QString &mark, QString *text)     const;
		const void on_plugin_VocabularyGetRecordCount          (quint8 categoryId, quint32 *count)                        const;
		const void on_plugin_VocabularyGetRecordIds            (quint8 categoryId, ExpInterface::RecordIdList *recordIds) const;
}; // WordsExportDialog

#endif // WORDSEXPORTDIALOG_H