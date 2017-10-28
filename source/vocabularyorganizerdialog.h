#ifndef VOCABULARYORGANIZERDIALOG_H
#define VOCABULARYORGANIZERDIALOG_H

#include <ui_vocabularyorganizerdialog.h>

#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

class VocabularyOrganizerDialog : public QDialog
{
  Q_OBJECT

  public:
             VocabularyOrganizerDialog(VocabularyOrganizer *organizer, QWidget *parent);
    virtual ~VocabularyOrganizerDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    Ui::VocabularyOrganizerDialog _ui;
    VocabularyOrganizer          *_organizer;
    VocabularyOrganizerModel      _model;

    virtual void    accept        () Q_DECL_OVERRIDE;
            QString openPath      () const;
    virtual void    reject        () Q_DECL_OVERRIDE;

  private Q_SLOTS:
    void on_close_clicked                              (bool checked = false);
    void on_new2_clicked                               (bool checked = false);
    void on_open_clicked                               (bool checked = false);
    void on_vocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif