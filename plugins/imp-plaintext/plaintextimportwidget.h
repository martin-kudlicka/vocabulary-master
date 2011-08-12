#ifndef PLAINTEXTIMPORTWIDGET_H
#define PLAINTEXTIMPORTWIDGET_H

#include <ui_plaintextimportwidget.h>

#include "plaintextimportwidget/codecsmodel.h"

class PlaintextImportWidget : public QWidget
{
    Q_OBJECT

    public:
        PlaintextImportWidget(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
        CodecsModel _cmCodecsModel;
        Ui::qwPlaintextImport _qwpiPlaintextImport;
}; // PlaintextImportWidget

#endif // PLAINTEXTIMPORTWIDGET_H