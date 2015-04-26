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
        static const quint8 CATEGORY_PRIORITY_MAX = 9;
#endif

        VocabularyTabWidget(QWidget *parent = nullptr);

#ifndef EDITION_FREE
        quint8 addTab           (QWidget *page, const QString &label, bool enabled, quint8 priority);
        void   setShowEnabled   (bool enabled);
		void   setShowPriorities(bool enabled);

    private:
		static const quint8                  CATEGORY_PRIORITY_MIN    = 1;
        static const QTabBar::ButtonPosition POSITION_BUTTON_ENABLED  = QTabBar::LeftSide;
		static const QTabBar::ButtonPosition POSITION_BUTTON_PRIORITY = QTabBar::RightSide;

        bool _showEnabled;
		bool _showPriorities;

		virtual ~VocabularyTabWidget() override;

    signals:
        void tabEnableChanged  (quint8 index, Qt::CheckState state) const;
		void tabPriorityChanged(quint8 index, quint8 value)         const;

    private slots:
        void on_enabled_stateChanged (int state) const;
		void on_priority_valueChanged(int i)     const;
#endif
}; // VocabularyTabWidget

#endif // VOCABULARYTABWIDGET_H