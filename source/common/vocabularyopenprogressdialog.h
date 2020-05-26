#ifndef VOCABULARYOPENPROGRESSDIALOG_H
#define VOCABULARYOPENPROGRESSDIALOG_H

#include <ui_vocabularyopenprogressdialog.h>

class Vocabulary;

class VocabularyOpenProgressDialog : public QDialog
{
  Q_OBJECT

  public:
             VocabularyOpenProgressDialog(const Vocabulary *vocabulary, QWidget *parent);
    virtual ~VocabularyOpenProgressDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    Ui::VocabularyOpenProgressDialog _ui;

  private Q_SLOTS:
    void on_vocabulary_setOpenProgressValue(quintptr value)        const;
    void on_vocabulary_setOpenProgressMax  (quintptr max)          const;
    void on_vocabulary_setVocabularyName   (const QString &name)   const;
};

#endif