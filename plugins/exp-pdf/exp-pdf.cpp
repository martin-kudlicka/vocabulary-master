#include "exp-pdf.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QTextCodec>
#include <hpdf.h>

const void ExpPdf::addFont(const HPDF_Doc &pdfDocument, FontList *fontList, const PdfExportWidget::FontRole &fontRole, const qint8 &num /* PdfExportWidget::FONTROLE_NONE */) const
{
	const PdfExportWidget::FontRoleInfo fontRoleInfo = _widget->fontRoleInfo(fontRole, num);

	Font font;
	font.pdfFont   = HPDF_GetFont(pdfDocument, fontRoleInfo.font.toLocal8Bit(), fontRoleInfo.encoding.toLocal8Bit());
	font.size      = fontRoleInfo.size;
	font.textCodec = QTextCodec::codecForName(fontRoleInfo.textCodec);
	fontList->append(font);
} // addFont

const void ExpPdf::beginExport() const
{
	// get filename
	const QString fileName = QFileDialog::getSaveFileName(_widget, QString(), QString(), tr("pdf (*.pdf)"));
	if (fileName.isEmpty())
	{
		return;
	} // if

	// marks
	QStringList marks;
	emit vocabularyGetMarks(&marks);

	// PDF
	const HPDF_Doc pdfDocument = HPDF_New(NULL, NULL);
	HPDF_SetCompressionMode(pdfDocument, _widget->compression());

	// get font info with font and encoding set
	QList<Font> fonts;
	initFonts(pdfDocument, &fonts, marks.size());

	// categories
	ExpInterface::CategoryIdList categoryIds;
	emit vocabularyGetCategoryIds(&categoryIds);

	// total record count for progress
	quint32 totalRecords = 0;
	foreach (const quint8 &categoryId, categoryIds)
	{
		quint32 records;
		emit vocabularyGetRecordCount(categoryId, &records);
		totalRecords += records;
	} // foreach
	emit progressExportSetMax(totalRecords);

	// export
	HPDF_Page pdfPage = NULL;
	pdfAddPage(pdfDocument, &pdfPage, fonts.at(PdfExportWidget::FontRoleCategory).size);
	bool firstLine = true;
	quint32 records = 0;
	foreach (const quint8 &categoryId, categoryIds) {
        if (firstLine)
		{
            firstLine = false;
        }
		else
		{
			bool newPage = pdfNextLine(pdfDocument, &pdfPage);
			if (!newPage)
			{
				newPage = pdfNextLine(pdfDocument, &pdfPage);
			} // if

			if (newPage)
			{
				// header
				pdfShowTableHeader(pdfPage, fonts);
			} // if
        } // if else

		// category
        QString categoryName;
        emit vocabularyGetCategoryName(categoryId, &categoryName);
		pdfSetFont(pdfPage, fonts.at(PdfExportWidget::FontRoleCategory).pdfFont, fonts.at(PdfExportWidget::FontRoleCategory).size);
		pdfShowText(pdfPage, categoryName, fonts.at(PdfExportWidget::FontRoleCategory).textCodec);

		// header
		pdfNextLine(pdfDocument, &pdfPage);
		pdfShowTableHeader(pdfPage, fonts);

        // records
        ExpInterface::RecordIdList recordIds;
        emit vocabularyGetRecordIds(categoryId, &recordIds);
        foreach (const quint32 &recordId, recordIds)
		{
			if (pdfNextLine(pdfDocument, &pdfPage))
			{
				// header
				pdfShowTableHeader(pdfPage, fonts);
				pdfNextLine(pdfDocument, &pdfPage);
			} // if

			if (_widget->style() == PdfExportWidget::StyleText)
			{
				exportText(recordId, pdfPage, fonts, marks, _widget->textTemplate());
			}
			else
			{
				exportTable(recordId, pdfPage, fonts, marks);
			} // if else

            records++;
            emit progressExportSetValue(records);
        } // foreach
    } // foreach

	HPDF_SaveToFile(pdfDocument, fileName.toLocal8Bit());

	emit progressExportSetValue(0);
	HPDF_Free(pdfDocument);
} // beginExport

const void ExpPdf::exportTable(const quint32 &recordId, const HPDF_Page &pdfPage, const FontList &fontList, const QStringList &marks) const
{
	const PdfExportWidget::TableColumns *tableColumns = _widget->tableColumns();
	foreach (const PdfExportWidget::TableColumn &tableColumn, *tableColumns)
	{
		// write column text
		exportText(recordId, pdfPage, fontList, marks, tableColumn.templateEdit->text());

		// next column
		HPDF_Page_MoveTextPos(pdfPage, tableColumn.width->value(), 0);
	} // foreach
} // exportTable

