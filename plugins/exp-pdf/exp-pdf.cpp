#include "exp-pdf.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QTextCodec>
#include <hpdf.h>

const void ExpPdf::addFont(const HPDF_Doc &pdf, FontList *fontList, const PdfExportWidget::FontRole &fontRole, const qint8 &num /* PdfExportWidget::FONTROLE_NONE */) const
{
	PdfExportWidget::FontRoleInfo fontRoleInfo = _widget->getFontRoleInfo(fontRole, num);

	Font font;
	font.font      = HPDF_GetFont(pdf, fontRoleInfo.font.toLocal8Bit(), fontRoleInfo.encoding.toLocal8Bit());
	font.size      = fontRoleInfo.size;
	font.textCodec = QTextCodec::codecForName(fontRoleInfo.textCodec);
	fontList->append(font);
} // addFont

const void ExpPdf::beginExport() const
{
	// get filename
	QString fileName = QFileDialog::getSaveFileName(_widget, QString(), QString(), tr("pdf (*.pdf)"));
	if (fileName.isEmpty())
	{
		return;
	} // if

	// marks
	QStringList marks;
	emit vocabularyGetMarks(&marks);

	// PDF
	HPDF_Doc pdf = HPDF_New(NULL, NULL);
	HPDF_SetCompressionMode(pdf, _widget->getCompression());

	// get font info with font and encoding set
	QList<Font> fonts;
	initFonts(pdf, &fonts, marks.size());

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
	HPDF_Page page = NULL;
	pdfAddPage(pdf, &page, fonts.at(PdfExportWidget::FontRoleCategory).size);
	bool firstLine = true;
	quint32 records = 0;
	foreach (const quint8 &categoryId, categoryIds) {
        if (firstLine)
		{
            firstLine = false;
        }
		else
		{
			bool newPage = pdfNextLine(pdf, &page);
			if (!newPage)
			{
				newPage = pdfNextLine(pdf, &page);
			} // if

			if (newPage)
			{
				// header
				pdfShowTableHeader(page, fonts);
			} // if
        } // if else

		// category
        QString categoryName;
        emit vocabularyGetCategoryName(categoryId, &categoryName);
		pdfSetFont(page, fonts.at(PdfExportWidget::FontRoleCategory).font, fonts.at(PdfExportWidget::FontRoleCategory).size);
		pdfShowText(page, categoryName, fonts.at(PdfExportWidget::FontRoleCategory).textCodec);

		// header
		pdfNextLine(pdf, &page);
		pdfShowTableHeader(page, fonts);

        // records
        ExpInterface::RecordIdList recordIds;
        emit vocabularyGetRecordIds(categoryId, &recordIds);
        foreach (const quint32 &recordId, recordIds)
		{
			if (pdfNextLine(pdf, &page))
			{
				// header
				pdfShowTableHeader(page, fonts);
				pdfNextLine(pdf, &page);
			} // if

			if (_widget->getStyle() == PdfExportWidget::StyleText)
			{
				exportText(recordId, page, fonts, marks, _widget->getTextTemplate());
			}
			else
			{
				exportTable(recordId, page, fonts, marks);
			} // if else

            records++;
            emit progressExportSetValue(records);
        } // foreach
    } // foreach

	HPDF_SaveToFile(pdf, fileName.toLocal8Bit());

	emit progressExportSetValue(0);
	HPDF_Free(pdf);
} // beginExport

const void ExpPdf::exportTable(const quint32 &recordId, const HPDF_Page &page, const FontList &fontList, const QStringList &marks) const
{
	const PdfExportWidget::TableColumns *columns = _widget->getTableColumns();
	foreach (const PdfExportWidget::TableColumn &column, *columns)
	{
		// write column text
		exportText(recordId, page, fontList, marks, column.templateEdit->text());

		// next column
		HPDF_Page_MoveTextPos(page, column.width->value(), 0);
	} // foreach
} // exportTable

