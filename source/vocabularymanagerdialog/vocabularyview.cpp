#include "vocabularymanagerdialog/vocabularyview.h"

#ifndef EDITION_FREE
#include <QtGui/QMouseEvent>
#include <QtWidgets/QSpinBox>
#include <QtCore/QCoreApplication>
#endif

VocabularyView::VocabularyView(
#ifndef EDITION_FREE
    Vocabulary *vocabulary,
#endif
    QWidget *parent /* NULL */) : QTableView(parent)
#ifndef EDITION_FREE
	, _vocabulary(vocabulary)
#endif
{
#ifndef EDITION_FREE
    setMouseTracking(true);
    viewport()->installEventFilter(this);
#endif
} // VocabularyView

VocabularyView::~VocabularyView()
{
} // ~VocabularyView

#ifndef EDITION_FREE
bool VocabularyView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == viewport() && event->type() == QEvent::Leave)
	{
        update(_moveOld);
        _moveOld = QModelIndex();
    } // if

    return QTableView::eventFilter(watched, event);
} // eventFilter

void VocabularyView::mouseMoveEvent(QMouseEvent *event)
{
    const QModelIndex mouse = indexAt(event->pos());
    if (_moveOld.isValid() && _moveOld != mouse)
	{
        update(_moveOld);
    } // if
    _moveOld = mouse;

    const quint8 fieldId = _vocabulary->fieldId(mouse.column());
    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
	{
        const VocabularyDatabase::FieldBuiltIn builtIn = _vocabulary->fieldBuiltIn(fieldId);
        if (builtIn == VocabularyDatabase::FieldBuiltInPriority)
		{
            update(mouse);
        } // if
    } // if

    QTableView::mouseMoveEvent(event);
} // mouseMoveEvent

void VocabularyView::mousePressEvent(QMouseEvent *event)
{
    const QModelIndex oldIndex = currentIndex();
    QTableView::mousePressEvent(event);
    const QModelIndex newIndex = currentIndex();

    if (oldIndex != newIndex && event->button() == Qt::LeftButton)
	{
        const quint8 fieldId = _vocabulary->fieldId(newIndex.column());
        if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
            const VocabularyDatabase::FieldBuiltIn builtIn = _vocabulary->fieldBuiltIn(fieldId);
            if (builtIn == VocabularyDatabase::FieldBuiltInPriority)
			{
                QSpinBox *editor       = qobject_cast<QSpinBox *>(indexWidget(newIndex));
                const QPoint editorPos = editor->mapFromParent(event->pos());

                QMouseEvent press(QEvent::MouseButtonPress, editorPos, Qt::LeftButton, Qt::LeftButton, 0);
                QCoreApplication::sendEvent(editor, &press);
                QMouseEvent release(QEvent::MouseButtonRelease, editorPos, Qt::LeftButton, Qt::LeftButton, 0);
                QCoreApplication::sendEvent(editor, &release);
            } // if
        } // if
    } // if
} // mousePressEvent
#endif