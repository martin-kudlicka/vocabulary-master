#ifndef VOCABULARYOPENPROGRESSDIALOG_H
#define VOCABULARYOPENPROGRESSDIALOG_H

#include <ui_vocabularyopenprogressdialog.h>

#include <QtWidgets/QDialog>
#include "vocabulary.h"

class VocabularyOpenProgressDialog : public QDialog
{
  Q_OBJECT

  public:
             VocabularyOpenProgressDialog(const Vocabulary *vocabulary, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~VocabularyOpenProgressDialog() Q_DECL_OVERRIDE;

  private:
    Ui::VocabularyOpenProgressDialog _ui;

    private Q_SLOTS:
    void on_vocabulary_setOpenProgressValue(quintptr value)        const;
    void on_vocabulary_setOpenProgressMax  (quintptr max)          const;
    void on_vocabulary_setVocabularyName   (const QString &name)   const;
};

#endif