const void ExpPdf::exportText(const quint32 &recordId, const HPDF_Page &page, const FontList &fontList, const QStringList &marks, const QString &templateText) const
{
	// analyze template
	quint8 pos = 0;
	while (pos < templateText.size())
	{
		qint8 markPos = templateText.indexOf('$', pos);

		if (markPos == -1) 
		{
			// no other mark on the line
			pdfSetFont(page, fontList.at(PdfExportWidget::FontRoleTemplate).font, fontList.at(PdfExportWidget::FontRoleTemplate).size);
			QString text = templateText.mid(pos);
			pdfShowText(page, text, fontList.at(PdfExportWidget::FontRoleTemplate).textCodec);

			break;
		}
		else
		{
			// text before possible mark
			if (markPos > pos)
			{
				pdfSetFont(page, fontList.at(PdfExportWidget::FontRoleTemplate).font, fontList.at(PdfExportWidget::FontRoleTemplate).size);
				QString text = templateText.mid(pos, markPos - pos);
				pdfShowText(page, text, fontList.at(PdfExportWidget::FontRoleTemplate).textCodec);
			} // if
			pos = markPos;

			// check if valid mark
			for (quint8 markIndex = 0; markIndex < marks.size(); markIndex++)
			{
				QString mark = marks.at(markIndex);
				if (templateText.mid(markPos, mark.size()) == mark)
				{
					// valid mark, replace marks for data
					QString data;
					emit vocabularyGetMarkText(recordId, mark, &data);

					// show data
					pdfSetFont(page, fontList.at(PdfExportWidget::FontRoleMark + markIndex).font, fontList.at(PdfExportWidget::FontRoleMark + markIndex).size);
					pdfShowText(page, data, fontList.at(PdfExportWidget::FontRoleMark + markIndex).textCodec);

					pos += mark.size() - 1;
					break;
				} // if
			} // for

			pos++;
		} // if else
	} // while
} // exportText

const void ExpPdf::initFonts(const HPDF_Doc &pdf, FontList *fontList, const quint8 &markCount) const
{
	// get demanded fonts and encodings
	PdfExportWidget::FontRoleInfo font         = _widget->getFontRoleInfo(PdfExportWidget::FontRoleCategory);
	PdfExportWidget::FontSets fontSets         = font.fontSet;
	PdfExportWidget::EncodingSets encodingSets = font.encodingSet;
	font                                       = _widget->getFontRoleInfo(PdfExportWidget::FontRoleTemplate);
	fontSets                                  |= font.fontSet;
	encodingSets                              |= font.encodingSet;
	for (quint8 markIndex = 0; markIndex < markCount; markIndex++)
	{
		font          = _widget->getFontRoleInfo(PdfExportWidget::FontRoleMark, markIndex);
		fontSets     |= font.fontSet;
		encodingSets |= font.encodingSet;
	} // for

	// enable demanded CID fonts
	if (fontSets & PdfExportWidget::FontSetCNS)
	{
		HPDF_UseCNSFonts(pdf);
	} // if
	if (fontSets & PdfExportWidget::FontSetCNT)
	{
		HPDF_UseCNTFonts(pdf);
	} // if
	if (fontSets & PdfExportWidget::FontSetJP)
	{
		HPDF_UseJPFonts(pdf);
	} // if
	if (fontSets & PdfExportWidget::FontSetKR)
	{
		HPDF_UseKRFonts(pdf);
	} // if

	// enable demanded encodings
	if (encodingSets & PdfExportWidget::EncodingSetCNS)
	{
		HPDF_UseCNSEncodings(pdf);
	} // if
	if (encodingSets & PdfExportWidget::EncodingSetCNT)
	{
		HPDF_UseCNTEncodings(pdf);
	} // if
	if (encodingSets & PdfExportWidget::EncodingSetJPE)
	{
		HPDF_UseJPEncodings(pdf);
	} // if
	if (encodingSets & PdfExportWidget::EncodingSetKRE)
	{
		HPDF_UseKREncodings(pdf);
	} // if

	// load fonts
	addFont(pdf, fontList, PdfExportWidget::FontRoleCategory);
	addFont(pdf, fontList, PdfExportWidget::FontRoleTemplate);
	for (quint8 markIndex = 0; markIndex < markCount; markIndex++)
	{
		addFont(pdf, fontList, PdfExportWidget::FontRoleMark, markIndex);
	} // for
} // initFonts

