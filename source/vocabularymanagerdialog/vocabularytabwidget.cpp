#include "vocabularymanagerdialog/vocabularytabwidget.h"

#ifndef FREE
# include <QtGui/QCheckBox>
# include <QtGui/QSpinBox>

int VocabularyTabWidget::addTab(QWidget *page, const QString &label, const bool &pEnabled, const int &pPriority)
{
    int iTab = QTabWidget::addTab(page, label);

	// enabled/disabled
    if (_bShowEnabled) {
        QCheckBox *qcbEnabled = new QCheckBox(this);
        qcbEnabled->setCheckState(pEnabled ? Qt::Checked : Qt::Unchecked);
        tabBar()->setTabButton(iTab, POSITION_BUTTON_ENABLED, qcbEnabled);
        connect(qcbEnabled, SIGNAL(stateChanged(int)), SLOT(on_qcbEnabled_stateChanged(int)));
    } // if

	// priority
	if (_bShowPriorities) {
		QSpinBox *qsbPriority = new QSpinBox(this);
		qsbPriority->setMinimum(CATEGORY_PRIORITY_MIN);
		qsbPriority->setMaximum(CATEGORY_PRIORITY_MAX);
		qsbPriority->setValue(pPriority);
		tabBar()->setTabButton(iTab, POSITION_BUTTON_PRIORITY, qsbPriority);
		connect(qsbPriority, SIGNAL(valueChanged(int)), SLOT(on_qsbPriority_valueChanged(int)));
	} // if

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

const void VocabularyTabWidget::on_qsbPriority_valueChanged(int i) const
{
	for (int iI = 0; iI < count(); iI++) {
		if (sender() == tabBar()->tabButton(iI, POSITION_BUTTON_PRIORITY)) {
			emit TabPriorityChanged(iI, i);
			return;
		} // if
	} // for
} // on_qsbPriority_valueChanged

const void VocabularyTabWidget::SetShowEnabled(const bool &pEnabled)
{
    _bShowEnabled = pEnabled;
} // SetShowEnabled

const void VocabularyTabWidget::SetShowPriorities(const bool &pEnabled)
{
	_bShowPriorities = pEnabled;
} // SetShowPriorities
#endif

VocabularyTabWidget::VocabularyTabWidget(QWidget *pParent /* NULL */) : QTabWidget(pParent)
{
#ifndef FREE
    _bShowEnabled = true;
	_bShowPriorities = true;
#endif
} // VocabularyTabWidget