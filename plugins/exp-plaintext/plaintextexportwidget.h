#ifndef PLAINTEXTEXPORTWIDGET_H
#define PLAINTEXTEXPORTWIDGET_H

#include <ui_plaintextexportwidget.h>

#include "../common/exp-interface.h"
#include "../common/codecsmodel.h"

class PlaintextExportWidget : public QWidget
{
  Q_OBJECT

  public:
    PlaintextExportWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

    QString codec  () const;
    QString text   () const;
    void    refresh() const;

  private:
    CodecsModel               _codecsModel;
    Ui::PlaintextExportWidget _ui;

    virtual ~PlaintextExportWidget() override;

    void preselectCodec(const QString &pCodec) const;

  signals:
    void progressExportSetMax     (quintptr max)                                               const;
    void progressExportSetValue   (quintptr value)                                             const;
    void vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                  const;
    void vocabularyGetCategoryName(quintptr categoryId, QString *name)                         const;
    void vocabularyGetMarks       (QStringList *marks)                                         const;
    void vocabularyGetMarkText    (quintptr recordId, const QString &mark, QString *text)      const;
    void vocabularyGetRecordCount (quintptr categoryId, quintptr *count)                       const;
    void vocabularyGetRecordIds   (quintptr categoryId, ExpInterface::RecordIdList *recordIds) const;

  private slots:
    void on_refresh_clicked(bool checked = false) const;
};

#endif