const void ExpPdf::on_widget_vocabularyGetMarks(QStringList *marks) const
{
	emit vocabularyGetMarks(marks);
} // on_widget_vocabularyGetMarks

const void ExpPdf::pdfAddPage(const HPDF_Doc &pdf, HPDF_Page *page, const HPDF_REAL &defaultSize /* 0 */) const
{
	// remember current font and size
	HPDF_Font font;
	HPDF_REAL size;
	if (*page)
	{
		font = HPDF_Page_GetCurrentFont(*page);
		size = HPDF_Page_GetCurrentFontSize(*page);
	}
	else
	{
		font = NULL;
		size = defaultSize;
	} // if else

	*page = HPDF_AddPage(pdf);
	HPDF_Page_SetSize(*page, _widget->getPageSize(), HPDF_PAGE_PORTRAIT);
	HPDF_Page_BeginText(*page);

	// horizontal and vertical border
	HPDF_Page_MoveTextPos(*page, _widget->getBorder(), HPDF_Page_GetHeight(*page) - _widget->getBorder() - size);

	// set previous font and size
	if (font)
	{
		pdfSetFont(*page, font, size);
	} // if
} // pdfAddPage

const bool ExpPdf::pdfNextLine(const HPDF_Doc &pdf, HPDF_Page *page) const
{
	// check current position
	HPDF_REAL size      = HPDF_Page_GetCurrentFontSize(*page);
	HPDF_Point position = HPDF_Page_GetCurrentTextPos(*page);
	if (position.y < size + _widget->getBorder())
	{
		pdfAddPage(pdf, page);
		return true;
	}
	else
	{
		HPDF_Page_MoveToNextLine(*page);

		// horizontal border
		position = HPDF_Page_GetCurrentTextPos(*page);
		HPDF_Page_MoveTextPos(*page, _widget->getBorder() - position.x, 0);

		return false;
	} // if else
} // pdfNextLine

const void ExpPdf::pdfSetFont(const HPDF_Page &page, const HPDF_Font &font, const quint8 &size) const
{
	HPDF_Page_SetFontAndSize(page, font, size);
	HPDF_Page_SetTextLeading(page, size);
} // pdfSetFont

const void ExpPdf::pdfShowTableHeader(const HPDF_Page &page, const FontList &fontList) const
{
	const PdfExportWidget::TableColumns *columns = _widget->getTableColumns();
	foreach (const PdfExportWidget::TableColumn &column, *columns)
	{
		// write column header
		exportText(RECORD_NONE, page, fontList, QStringList(), column.headerEdit->text());

		// next column
		HPDF_Page_MoveTextPos(page, column.width->value(), 0);
	} // foreach
} // pdfShowTableHeader

const void ExpPdf::pdfShowText(const HPDF_Page &page, const QString &text, const QTextCodec *textCodec) const
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

	HPDF_Page_ShowText(page, encoded);
} // pdfShowText

const QString ExpPdf::pluginName() const
{
	return tr("Adobe Reader (pdf)");
} // pluginName

const void ExpPdf::setupUi(QWidget *parent)
{
	_widget = new PdfExportWidget(parent);
	QBoxLayout *layout = qobject_cast<QBoxLayout *>(parent->layout());
	layout->insertWidget(WIDGET_POSITION, _widget);

	connect(_widget, SIGNAL(vocabularyGetMarks(QStringList *)), SLOT(on_widget_vocabularyGetMarks(QStringList *)));

	_widget->initMarkFonts();
} // setupUi