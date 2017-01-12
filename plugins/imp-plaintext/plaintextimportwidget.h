#ifndef PLAINTEXTIMPORTWIDGET_H
#define PLAINTEXTIMPORTWIDGET_H

#include <ui_plaintextimportwidget.h>

#include "../common/codecsmodel.h"

class PlaintextFile;

class PlaintextImportWidget : public QWidget
{
  Q_OBJECT

  public:
    PlaintextImportWidget(PlaintextFile *file, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);

    quintptr lineCount     () const;
    quintptr linesPerRecord() const;
    QString  regExp        () const;

  private:
    CodecsModel               _codecsModel;
    PlaintextFile            *_file;
    Ui::PlaintextImportWidget _ui;

    virtual ~PlaintextImportWidget() Q_DECL_OVERRIDE;

    void preselectCodec() const;
    void refreshPreview() const;

  private Q_SLOTS:
    void on_codecs_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
    void on_linesPerRecord_valueChanged           (int i)                                                            const;
};

#endif