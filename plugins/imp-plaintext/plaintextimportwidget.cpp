#include "plaintextimportwidget.h"

PlaintextImportWidget::PlaintextImportWidget(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags)
{
    _qwpiPlaintextImport.setupUi(this);

    _qwpiPlaintextImport.qtvCodecs->setModel(&_cmCodecsModel);
} // PlaintextImportWidget