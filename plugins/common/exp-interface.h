#ifndef EXPINTERFACE_H
#define EXPINTERFACE_H

#include <QtPlugin>

static const char *EXP_INTERFACE = "Exp Interface 1.0";

class ExpInterface : public QObject
{
    Q_OBJECT

	public:
		static const int WIDGET_POSITION = 0;

		virtual const QString GetPluginName() const = 0;
		virtual const void SetupUI(QWidget *pParent) = 0;

    signals:
        void VocabularyGetMarks(QStringList *pMarks) const;
}; // ExpInterface

Q_DECLARE_INTERFACE(ExpInterface, EXP_INTERFACE);

#endif // EXPINTERFACE_H