const void ExpPdf::exportText(const quint32 &recordId, const HPDF_Page &pdfPage, const FontList &fontList, const QStringList &marks, const QString &templateText) const
{
	// analyze template
	quint8 pos = 0;
	while (pos < templateText.size())
	{
		const qint8 markPos = templateText.indexOf('$', pos);

		if (markPos == -1) 
		{
			// no other mark on the line
			pdfSetFont(pdfPage, fontList.at(PdfExportWidget::FontRoleTemplate).pdfFont, fontList.at(PdfExportWidget::FontRoleTemplate).size);
			const QString text = templateText.mid(pos);
			pdfShowText(pdfPage, text, fontList.at(PdfExportWidget::FontRoleTemplate).textCodec);

			break;
		}
		else
		{
			// text before possible mark
			if (markPos > pos)
			{
				pdfSetFont(pdfPage, fontList.at(PdfExportWidget::FontRoleTemplate).pdfFont, fontList.at(PdfExportWidget::FontRoleTemplate).size);
				const QString text = templateText.mid(pos, markPos - pos);
				pdfShowText(pdfPage, text, fontList.at(PdfExportWidget::FontRoleTemplate).textCodec);
			} // if
			pos = markPos;

			// check if valid mark
			for (quint8 markIndex = 0; markIndex < marks.size(); markIndex++)
			{
				const QString mark = marks.at(markIndex);
				if (templateText.mid(markPos, mark.size()) == mark)
				{
					// valid mark, replace marks for data
					QString data;
					emit vocabularyGetMarkText(recordId, mark, &data);

					// show data
					pdfSetFont(pdfPage, fontList.at(PdfExportWidget::FontRoleMark + markIndex).pdfFont, fontList.at(PdfExportWidget::FontRoleMark + markIndex).size);
					pdfShowText(pdfPage, data, fontList.at(PdfExportWidget::FontRoleMark + markIndex).textCodec);

					pos += mark.size() - 1;
					break;
				} // if
			} // for

			pos++;
		} // if else
	} // while
} // exportText

const void ExpPdf::initFonts(const HPDF_Doc &pdfDocument, FontList *fontList, const quint8 &markCount) const
{
	// get demanded fonts and encodings
	PdfExportWidget::FontRoleInfo font         = _widget->fontRoleInfo(PdfExportWidget::FontRoleCategory);
	PdfExportWidget::FontSets fontSets         = font.fontSet;
	PdfExportWidget::EncodingSets encodingSets = font.encodingSet;
	font                                       = _widget->fontRoleInfo(PdfExportWidget::FontRoleTemplate);
	fontSets                                  |= font.fontSet;
	encodingSets                              |= font.encodingSet;
	for (quint8 markIndex = 0; markIndex < markCount; markIndex++)
	{
		font          = _widget->fontRoleInfo(PdfExportWidget::FontRoleMark, markIndex);
		fontSets     |= font.fontSet;
		encodingSets |= font.encodingSet;
	} // for

	// enable demanded CID fonts
	if (fontSets & PdfExportWidget::FontSetCNS)
	{
		HPDF_UseCNSFonts(pdfDocument);
	} // if
	if (fontSets & PdfExportWidget::FontSetCNT)
	{
		HPDF_UseCNTFonts(pdfDocument);
	} // if
	if (fontSets & PdfExportWidget::FontSetJP)
	{
		HPDF_UseJPFonts(pdfDocument);
	} // if
	if (fontSets & PdfExportWidget::FontSetKR)
	{
		HPDF_UseKRFonts(pdfDocument);
	} // if

	// enable demanded encodings
	if (encodingSets & PdfExportWidget::EncodingSetCNS)
	{
		HPDF_UseCNSEncodings(pdfDocument);
	} // if
	if (encodingSets & PdfExportWidget::EncodingSetCNT)
	{
		HPDF_UseCNTEncodings(pdfDocument);
	} // if
	if (encodingSets & PdfExportWidget::EncodingSetJPE)
	{
		HPDF_UseJPEncodings(pdfDocument);
	} // if
	if (encodingSets & PdfExportWidget::EncodingSetKRE)
	{
		HPDF_UseKREncodings(pdfDocument);
	} // if

	// load fonts
	addFont(pdfDocument, fontList, PdfExportWidget::FontRoleCategory);
	addFont(pdfDocument, fontList, PdfExportWidget::FontRoleTemplate);
	for (quint8 markIndex = 0; markIndex < markCount; markIndex++)
	{
		addFont(pdfDocument, fontList, PdfExportWidget::FontRoleMark, markIndex);
	} // for
} // initFonts

