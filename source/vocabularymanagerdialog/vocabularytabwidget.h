#ifndef VOCABULARYTABWIDGET_H
#define VOCABULARYTABWIDGET_H

#include <QtWidgets/QTabWidget>
#ifndef EDITION_FREE
# include <QtWidgets/QTabBar>
#endif

class VocabularyTabWidget : public QTabWidget
{
#ifndef EDITION_FREE
  Q_OBJECT
#endif

  public:
#ifndef EDITION_FREE
    static const auto CATEGORY_PRIORITY_MAX = 9;
#endif

    VocabularyTabWidget(QWidget *parent = Q_NULLPTR);

#ifndef EDITION_FREE
    quintptr addTab           (QWidget *page, const QString &label, bool enabled, quintptr priority);
    void     setShowEnabled   (bool enabled);
    void     setShowPriorities(bool enabled);

  private:
    bool _showEnabled;
    bool _showPriorities;

    virtual ~VocabularyTabWidget() Q_DECL_OVERRIDE;

  Q_SIGNALS:
    void tabEnableChanged  (quintptr index, Qt::CheckState state) const;
    void tabPriorityChanged(quintptr index, quintptr value)       const;

  private Q_SLOTS:
      void on_enabled_stateChanged (int state) const;
      void on_priority_valueChanged(int i)     const;
#endif
};

#endif