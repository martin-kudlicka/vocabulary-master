#include "vocabularymanagerdialog/vocabularytabwidget.h"

#ifndef FREE
# include <QtGui/QCheckBox>

int VocabularyTabWidget::addTab(QWidget *page, const QString &label, const bool &pEnabled)
{
    int iTab = QTabWidget::addTab(page, label);

    QCheckBox *qcbEnabled = new QCheckBox(this);
    qcbEnabled->setCheckState(pEnabled ? Qt::Checked : Qt::Unchecked);
    tabBar()->setTabButton(iTab, POSITION_BUTTON_ENABLED, qcbEnabled);
    connect(qcbEnabled, SIGNAL(stateChanged(int)), SLOT(on_qcbEnabled_stateChanged(int)));

    return iTab;
} // addTab

const void VocabularyTabWidget::on_qcbEnabled_stateChanged(int state) const
{
    for (int iI = 0; iI < count(); iI++) {
        if (sender() == tabBar()->tabButton(iI, POSITION_BUTTON_ENABLED)) {
            emit TabEnableChanged(iI, static_cast<Qt::CheckState>(state));
            return;
        } // if
    } // for
} // on_qcbEnabled_stateChanged
#endif

VocabularyTabWidget::VocabularyTabWidget(QWidget *pParent /* NULL */) : QTabWidget(pParent)
{
} // VocabularyTabWidget