const void ExpPdf::on_widget_vocabularyGetMarks(QStringList *marks) const
{
	emit vocabularyGetMarks(marks);
} // on_widget_vocabularyGetMarks

const void ExpPdf::pdfAddPage(const HPDF_Doc &pdfDocument, HPDF_Page *pdfPage, const HPDF_REAL &defaultSize /* 0 */) const
{
	// remember current font and size
	HPDF_Font pdfFont;
	HPDF_REAL pdfSize;
	if (*pdfPage)
	{
		pdfFont = HPDF_Page_GetCurrentFont(*pdfPage);
		pdfSize = HPDF_Page_GetCurrentFontSize(*pdfPage);
	}
	else
	{
		pdfFont = NULL;
		pdfSize = defaultSize;
	} // if else

	*pdfPage = HPDF_AddPage(pdfDocument);
	HPDF_Page_SetSize(*pdfPage, _widget->pageSize(), HPDF_PAGE_PORTRAIT);
	HPDF_Page_BeginText(*pdfPage);

	// horizontal and vertical border
	HPDF_Page_MoveTextPos(*pdfPage, _widget->border(), HPDF_Page_GetHeight(*pdfPage) - _widget->border() - pdfSize);

	// set previous font and size
	if (pdfFont)
	{
		pdfSetFont(*pdfPage, pdfFont, pdfSize);
	} // if
} // pdfAddPage

const bool ExpPdf::pdfNextLine(const HPDF_Doc &pdfDocument, HPDF_Page *pdfPage) const
{
	// check current position
	const HPDF_REAL pdfSize = HPDF_Page_GetCurrentFontSize(*pdfPage);
	HPDF_Point pdfPosition  = HPDF_Page_GetCurrentTextPos(*pdfPage);
	if (pdfPosition.y < pdfSize + _widget->border())
	{
		pdfAddPage(pdfDocument, pdfPage);
		return true;
	}
	else
	{
		HPDF_Page_MoveToNextLine(*pdfPage);

		// horizontal border
		pdfPosition = HPDF_Page_GetCurrentTextPos(*pdfPage);
		HPDF_Page_MoveTextPos(*pdfPage, _widget->border() - pdfPosition.x, 0);

		return false;
	} // if else
} // pdfNextLine

const void ExpPdf::pdfSetFont(const HPDF_Page &pdfPage, const HPDF_Font &pdfFont, const quint8 &size) const
{
	HPDF_Page_SetFontAndSize(pdfPage, pdfFont, size);
	HPDF_Page_SetTextLeading(pdfPage, size);
} // pdfSetFont

const void ExpPdf::pdfShowTableHeader(const HPDF_Page &pdfPage, const FontList &fontList) const
{
	const PdfExportWidget::TableColumns *columns = _widget->tableColumns();
	foreach (const PdfExportWidget::TableColumn &column, *columns)
	{
		// write column header
		exportText(RECORD_NONE, pdfPage, fontList, QStringList(), column.headerEdit->text());

		// next column
		HPDF_Page_MoveTextPos(pdfPage, column.width->value(), 0);
	} // foreach
} // pdfShowTableHeader

const void ExpPdf::pdfShowText(const HPDF_Page &pdfPage, const QString &text, const QTextCodec *textCodec) const
{
	QByteArray encoded;
	if (textCodec)
	{
		encoded = textCodec->fromUnicode(text);
	}
	else
	{
		encoded = text.toLatin1();
	} // if else

	HPDF_Page_ShowText(pdfPage, encoded);
} // pdfShowText

const QString ExpPdf::pluginName() const
{
	return tr("Adobe Reader (pdf)");
} // pluginName

const void ExpPdf::setupUi(QWidget *parent)
{
	_widget = new PdfExportWidget(parent);
	QBoxLayout *boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
	boxLayout->insertWidget(WIDGET_POSITION, _widget);

	connect(_widget, SIGNAL(vocabularyGetMarks(QStringList *)), SLOT(on_widget_vocabularyGetMarks(QStringList *)));

	_widget->initMarkFonts();
} // setupUi