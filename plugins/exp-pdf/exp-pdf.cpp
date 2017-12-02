#include "exp-pdf.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QTextCodec>
#include <hpdf.h>

Q_DECL_CONSTEXPR auto RECORD_NONE = -1;

void ExpPdf::addFont(HPDF_Doc pdfDocument, FontList *fontList, PdfExportWidget::FontRole fontRole, qintptr num /* PdfExportWidget::FONTROLE_NONE */) const
{
  const auto fontRoleInfo = _widget->fontRoleInfo(fontRole, num);

  Font font;
  font.pdfFont   = HPDF_GetFont(pdfDocument, fontRoleInfo.font.toLocal8Bit(), fontRoleInfo.encoding.toLocal8Bit());
  font.size      = fontRoleInfo.size;
  font.textCodec = QTextCodec::codecForName(fontRoleInfo.textCodec);
  fontList->append(font);
}

void ExpPdf::exportTable(quintptr recordId, HPDF_Page pdfPage, const FontList &fontList, const QStringList &marks) const
{
  const auto tableColumns = _widget->tableColumns();
  for (const auto &tableColumn : *tableColumns)
  {
    // write column text
    exportText(recordId, pdfPage, fontList, marks, tableColumn.templateEdit->text());

    // next column
    HPDF_Page_MoveTextPos(pdfPage, tableColumn.width->value(), 0);
  }
}

void ExpPdf::exportText(quintptr recordId, HPDF_Page pdfPage, const FontList &fontList, const QStringList &marks, const QString &templateText) const
{
  // analyze template
  auto pos = 0;
  while (pos < templateText.size())
  {
    const auto markPos = templateText.indexOf('$', pos);

    if (markPos == -1)
    {
      // no other mark on the line
      pdfSetFont(pdfPage, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Template)).pdfFont, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Template)).size);
      const auto text = templateText.mid(pos);
      pdfShowText(pdfPage, text, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Template)).textCodec);

      break;
    }
    else
    {
      // text before possible mark
      if (markPos > pos)
      {
        pdfSetFont(pdfPage, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Template)).pdfFont, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Template)).size);
        const auto text = templateText.mid(pos, markPos - pos);
        pdfShowText(pdfPage, text, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Template)).textCodec);
      }
      pos = markPos;

      // check if valid mark
      for (auto markIndex = 0; markIndex < marks.size(); ++markIndex)
      {
        const auto mark = marks.at(markIndex);
        if (templateText.mid(markPos, mark.size()) == mark)
        {
          // valid mark, replace marks for data
          QString data;
          emit vocabularyGetMarkText(recordId, mark, &data);

          // show data
          pdfSetFont(pdfPage, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Mark) + markIndex).pdfFont, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Mark) + markIndex).size);
          pdfShowText(pdfPage, data, fontList.at(static_cast<int>(PdfExportWidget::FontRole::Mark) + markIndex).textCodec);

          pos += mark.size() - 1;
          break;
        }
      }

      ++pos;
    }
  }
}

void ExpPdf::initFonts(HPDF_Doc pdfDocument, FontList *fontList, qintptr markCount) const
{
  // get demanded fonts and encodings
  auto font                                  = _widget->fontRoleInfo(PdfExportWidget::FontRole::Category);
  PdfExportWidget::FontSets fontSets         = font.fontSet;
  PdfExportWidget::EncodingSets encodingSets = font.encodingSet;
  font              = _widget->fontRoleInfo(PdfExportWidget::FontRole::Template);
  fontSets         |= font.fontSet;
  encodingSets     |= font.encodingSet;
  for (auto markIndex = 0; markIndex < markCount; ++markIndex)
  {
    font          = _widget->fontRoleInfo(PdfExportWidget::FontRole::Mark, markIndex);
    fontSets     |= font.fontSet;
    encodingSets |= font.encodingSet;
  }

  // enable demanded CID fonts
  if (fontSets & PdfExportWidget::FontSet::CNS)
  {
    HPDF_UseCNSFonts(pdfDocument);
  }
  if (fontSets & PdfExportWidget::FontSet::CNT)
  {
    HPDF_UseCNTFonts(pdfDocument);
  }
  if (fontSets & PdfExportWidget::FontSet::JP)
  {
    HPDF_UseJPFonts(pdfDocument);
  }
  if (fontSets & PdfExportWidget::FontSet::KR)
  {
    HPDF_UseKRFonts(pdfDocument);
  }

  // enable demanded encodings
  if (encodingSets & PdfExportWidget::EncodingSet::CNS)
  {
    HPDF_UseCNSEncodings(pdfDocument);
  }
  if (encodingSets & PdfExportWidget::EncodingSet::CNT)
  {
    HPDF_UseCNTEncodings(pdfDocument);
  }
  if (encodingSets & PdfExportWidget::EncodingSet::JPE)
  {
    HPDF_UseJPEncodings(pdfDocument);
  }
  if (encodingSets & PdfExportWidget::EncodingSet::KRE)
  {
    HPDF_UseKREncodings(pdfDocument);
  }

  // load fonts
  addFont(pdfDocument, fontList, PdfExportWidget::FontRole::Category);
  addFont(pdfDocument, fontList, PdfExportWidget::FontRole::Template);
  for (auto markIndex = 0; markIndex < markCount; ++markIndex)
  {
    addFont(pdfDocument, fontList, PdfExportWidget::FontRole::Mark, markIndex);
  }
}

void ExpPdf::on_widget_vocabularyGetMarks(QStringList *marks) const
{
  emit vocabularyGetMarks(marks);
}

