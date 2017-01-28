#ifndef VOCABULARYMANAGERDIALOG_H
#define VOCABULARYMANAGERDIALOG_H

#include <ui_vocabularymanagerdialog.h>

class Vocabulary;
class Settings;
class Plugins;
class VocabularyView;

class VocabularyManagerDialog : public QDialog
{
  Q_OBJECT

  public:
             VocabularyManagerDialog(const QSharedPointer<Vocabulary> &vocabulary, const Settings *settings, const Plugins *plugins, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~VocabularyManagerDialog() Q_DECL_OVERRIDE;

    void execOnRecord(quintptr recordId);

  private:
    enum class EditorColumn
    {
      LeftLabel,
      LeftControl,
      RightLabel,
      RightControl
    };

    const Plugins                    *_plugins;
          QList<quintptr>             _categories;
    const Settings                   *_settings;
          Ui::VocabularyManagerDialog _ui;
    const QSharedPointer<Vocabulary>  _vocabulary;

    void addTab               (quintptr categoryId);
    void enableTabControls    ()                                const;
    void enableWordControls   ()                                const;
    void focusOnRecord        (quintptr recordId)               const;
    void hideColumns          ()                                const;
    void hideColumns          (VocabularyView *tableView)       const;
    void initEditor           ();
    void initTabs             ();
    void reassignModels       ()                                const;
    void selectFirstEnabledTab();
    void setPriorityDelegate  ();
    void setPriorityDelegate  (VocabularyView *tableView);
    void stretchColumns       ()                                const;
    void stretchColumns       (const VocabularyView *tableView) const;
    void uninitEditor         ()                                const;
    void updateEditor         ()                                const;
    void updateEditor         (EditorColumn controlsColumn)     const;

  private Q_SLOTS:
    void on_control_textEdited                           (const QString &text)                                              const;
    void on_categoryAdd_clicked                          (bool checked = false);
    void on_categoryRemove_clicked                       (bool checked = false);
    void on_searchButton_clicked                         (bool checked = false)                                             const;
    void on_searchEdit_textChanged                       (const QString &text)                                              const;
    void on_tabs_currentChanged                          (int index)                                                        const;
    void on_tabs_tabEnableChanged                        (quintptr index, Qt::CheckState state)                             const;
    void on_tabs_tabPriorityChanged                      (quintptr index, quintptr value)                                   const;
    void on_vocabularySettings_clicked                   (bool checked = false);
    void on_vocabularyViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
    void on_wordAdd_clicked                              (bool checked = false);
    void on_wordCopyMove_clicked                         (bool checked = false);
    void on_wordExport_clicked                           (bool checked = false);
    void on_wordImport_clicked                           (bool checked = false);
    void on_wordRemove_clicked                           (bool checked = false);
};

#endif