#include "vocabularymanagerdialog/vocabularytabwidget.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSpinBox>

static const auto CATEGORY_PRIORITY_MIN    = 1;
static const auto POSITION_BUTTON_ENABLED  = QTabBar::LeftSide;
static const auto POSITION_BUTTON_PRIORITY = QTabBar::RightSide;

VocabularyTabWidget::VocabularyTabWidget(QWidget *pParent /* Q_NULLPTR */) : QTabWidget(pParent), _showEnabled(true), _showPriorities(true)
{
}

quintptr VocabularyTabWidget::addTab(QWidget *page, const QString &label, bool enabled, quintptr priority)
{
  const auto tab = QTabWidget::addTab(page, label);

  // enabled/disabled
  if (_showEnabled)
  {
    auto enabledCheckBox = new QCheckBox(this);
    enabledCheckBox->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
    tabBar()->setTabButton(tab, POSITION_BUTTON_ENABLED, enabledCheckBox);
    connect(enabledCheckBox, &QCheckBox::stateChanged, this, &VocabularyTabWidget::on_enabled_stateChanged);
  }

  // priority
  if (_showPriorities)
  {
    auto prioritySpinBox = new QSpinBox(this);
    prioritySpinBox->setMinimum(CATEGORY_PRIORITY_MIN);
    prioritySpinBox->setMaximum(CATEGORY_PRIORITY_MAX);
    prioritySpinBox->setValue(priority);
    tabBar()->setTabButton(tab, POSITION_BUTTON_PRIORITY, prioritySpinBox);
    connect(prioritySpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &VocabularyTabWidget::on_priority_valueChanged);
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