void ExpPdf::pdfAddPage(HPDF_Doc pdfDocument, HPDF_Page *pdfPage, HPDF_REAL defaultSize /* 0 */) const
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
    pdfFont = Q_NULLPTR;
    pdfSize = defaultSize;
  }

  *pdfPage = HPDF_AddPage(pdfDocument);
  HPDF_Page_SetSize(*pdfPage, _widget->pageSize(), HPDF_PAGE_PORTRAIT);
  HPDF_Page_BeginText(*pdfPage);

  // horizontal and vertical border
  HPDF_Page_MoveTextPos(*pdfPage, _widget->border(), HPDF_Page_GetHeight(*pdfPage) - _widget->border() - pdfSize);

  // set previous font and size
  if (pdfFont)
  {
    pdfSetFont(*pdfPage, pdfFont, pdfSize);
  }
}

bool ExpPdf::pdfNextLine(HPDF_Doc pdfDocument, HPDF_Page *pdfPage) const
{
  // check current position
  const auto pdfSize = HPDF_Page_GetCurrentFontSize(*pdfPage);
  auto pdfPosition   = HPDF_Page_GetCurrentTextPos(*pdfPage);
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
  }
}

void ExpPdf::pdfSetFont(HPDF_Page pdfPage, HPDF_Font pdfFont, quintptr size) const
{
  HPDF_Page_SetFontAndSize(pdfPage, pdfFont, size);
  HPDF_Page_SetTextLeading(pdfPage, size);
}

void ExpPdf::pdfShowTableHeader(HPDF_Page pdfPage, const FontList &fontList) const
{
  const auto columns = _widget->tableColumns();
  for (const auto &column : *columns)
  {
    // write column header
    exportText(RECORD_NONE, pdfPage, fontList, QStringList(), column.headerEdit->text());

    // next column
    HPDF_Page_MoveTextPos(pdfPage, column.width->value(), 0);
  }
}

void ExpPdf::pdfShowText(HPDF_Page pdfPage, const QString &text, const QTextCodec *textCodec) const
{
  QByteArray encoded;
  if (textCodec)
  {
    encoded = textCodec->fromUnicode(text);
  }
  else
  {
    encoded = text.toLatin1();
  }

  HPDF_Page_ShowText(pdfPage, encoded);
}

void ExpPdf::beginExport() const
{
  // get filename
  const auto fileName = QFileDialog::getSaveFileName(_widget, QString(), QString(), tr("pdf (*.pdf)"));
  if (fileName.isEmpty())
  {
    return;
  }

  // marks
  QStringList marks;
  emit vocabularyGetMarks(&marks);

  // PDF
  const auto pdfDocument = HPDF_New(Q_NULLPTR, Q_NULLPTR);
  HPDF_SetCompressionMode(pdfDocument, _widget->compression());

  // get font info with font and encoding set
  QList<Font> fonts;
  initFonts(pdfDocument, &fonts, marks.size());

  // categories
  ExpInterface::CategoryIdList categoryIds;
  emit vocabularyGetCategoryIds(&categoryIds);

  // total record count for progress
  auto totalRecords = 0;
  for (auto categoryId : categoryIds)
  {
    quintptr records;
    emit vocabularyGetRecordCount(categoryId, &records);
    totalRecords += records;
  }
  emit progressExportSetMax(totalRecords);

  // export
  HPDF_Page pdfPage = Q_NULLPTR;
  pdfAddPage(pdfDocument, &pdfPage, fonts.at(static_cast<int>(PdfExportWidget::FontRole::Category)).size);
  auto firstLine = true;
  auto records = 0;
  for (auto categoryId : categoryIds)
  {
    if (firstLine)
    {
      firstLine = false;
    }
    else
    {
      auto newPage = pdfNextLine(pdfDocument, &pdfPage);
      if (!newPage)
      {
        newPage = pdfNextLine(pdfDocument, &pdfPage);
      }

      if (newPage)
      {
        // header
        pdfShowTableHeader(pdfPage, fonts);
      }
    }

    // category
    QString categoryName;
    emit vocabularyGetCategoryName(categoryId, &categoryName);
    pdfSetFont(pdfPage, fonts.at(static_cast<int>(PdfExportWidget::FontRole::Category)).pdfFont, fonts.at(static_cast<int>(PdfExportWidget::FontRole::Category)).size);
    pdfShowText(pdfPage, categoryName, fonts.at(static_cast<int>(PdfExportWidget::FontRole::Category)).textCodec);

    // header
    pdfNextLine(pdfDocument, &pdfPage);
    pdfShowTableHeader(pdfPage, fonts);

    // records
    ExpInterface::RecordIdList recordIds;
    emit vocabularyGetRecordIds(categoryId, &recordIds);
    for (auto recordId : recordIds)
    {
      if (pdfNextLine(pdfDocument, &pdfPage))
      {
        // header
        pdfShowTableHeader(pdfPage, fonts);
        pdfNextLine(pdfDocument, &pdfPage);
      }

      if (_widget->style() == PdfExportWidget::Style::Text)
      {
        exportText(recordId, pdfPage, fonts, marks, _widget->textTemplate());
      }
      else
      {
        exportTable(recordId, pdfPage, fonts, marks);
      }

      ++records;
      emit progressExportSetValue(records);
    }
  }

  HPDF_SaveToFile(pdfDocument, fileName.toLocal8Bit());

  emit progressExportSetValue(0);
  HPDF_Free(pdfDocument);
}

QString ExpPdf::pluginName() const
{
  return tr("Adobe Reader (pdf)");
}

void ExpPdf::setupUi(QWidget *parent)
{
  _widget        = new PdfExportWidget(parent);
  auto boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
  boxLayout->insertWidget(WIDGET_POSITION, _widget);

  connect(_widget, &PdfExportWidget::vocabularyGetMarks, this, &ExpPdf::on_widget_vocabularyGetMarks);

  _widget->initMarkFonts();
}