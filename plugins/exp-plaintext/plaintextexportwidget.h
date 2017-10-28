#ifndef PLAINTEXTEXPORTWIDGET_H
#define PLAINTEXTEXPORTWIDGET_H

#include <ui_plaintextexportwidget.h>

#include "../common/exp-interface.h"
#include "../common/codecsmodel.h"

class PlaintextExportWidget : public QWidget
{
  Q_OBJECT

  public:
    PlaintextExportWidget(QWidget *parent);

    QString codec  () const;
    QString text   () const;
    void    refresh() const;

  private:
    CodecsModel               _codecsModel;
    Ui::PlaintextExportWidget _ui;

    virtual ~PlaintextExportWidget() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    void preselectCodec(const QString &pCodec) const;

  Q_SIGNALS:
    void progressExportSetMax     (quintptr max)                                               const;
    void progressExportSetValue   (quintptr value)                                             const;
    void vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                  const;
    void vocabularyGetCategoryName(quintptr categoryId, QString *name)                         const;
    void vocabularyGetMarks       (QStringList *marks)                                         const;
    void vocabularyGetMarkText    (quintptr recordId, const QString &mark, QString *text)      const;
    void vocabularyGetRecordCount (quintptr categoryId, quintptr *count)                       const;
    void vocabularyGetRecordIds   (quintptr categoryId, ExpInterface::RecordIdList *recordIds) const;

  private Q_SLOTS:
    void on_refresh_clicked(bool checked = false) const;
};

#endif