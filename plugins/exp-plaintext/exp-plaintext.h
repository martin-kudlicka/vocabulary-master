#ifndef EXP_PLAINTEXT_H
#define EXP_PLAINTEXT_H

#include "../common/exp-interface.h"

class PlaintextExportWidget;

class ExpPlaintext : public ExpInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IID_EXPINTERFACE FILE "exp-plaintext.json")
  Q_INTERFACES(ExpInterface)

  private:
    PlaintextExportWidget *_widget;

    virtual ~ExpPlaintext() override;

    virtual void    beginExport() const          override;
    virtual QString pluginName () const          override;
    virtual void    setupUi    (QWidget *parent) override;

  private slots:
    void on_widget_progressExportSetMax     (quintptr max)                                               const;
    void on_widget_progressExportSetValue   (quintptr value)                                             const;
    void on_widget_vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                  const;
    void on_widget_vocabularyGetCategoryName(quintptr categoryId, QString *name)                         const;
    void on_widget_vocabularyGetMarks       (QStringList *marks)                                         const;
    void on_widget_vocabularyGetMarkText    (quintptr recordId, const QString &mark, QString *text)      const;
    void on_widget_vocabularyGetRecordCount (quintptr categoryId, quintptr *count)                       const;
    void on_widget_vocabularyGetRecordIds   (quintptr categoryId, ExpInterface::RecordIdList *recordIds) const;
};

#endif