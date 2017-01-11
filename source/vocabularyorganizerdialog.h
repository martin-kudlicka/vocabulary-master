#ifndef VOCABULARYORGANIZERDIALOG_H
#define VOCABULARYORGANIZERDIALOG_H

#include <ui_vocabularyorganizerdialog.h>

#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

class VocabularyOrganizerDialog : public QDialog
{
  Q_OBJECT

  public:
             VocabularyOrganizerDialog(VocabularyOrganizer *organizer, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~VocabularyOrganizerDialog() Q_DECL_OVERRIDE;

  private:
    Ui::VocabularyOrganizerDialog _ui;
    VocabularyOrganizer          *_organizer;
    VocabularyOrganizerModel      _model;

#ifndef EDITION_TRY
    virtual void    accept        () Q_DECL_OVERRIDE;
#endif
#ifdef EDITION_FREE
            void    enableControls() const;
#endif
#ifndef EDITION_TRY
            QString openPath      () const;
    virtual void    reject        () Q_DECL_OVERRIDE;
#endif

    private Q_SLOTS:
      void on_close_clicked                              (bool checked = false);
      void on_new2_clicked                               (bool checked = false);
#ifndef EDITION_TRY
      void on_open_clicked                               (bool checked = false);
#endif
      void on_vocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif