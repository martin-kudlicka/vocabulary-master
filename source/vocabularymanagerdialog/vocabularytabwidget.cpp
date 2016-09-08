#include "vocabularymanagerdialog/vocabularytabwidget.h"

#ifndef EDITION_FREE
# include <QtWidgets/QCheckBox>
# include <QtWidgets/QSpinBox>
#endif

VocabularyTabWidget::VocabularyTabWidget(QWidget *pParent /* nullptr */) : QTabWidget(pParent)
#ifndef EDITION_FREE
    , _showEnabled(true), _showPriorities(true)
#endif
{
}

VocabularyTabWidget::~VocabularyTabWidget()
{
}

#ifndef EDITION_FREE
quint8 VocabularyTabWidget::addTab(QWidget *page, const QString &label, bool enabled, quint8 priority)
{
    const quint8 tab = QTabWidget::addTab(page, label);

	// enabled/disabled
    if (_showEnabled)
	{
        QCheckBox *enabledCheckBox = new QCheckBox(this);
        enabledCheckBox->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
        tabBar()->setTabButton(tab, POSITION_BUTTON_ENABLED, enabledCheckBox);
        connect(enabledCheckBox, SIGNAL(stateChanged(int)), SLOT(on_enabled_stateChanged(int)));
    }

	// priority
	if (_showPriorities)
	{
		QSpinBox *prioritySpinBox = new QSpinBox(this);
		prioritySpinBox->setMinimum(CATEGORY_PRIORITY_MIN);
		prioritySpinBox->setMaximum(CATEGORY_PRIORITY_MAX);
		prioritySpinBox->setValue(priority);
		tabBar()->setTabButton(tab, POSITION_BUTTON_PRIORITY, prioritySpinBox);
		connect(prioritySpinBox, SIGNAL(valueChanged(int)), SLOT(on_priority_valueChanged(int)));
	}

    return tab;
}

void VocabularyTabWidget::setShowEnabled(bool enabled)
{
    _showEnabled = enabled;
}

void VocabularyTabWidget::setShowPriorities(bool enabled)
{
	_showPriorities = enabled;
}

void VocabularyTabWidget::on_enabled_stateChanged(int state) const
{
    for (quint8 index = 0; index < count(); index++)
	{
        if (sender() == tabBar()->tabButton(index, POSITION_BUTTON_ENABLED))
		{
            emit tabEnableChanged(index, static_cast<Qt::CheckState>(state));
            return;
        }
    }
}

void VocabularyTabWidget::on_priority_valueChanged(int i) const
{
	for (int index = 0; index < count(); index++)
	{
		if (sender() == tabBar()->tabButton(index, POSITION_BUTTON_PRIORITY))
		{
			emit tabPriorityChanged(index, i);
			return;
		}
	}
}
#endif