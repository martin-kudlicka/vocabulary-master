#include "vocabularymanagerdialog/vocabularytabwidget.h"

#ifndef EDITION_FREE
# include <QtWidgets/QCheckBox>
# include <QtWidgets/QSpinBox>
#endif

static const auto CATEGORY_PRIORITY_MIN = 1;
static const auto POSITION_BUTTON_ENABLED = QTabBar::LeftSide;
static const auto POSITION_BUTTON_PRIORITY = QTabBar::RightSide;

VocabularyTabWidget::VocabularyTabWidget(QWidget *pParent /* Q_NULLPTR */) : QTabWidget(pParent)
#ifndef EDITION_FREE
                                         , _showEnabled(true), _showPriorities(true)
#endif
{
}

VocabularyTabWidget::~VocabularyTabWidget()
{
}

#ifndef EDITION_FREE
quintptr VocabularyTabWidget::addTab(QWidget *page, const QString &label, bool enabled, quintptr priority)
{
  const auto tab = QTabWidget::addTab(page, label);

  // enabled/disabled
  if (_showEnabled)
  {
    auto enabledCheckBox = new QCheckBox(this);
    enabledCheckBox->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
    tabBar()->setTabButton(tab, POSITION_BUTTON_ENABLED, enabledCheckBox);
    connect(enabledCheckBox, SIGNAL(stateChanged(int)), SLOT(on_enabled_stateChanged(int)));
  }

  // priority
  if (_showPriorities)
  {
    auto prioritySpinBox = new QSpinBox(this);
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
  for (auto index = 0; index < count(); index++)
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
  for (auto index = 0; index < count(); index++)
  {
    if (sender() == tabBar()->tabButton(index, POSITION_BUTTON_PRIORITY))
    {
      emit tabPriorityChanged(index, i);
      return;
    }
  }
}
#endif