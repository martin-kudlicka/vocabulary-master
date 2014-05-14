#include "pdfexportwidget.h"

#include <hpdf_consts.h>

PdfExportWidget::EncodingInfo ENCODINGS[] = { {"StandardEncoding", PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, NULL},
												  {"MacRomanEncoding", PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "MacRoman"},
												  {"WinAnsiEncoding",  PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1252"},
												  {"FontSpecific",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, NULL},
												  {"ISO8859-2",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-2"},
												  {"ISO8859-3",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-3"},
												  {"ISO8859-4",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-4"},
												  {"ISO8859-5",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-5"},
												  {"ISO8859-6",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-6"},
												  {"ISO8859-7",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-7"},
												  {"ISO8859-8",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-8"},
												  {"ISO8859-9",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-9"},
												  {"ISO8859-10",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-10"},
												  {"ISO8859-11",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO 8859-11"},
												  {"ISO8859-13",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-13"},
												  {"ISO8859-14",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-14"},
												  {"ISO8859-15",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-15"},
												  {"ISO8859-16",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "ISO-8859-16"},
												  {"CP1250",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1250"},
												  {"CP1251",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1251"},
												  {"CP1252",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1252"},
												  {"CP1253",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1253"},
												  {"CP1254",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1254"},
												  {"CP1255",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1255"},
												  {"CP1256",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1256"},
												  {"CP1257",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1257"},
												  {"CP1258",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "CP1258"},
												  {"KOI8-R",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone, "KOI8-R"},
												  {"GB-EUC-H",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNS,  "GB2312"},
												  {"GB-EUC-V",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNS,  "GB2312"},
												  {"GBK-EUC-H",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNS,  "CP936"},
												  {"GBK-EUC-V",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNS,  "CP936"},
												  {"ETen-B5-H",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNT,  "CP950"},
												  {"ETen-B5-V",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNT,  "CP950"},
												  {"90ms-RKSJ-H",	   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE,  "Shift_JIS"},
												  {"90ms-RKSJ-V",	   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE,  "Shift_JIS"},
												  {"90msp-RKSJ-H",	   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE,  "Shift_JIS"},
												  {"EUC-H",			   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE,  "EUC-JP"},
												  {"EUC-V",			   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE,  "EUC-JP"},
												  {"KSC-EUC-H",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE,  "EUC-KR"},
												  {"KSC-EUC-V",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE,  "EUC-KR"},
												  {"KSCms-UHC-H",	   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE,  "cp949"},
												  {"KSCms-UHC-HW-H",   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE,  "cp949"},
												  {"KSCms-UHC-HW-V",   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE,  "cp949"} };

PdfExportWidget::FontInfo FONTS[] = { {"Courier",				PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Courier-Bold",			PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Courier-Oblique",		PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Courier-BoldOblique",	PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Helvetica",				PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Helvetica-Bold",		PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Helvetica-Oblique",		PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Helvetica-BoldOblique", PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Times-Roman",			PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Times-Bold",			PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Times-Italic",			PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Times-BoldItalic",		PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"Symbol",				PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"ZapfDingbats",			PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::FontSetNone},
										  {"SimSun",				PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetCNS},
										  {"SimHei",				PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetCNS},
										  {"MingLiU",				PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetCNT},
										  {"MS-Mincyo",				PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetJP},
										  {"MS-Gothic",				PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetJP},
										  {"MS-PMincyo",			PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetJP},
										  {"MS-PGothic",			PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetJP},
										  {"Batang",				PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetKR},
										  {"Dotum",					PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetKR},
										  {"BatangChe",				PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetKR},
										  {"DotumChe",				PdfExportWidget::EncodingTypeMultibyte,	 PdfExportWidget::FontSetKR} };

PdfExportWidget::PageSize PAGE_SIZES[] = { {"Letter",	HPDF_PAGE_SIZE_LETTER},
											  {"Legal",		HPDF_PAGE_SIZE_LEGAL},
											  {"A3",		HPDF_PAGE_SIZE_A3},
											  {"A4",		HPDF_PAGE_SIZE_A4},
											  {"A5",		HPDF_PAGE_SIZE_A5},
											  {"B4",		HPDF_PAGE_SIZE_B4},
											  {"B5",		HPDF_PAGE_SIZE_B5},
											  {"Executive", HPDF_PAGE_SIZE_EXECUTIVE},
											  {"US 4x6",	HPDF_PAGE_SIZE_US4x6},
											  {"US 4x8",	HPDF_PAGE_SIZE_US4x8},
											  {"US 5x7",	HPDF_PAGE_SIZE_US5x7},
											  {"Comm 10",	HPDF_PAGE_SIZE_COMM10} };

PdfExportWidget::PdfExportWidget(QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QWidget(parent, flags)
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

	connect(_ui.categoryFont, SIGNAL(currentIndexChanged(int)), SLOT(on_font_currentIndexChanged(int)));
	connect(_ui.templateFont, SIGNAL(currentIndexChanged(int)), SLOT(on_font_currentIndexChanged(int)));
} // PdfExportWidget

const quint8 PdfExportWidget::border() const
{
	return _ui.border->value();
} // border

const quint8 PdfExportWidget::compression() const
{
	quint8 compression;

	if (_ui.compressText->isChecked()) {
		compression = HPDF_COMP_TEXT;
	} else {
		compression = HPDF_COMP_NONE;
	} // if else
	if (_ui.compressImage->isChecked()) {
		compression |= HPDF_COMP_IMAGE;
	} // if
	if (_ui.compressMetadata->isChecked()) {
		compression |= HPDF_COMP_METADATA;
	} // if

	return compression;
} // compression

const PdfExportWidget::FontRoleInfo PdfExportWidget::fontRoleInfo(const FontRole &fontRole, const qint8 &num /* FONTROLE_NONE */) const
{
	FontRoleInfo fontRoleInfo;

	switch (fontRole)
	{
		case FontRoleCategory:
			fontRoleInfo.font     = _ui.categoryFont->currentText();
			fontRoleInfo.encoding = _ui.categoryEncoding->currentText();
			fontRoleInfo.size     = _ui.categoryFontSize->value();
			break;
		case FontRoleTemplate:
			fontRoleInfo.font     = _ui.templateFont->currentText();
			fontRoleInfo.encoding = _ui.templateEncoding->currentText();
			fontRoleInfo.size     = _ui.templateFontSize->value();
			break;
		case FontRoleMark:
			FontControls controls = _fontControls.at(num + DEFAULT_FONT_COUNT);

			fontRoleInfo.font     = controls.font->currentText();
			fontRoleInfo.encoding = controls.encoding->currentText();
			fontRoleInfo.size     = controls.size->value();
	} // switch

	fontRoleInfo.fontSet     = fontSet(fontRoleInfo.font);
	fontRoleInfo.encodingSet = encodingSet(fontRoleInfo.encoding);
	fontRoleInfo.textCodec   = textCodec(fontRoleInfo.encoding);

	return fontRoleInfo;
} // fontRoleInfo

const void PdfExportWidget::initMarkFonts()
{
	// mark fonts
	QStringList marks;
	emit vocabularyGetMarks(&marks);
	foreach (const QString &mark, marks)
	{
		// form label and field
		QLabel *markLabel        = new QLabel(mark + ':', _ui.font);
		QHBoxLayout *fontDetails = new QHBoxLayout(_ui.font);

		// font details
		QComboBox *font     = new QComboBox(_ui.font);
		QComboBox *encoding = new QComboBox(_ui.font);
		QSpinBox *fontSize  = new QSpinBox(_ui.font);

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

		connect(font, SIGNAL(currentIndexChanged(int)), SLOT(on_font_currentIndexChanged(int)));
	} // foreach
} // initMarkFonts

const HPDF_PageSizes PdfExportWidget::pageSize() const
{
	return static_cast<const HPDF_PageSizes>(_ui.pageSize->itemData(_ui.pageSize->currentIndex()).toInt());
} // pageSize

const PdfExportWidget::Style PdfExportWidget::style() const
{
	if (_ui.styleText->isChecked())
	{
		return StyleText;
	} else {
		return StyleTable;
	} // if else
} // style

const PdfExportWidget::TableColumns *PdfExportWidget::tableColumns() const
{
	return &_tableColumns;
} // tableColumns

const QString PdfExportWidget::textTemplate() const
{
	return _ui.textEdit->text();
} // textTemplate

const void PdfExportWidget::addTableColumn()
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
    QHBoxLayout *header = new QHBoxLayout(tableColumn.headerWidget);
    header->setContentsMargins(QMargins());
    header->addWidget(tableColumn.headerEdit);
    header->addWidget(tableColumn.width);
    _ui.tableColumns->addWidget(tableColumn.headerWidget, TableRowHeader, _tableColumns.size() + LABEL_COLUMN);
    // template
    _ui.tableColumns->addWidget(tableColumn.templateEdit, TableRowTemplate, _tableColumns.size() + LABEL_COLUMN);
} // addTableColumn

const void PdfExportWidget::fillEncodings(QComboBox *comboBox, const QString &font) const
{
	// get font encoding type
	EncodingType encoding;
	for (quint8 fontIndex = 0; fontIndex < sizeof(FONTS) / sizeof(FontInfo); fontIndex++)
	{
		if (FONTS[fontIndex].name == font)
		{
			encoding = FONTS[fontIndex].encodingType;
			break;
		} // if
	} // for

	comboBox->clear();
	for (quint8 encodingIndex = 0; encodingIndex < sizeof(ENCODINGS) / sizeof(EncodingInfo); encodingIndex++)
	{
		if (ENCODINGS[encodingIndex].encodingType == encoding)
		{
			comboBox->addItem(ENCODINGS[encodingIndex].name);
		} // if
	} // for
} // fillEncodings

const void PdfExportWidget::fillFonts(QComboBox *comboBox) const
{
	for (quint8 fontIndex = 0; fontIndex < sizeof(FONTS) / sizeof(FontInfo); fontIndex++)
	{
		comboBox->addItem(FONTS[fontIndex].name);
	} // for
} // fillFonts

const void PdfExportWidget::fillPageSizes() const
{
	for (quint8 sizeIndex = 0; sizeIndex < sizeof(PAGE_SIZES) / sizeof(PageSize); sizeIndex++)
	{
		_ui.pageSize->addItem(PAGE_SIZES[sizeIndex].name, PAGE_SIZES[sizeIndex].pdfPageSize);

		if (PAGE_SIZES[sizeIndex].pdfPageSize == HPDF_PAGE_SIZE_A4)
		{
			_ui.pageSize->setCurrentIndex(sizeIndex);
		} // if
	} // for
} // fillPageSizes

const PdfExportWidget::EncodingSet PdfExportWidget::encodingSet(const QString &encoding) const
{
	for (quint8 encodingIndex = 0; encodingIndex < sizeof(ENCODINGS) / sizeof(EncodingInfo); encodingIndex++)
	{
		if (ENCODINGS[encodingIndex].name == encoding)
		{
			return ENCODINGS[encodingIndex].encodingSet;
		} // if
	} // for

	return EncodingSetNone;
} // encodingSet

const PdfExportWidget::FontSet PdfExportWidget::fontSet(const QString &font) const
{
	for (quint8 fontIndex = 0; fontIndex < sizeof(FONTS) / sizeof(FontInfo); fontIndex++)
	{
		if (FONTS[fontIndex].name == font)
		{
			return FONTS[fontIndex].fontSet;
		} // if
	} // for

	return FontSetNone;
} // fontSet

const char *PdfExportWidget::textCodec(const QString &encoding) const
{
	for (quint8 encodingIndex = 0; encodingIndex < sizeof(ENCODINGS) / sizeof(EncodingInfo); encodingIndex++)
	{
		if (ENCODINGS[encodingIndex].name == encoding)
		{
			return ENCODINGS[encodingIndex].textCodec;
		} // if
	} // for

	return NULL;
} // textCodec

const void PdfExportWidget::initTableColumns()
{
    for (quint8 column = 0; column < _ui.tableColums->value(); column++)
	{
        addTableColumn();
    } // for
} // initTableColumns

const void PdfExportWidget::removeTableColumn()
{
	TableColumn tableColumn = _tableColumns.takeLast();
	// header
	tableColumn.headerEdit->deleteLater();
	tableColumn.width->deleteLater();
	tableColumn.headerWidget->deleteLater();
	// template
	tableColumn.templateEdit->deleteLater();
} // removeTableColumn

const void PdfExportWidget::on_font_currentIndexChanged(int index) const
{
	// get font controls
	FontControls controls;
	foreach (controls, _fontControls) {
		if (controls.font == sender()) {
			break;
		} // if
	} // foreach

	// remember old encoding selection
	const QString oldEncoding = controls.encoding->currentText();

	// fill new encodings
	fillEncodings(controls.encoding, controls.font->currentText());

	// try to select old encoding if possible
	const quint8 pos = controls.encoding->findText(oldEncoding);
	if (pos != -1)
	{
		controls.encoding->setCurrentIndex(pos);
	} // if
} // on_font_currentIndexChanged

const void PdfExportWidget::on_styleTable_clicked(bool checked /* false */) const
{
	_ui.styles->setCurrentIndex(StyleTable);
} // on_styleTable_clicked

const void PdfExportWidget::on_styleText_clicked(bool checked /* false */) const
{
    _ui.styles->setCurrentIndex(StyleText);
} // on_styleText_clicked

const void PdfExportWidget::on_tableColums_valueChanged(int i)
{
    if (i < _tableColumns.size())
	{
        removeTableColumn();
    }
	else
	{
        addTableColumn();
    } // if else
} // on_tableColums_valueChanged