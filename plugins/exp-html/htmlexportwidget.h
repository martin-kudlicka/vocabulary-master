#ifndef HTMLEXPORTWIDGET_H
#define HTMLEXPORTWIDGET_H

#include <ui_htmlexportwidget.h>

#include "../common/exp-interface.h"
#include "../common/codecsmodel.h"

class HtmlExportWidget : public QWidget
{
  Q_OBJECT

  public:
    HtmlExportWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

    QString codec  () const;
    void    refresh() const;
    QString text   () const;

  private:
    enum class Style
    {
      Text,
      Table
    };
    enum class TableRow
    {
      Header,
      Template
    };

    struct TableColumn
    {
      QWidget   *headerWidget;
      QLineEdit *headerEdit;
      QSpinBox  *width;
      QLineEdit *templateEdit;
    };

    CodecsModel          _codecsModel;
    QList<TableColumn>   _tableColumns;
    Ui::HtmlExportWidget _ui;

    virtual ~HtmlExportWidget() override;

    void addTableColumn   ();
    void initTableColumns ();
    void insertTableText  (const QTextTable *tablePreview, quintptr row, quintptr column, const QString &text) const;
    void preselectCodec   (const QString &codec)                                                               const;
    void refreshTable     ()                                                                                   const;
    void refreshText      ()                                                                                   const;
    void removeTableColumn();

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
    void on_styleTable_clicked      (bool checked = false) const;
    void on_styleText_clicked       (bool checked = false) const;
    void on_tableColums_valueChanged(int i);
    void on_tableRefresh_clicked    (bool checked = false) const;
    void on_textRefresh_clicked     (bool checked = false) const;
};

#endif