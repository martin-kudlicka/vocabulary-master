#ifndef VOCABULARYTABWIDGET_H
#define VOCABULARYTABWIDGET_H

#include <QtWidgets/QTabWidget>
#ifndef FREE
# include <QtWidgets/QTabBar>
#endif

class VocabularyTabWidget : public QTabWidget
{
#ifndef FREE
    Q_OBJECT
#endif

    public:
#ifndef FREE
        static const int CATEGORY_PRIORITY_MAX = 9;
#endif

        VocabularyTabWidget(QWidget *pParent = NULL);

#ifndef FREE
        int addTab(QWidget *page, const QString &label, const bool &pEnabled, const int &pPriority);
        const void SetShowEnabled(const bool &pEnabled);
		const void SetShowPriorities(const bool &pEnabled);

    private:
		static const int CATEGORY_PRIORITY_MIN = 1;
        static const QTabBar::ButtonPosition POSITION_BUTTON_ENABLED = QTabBar::LeftSide;
		static const QTabBar::ButtonPosition POSITION_BUTTON_PRIORITY = QTabBar::RightSide;

        bool _bShowEnabled;
		bool _bShowPriorities;

    signals:
        void TabEnableChanged(const int &pIndex, const Qt::CheckState &pState) const;
		void TabPriorityChanged(const int &pIndex, const int &pValue) const;

    private slots:
        const void on_qcbEnabled_stateChanged(int state) const;
		const void on_qsbPriority_valueChanged(int i) const;
#endif
}; // VocabularyTabWidget

#endif // VOCABULARYTABWIDGET_H