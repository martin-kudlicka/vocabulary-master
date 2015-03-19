#ifndef VOCABULARYORGANIZERDIALOG_H
#define VOCABULARYORGANIZERDIALOG_H

#include <ui_vocabularyorganizerdialog.h>

#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

class VocabularyOrganizerDialog : public QDialog
{
    Q_OBJECT

    public:
                 VocabularyOrganizerDialog(VocabularyOrganizer *organizer, QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~VocabularyOrganizerDialog() override;

	private:
		Ui::VocabularyOrganizerDialog _ui;
		VocabularyOrganizer          *_organizer;
		VocabularyOrganizerModel      _model;

#ifndef EDITION_TRY
		virtual void    accept        () override;
#endif
#ifdef EDITION_FREE
		        void    enableControls() const;
#endif
#ifndef EDITION_TRY
		        QString openPath      () const;
		virtual void    reject        () override;
#endif

	private slots:
		void on_close_clicked                              (bool checked = false);
		void on_new2_clicked                               (bool checked = false);
#ifndef EDITION_TRY
		void on_open_clicked                               (bool checked = false);
#endif
		void on_vocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
}; // VocabularyOrganizerDialog

#endif // VOCABULARYORGANIZERDIALOG_H