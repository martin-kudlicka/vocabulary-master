#include "pdfexportwidget.h"

#include <hpdf_consts.h>

const auto DEFAULT_FONT_COUNT  = 2;
const auto COLUMN_DEFAULTWIDTH = 100;
const auto COLUMN_MAX_WIDTH    = 999;
const auto LABEL_COLUMN        = 1;

PdfExportWidget::EncodingInfo ENCODINGS[] = { { "StandardEncoding", PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, Q_NULLPTR },
                                              { "MacRomanEncoding", PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "MacRoman" },
                                              { "WinAnsiEncoding",  PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1252" },
                                              { "FontSpecific",     PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, Q_NULLPTR },
                                              { "ISO8859-2",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-2" },
                                              { "ISO8859-3",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-3" },
                                              { "ISO8859-4",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-4" },
                                              { "ISO8859-5",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-5" },
                                              { "ISO8859-6",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-6" },
                                              { "ISO8859-7",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-7" },
                                              { "ISO8859-8",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-8" },
                                              { "ISO8859-9",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-9" },
                                              { "ISO8859-10",       PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-10" },
                                              { "ISO8859-11",       PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO 8859-11" },
                                              { "ISO8859-13",       PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-13" },
                                              { "ISO8859-14",       PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-14" },
                                              { "ISO8859-15",       PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-15" },
                                              { "ISO8859-16",       PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "ISO-8859-16" },
                                              { "CP1250",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1250" },
                                              { "CP1251",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1251" },
                                              { "CP1252",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1252" },
                                              { "CP1253",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1253" },
                                              { "CP1254",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1254" },
                                              { "CP1255",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1255" },
                                              { "CP1256",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1256" },
                                              { "CP1257",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1257" },
                                              { "CP1258",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "CP1258" },
                                              { "KOI8-R",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::EncodingSet::None, "KOI8-R" },
                                              { "GB-EUC-H",         PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::CNS,  "GB2312" },
                                              { "GB-EUC-V",         PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::CNS,  "GB2312" },
                                              { "GBK-EUC-H",        PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::CNS,  "CP936" },
                                              { "GBK-EUC-V",        PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::CNS,  "CP936" },
                                              { "ETen-B5-H",        PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::CNT,  "CP950" },
                                              { "ETen-B5-V",        PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::CNT,  "CP950" },
                                              { "90ms-RKSJ-H",      PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::JPE,  "Shift_JIS" },
                                              { "90ms-RKSJ-V",      PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::JPE,  "Shift_JIS" },
                                              { "90msp-RKSJ-H",     PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::JPE,  "Shift_JIS" },
                                              { "EUC-H",            PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::JPE,  "EUC-JP" },
                                              { "EUC-V",            PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::JPE,  "EUC-JP" },
                                              { "KSC-EUC-H",        PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::KRE,  "EUC-KR" },
                                              { "KSC-EUC-V",        PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::KRE,  "EUC-KR" },
                                              { "KSCms-UHC-H",      PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::KRE,  "cp949" },
                                              { "KSCms-UHC-HW-H",   PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::KRE,  "cp949" },
                                              { "KSCms-UHC-HW-V",   PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::EncodingSet::KRE,  "cp949" } };

PdfExportWidget::FontInfo FONTS[] = { { "Courier",               PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Courier-Bold",          PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Courier-Oblique",       PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Courier-BoldOblique",   PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Helvetica",             PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Helvetica-Bold",        PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Helvetica-Oblique",     PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Helvetica-BoldOblique", PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Times-Roman",           PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Times-Bold",            PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Times-Italic",          PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Times-BoldItalic",      PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "Symbol",                PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "ZapfDingbats",          PdfExportWidget::EncodingType::Singlebyte, PdfExportWidget::FontSet::None },
                                      { "SimSun",                PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::CNS },
                                      { "SimHei",                PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::CNS },
                                      { "MingLiU",               PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::CNT },
                                      { "MS-Mincyo",             PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::JP },
                                      { "MS-Gothic",             PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::JP },
                                      { "MS-PMincyo",            PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::JP },
                                      { "MS-PGothic",            PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::JP },
                                      { "Batang",                PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::KR },
                                      { "Dotum",                 PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::KR },
                                      { "BatangChe",             PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::KR },
                                      { "DotumChe",              PdfExportWidget::EncodingType::Multibyte,  PdfExportWidget::FontSet::KR } };

PdfExportWidget::PageSize PAGE_SIZES[] = { { "Letter",    HPDF_PAGE_SIZE_LETTER },
                                           { "Legal",     HPDF_PAGE_SIZE_LEGAL },
                                           { "A3",        HPDF_PAGE_SIZE_A3 },
                                           { "A4",        HPDF_PAGE_SIZE_A4 },
                                           { "A5",        HPDF_PAGE_SIZE_A5 },
                                           { "B4",        HPDF_PAGE_SIZE_B4 },
                                           { "B5",        HPDF_PAGE_SIZE_B5 },
                                           { "Executive", HPDF_PAGE_SIZE_EXECUTIVE },
                                           { "US 4x6",    HPDF_PAGE_SIZE_US4x6 },
                                           { "US 4x8",    HPDF_PAGE_SIZE_US4x8 },
                                           { "US 5x7",    HPDF_PAGE_SIZE_US5x7 },
                                           { "Comm 10",   HPDF_PAGE_SIZE_COMM10 } };

PdfExportWidget::PdfExportWidget(QWidget *parent) : QWidget(parent)
{
  _ui.setupUi(this);

  fillFonts(_ui.categoryFont);
  fillEncodings(_ui.categoryEncoding, _ui.categoryFont->currentText());
  fillFonts(_ui.templateFont);
  fillEncodings(_ui.templateEncoding, _ui.templateFont->currentText());
  fillPageSizes();
  initTableColumns();

  // font controls
  FontControls controls;
  controls.font     = _ui.categoryFont;
  controls.encoding = _ui.categoryEncoding;
  controls.size     = _ui.categoryFontSize;
  _fontControls.append(controls);
  controls.font     = _ui.templateFont;
  controls.encoding = _ui.templateEncoding;
  controls.size     = _ui.templateFontSize;
  _fontControls.append(controls);

  connect(_ui.categoryFont, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PdfExportWidget::on_font_currentIndexChanged);
  connect(_ui.templateFont, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PdfExportWidget::on_font_currentIndexChanged);
}

quintptr PdfExportWidget::border() const
{
  return _ui.border->value();
}

quintptr PdfExportWidget::compression() const
{
  quintptr compression2;

  if (_ui.compressText->isChecked())
  {
    compression2 = HPDF_COMP_TEXT;
  }
  else
  {
    compression2 = HPDF_COMP_NONE;
  }
  if (_ui.compressImage->isChecked())
  {
    compression2 |= HPDF_COMP_IMAGE;
  }
  if (_ui.compressMetadata->isChecked())
  {
    compression2 |= HPDF_COMP_METADATA;
  }

  return compression2;
}

PdfExportWidget::FontRoleInfo PdfExportWidget::fontRoleInfo(FontRole fontRole, qintptr num /* FONTROLE_NONE */) const
{
  FontRoleInfo fontRoleInfo2;

  switch (fontRole)
  {
    case FontRole::Category:
      fontRoleInfo2.font     = _ui.categoryFont->currentText();
      fontRoleInfo2.encoding = _ui.categoryEncoding->currentText();
      fontRoleInfo2.size     = _ui.categoryFontSize->value();
      break;
    case FontRole::Template:
      fontRoleInfo2.font     = _ui.templateFont->currentText();
      fontRoleInfo2.encoding = _ui.templateEncoding->currentText();
      fontRoleInfo2.size     = _ui.templateFontSize->value();
      break;
    case FontRole::Mark:
      auto controls = _fontControls.at(num + DEFAULT_FONT_COUNT);

      fontRoleInfo2.font     = controls.font->currentText();
      fontRoleInfo2.encoding = controls.encoding->currentText();
      fontRoleInfo2.size     = controls.size->value();
  }

  fontRoleInfo2.fontSet     = fontSet(fontRoleInfo2.font);
  fontRoleInfo2.encodingSet = encodingSet(fontRoleInfo2.encoding);
  fontRoleInfo2.textCodec   = textCodec(fontRoleInfo2.encoding);

  return fontRoleInfo2;
}

void PdfExportWidget::initMarkFonts()
{
  // mark fonts
  QStringList marks;
  emit vocabularyGetMarks(&marks);
  for (const auto &mark : marks)
  {
    // form label and field
    auto markLabel   = new QLabel(mark + ':', _ui.font);
    auto fontDetails = new QHBoxLayout(_ui.font);

    // font details
    auto font     = new QComboBox(_ui.font);
    auto encoding = new QComboBox(_ui.font);
    auto fontSize = new QSpinBox(_ui.font);

    fontDetails->addWidget(font);
    fontDetails->addWidget(encoding);
    fontDetails->addWidget(fontSize);

    fillFonts(font);
    fillEncodings(encoding, font->currentText());
    fontSize->setMinimum(_ui.templateFontSize->minimum());
    fontSize->setValue(_ui.templateFontSize->value());
    fontSize->setSuffix(_ui.templateFontSize->suffix());

    // add new row to form
    _ui.fontLayout->addRow(markLabel, fontDetails);

    // font controls
    FontControls controls;
    controls.font     = font;
    controls.encoding = encoding;
    controls.size     = fontSize;
    _fontControls.append(controls);

    connect(font, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PdfExportWidget::on_font_currentIndexChanged);
  }
}

HPDF_PageSizes PdfExportWidget::pageSize() const
{
  return static_cast<const HPDF_PageSizes>(_ui.pageSize->itemData(_ui.pageSize->currentIndex()).toInt());
}

PdfExportWidget::Style PdfExportWidget::style() const
{
  if (_ui.styleText->isChecked())
  {
    return Style::Text;
  }
  else
  {
    return Style::Table;
  }
}

const PdfExportWidget::TableColumns *PdfExportWidget::tableColumns() const
{
  return &_tableColumns;
}

QString PdfExportWidget::textTemplate() const
{
  return _ui.textEdit->text();
}

void PdfExportWidget::addTableColumn()
{
  // controls
  TableColumn tableColumn;
  tableColumn.headerEdit   = new QLineEdit(_ui.pageTable);
  tableColumn.templateEdit = new QLineEdit(_ui.pageTable);
  tableColumn.width        = new QSpinBox(_ui.pageTable);
  tableColumn.width->setMaximum(COLUMN_MAX_WIDTH);
  tableColumn.width->setValue(COLUMN_DEFAULTWIDTH);
  tableColumn.headerWidget = new QWidget(_ui.pageTable);
  _tableColumns.append(tableColumn);

  // header
  auto header = new QHBoxLayout(tableColumn.headerWidget);
  header->setContentsMargins(QMargins());
  header->addWidget(tableColumn.headerEdit);
  header->addWidget(tableColumn.width);
  _ui.tableColumns->addWidget(tableColumn.headerWidget, static_cast<int>(TableRow::Header), _tableColumns.count() + LABEL_COLUMN);
  // template
  _ui.tableColumns->addWidget(tableColumn.templateEdit, static_cast<int>(TableRow::Template), _tableColumns.count() + LABEL_COLUMN);
}

void PdfExportWidget::fillEncodings(QComboBox *comboBox, const QString &font) const
{
  // get font encoding type
  EncodingType encoding = EncodingType::Singlebyte;
  for (auto fontIndex = 0; fontIndex < _countof(FONTS); ++fontIndex)
  {
    if (FONTS[fontIndex].name == font)
    {
      encoding = FONTS[fontIndex].encodingType;
      break;
    }
  }

  comboBox->clear();
  for (auto encodingIndex = 0; encodingIndex < _countof(ENCODINGS); ++encodingIndex)
  {
    if (ENCODINGS[encodingIndex].encodingType == encoding)
    {
      comboBox->addItem(ENCODINGS[encodingIndex].name);
    }
  }
}

void PdfExportWidget::fillFonts(QComboBox *comboBox) const
{
  for (auto fontIndex = 0; fontIndex < _countof(FONTS); ++fontIndex)
  {
    comboBox->addItem(FONTS[fontIndex].name);
  }
}

void PdfExportWidget::fillPageSizes() const
{
  for (auto sizeIndex = 0; sizeIndex < _countof(PAGE_SIZES); ++sizeIndex)
  {
    _ui.pageSize->addItem(PAGE_SIZES[sizeIndex].name, PAGE_SIZES[sizeIndex].pdfPageSize);

    if (PAGE_SIZES[sizeIndex].pdfPageSize == HPDF_PAGE_SIZE_A4)
    {
      _ui.pageSize->setCurrentIndex(sizeIndex);
    }
  }
}

PdfExportWidget::EncodingSet PdfExportWidget::encodingSet(const QString &encoding) const
{
  for (auto encodingIndex = 0; encodingIndex < _countof(ENCODINGS); ++encodingIndex)
  {
    if (ENCODINGS[encodingIndex].name == encoding)
    {
      return ENCODINGS[encodingIndex].encodingSet;
    }
  }

  return EncodingSet::None;
}

PdfExportWidget::FontSet PdfExportWidget::fontSet(const QString &font) const
{
  for (auto fontIndex = 0; fontIndex < _countof(FONTS); ++fontIndex)
  {
    if (FONTS[fontIndex].name == font)
    {
      return FONTS[fontIndex].fontSet;
    }
  }

  return FontSet::None;
}

char *PdfExportWidget::textCodec(const QString &encoding) const
{
  for (auto encodingIndex = 0; encodingIndex < _countof(ENCODINGS); ++encodingIndex)
  {
    if (ENCODINGS[encodingIndex].name == encoding)
    {
      return ENCODINGS[encodingIndex].textCodec;
    }
  }

  return Q_NULLPTR;
}

void PdfExportWidget::initTableColumns()
{
  for (auto column = 0; column < _ui.tableColums->value(); ++column)
  {
    addTableColumn();
  }
}

void PdfExportWidget::removeTableColumn()
{
  auto tableColumn = _tableColumns.takeLast();
  // header
  tableColumn.headerEdit->deleteLater();
  tableColumn.width->deleteLater();
  tableColumn.headerWidget->deleteLater();
  // template
  tableColumn.templateEdit->deleteLater();
}

void PdfExportWidget::on_font_currentIndexChanged(int index) const
{
  Q_UNUSED(index);

  // get font controls
  FontControls controls = {};
  foreach(controls, _fontControls)
  {
    if (controls.font == sender())
    {
      break;
    }
  }

  // remember old encoding selection
  const auto oldEncoding = controls.encoding->currentText();

  // fill new encodings
  fillEncodings(controls.encoding, controls.font->currentText());

  // try to select old encoding if possible
  const auto pos = controls.encoding->findText(oldEncoding);
  if (pos != -1)
  {
    controls.encoding->setCurrentIndex(pos);
  }
}

void PdfExportWidget::on_styleTable_clicked(bool checked /* false */) const
{
  Q_UNUSED(checked);

  _ui.styles->setCurrentIndex(static_cast<int>(Style::Table));
}

void PdfExportWidget::on_styleText_clicked(bool checked /* false */) const
{
  Q_UNUSED(checked);

  _ui.styles->setCurrentIndex(static_cast<int>(Style::Text));
}

void PdfExportWidget::on_tableColums_valueChanged(int i)
{
  if (i < _tableColumns.count())
  {
    removeTableColumn();
  }
  else
  {
    addTableColumn();
  }
}