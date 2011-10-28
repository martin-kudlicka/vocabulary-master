#include "pdfexportwidget.h"

PdfExportWidget::sEncodingInfo seiEncodings[] = { {"StandardEncoding", PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"MacRomanEncoding", PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"WinAnsiEncoding",  PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"FontSpecific",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-2",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-3",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-4",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-5",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-6",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-7",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-8",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-9",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-10",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-11",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-13",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-14",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-15",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"ISO8859-16",	   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1250",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1251",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1252",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1253",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1254",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1255",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1256",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1257",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"CP1258",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"KOI8-R",		   PdfExportWidget::EncodingTypeSinglebyte, PdfExportWidget::EncodingSetNone},
												  {"GB-EUC-H",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNS},
												  {"GB-EUC-V",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNS},
												  {"GBK-EUC-H",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNS},
												  {"GBK-EUC-V",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNS},
												  {"ETen-B5-H",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNT},
												  {"ETen-B5-V",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetCNT},
												  {"90ms-RKSJ-H",	   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE},
												  {"90ms-RKSJ-V",	   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE},
												  {"90msp-RKSJ-H",	   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE},
												  {"EUC-H",			   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE},
												  {"EUC-V",			   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetJPE},
												  {"KSC-EUC-H",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE},
												  {"KSC-EUC-V",		   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE},
												  {"KSCms-UHC-H",	   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE},
												  {"KSCms-UHC-HW-H",   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE},
												  {"KSCms-UHC-HW-V",   PdfExportWidget::EncodingTypeMultibyte,	PdfExportWidget::EncodingSetKRE} };

PdfExportWidget::sFontInfo sfiFonts[] = { {"Courier",				PdfExportWidget::EncodingTypeSinglebyte},
										  {"Courier-Bold",			PdfExportWidget::EncodingTypeSinglebyte},
										  {"Courier-Oblique",		PdfExportWidget::EncodingTypeSinglebyte},
										  {"Courier-BoldOblique",	PdfExportWidget::EncodingTypeSinglebyte},
										  {"Helvetica",				PdfExportWidget::EncodingTypeSinglebyte},
										  {"Helvetica-Bold",		PdfExportWidget::EncodingTypeSinglebyte},
										  {"Helvetica-Oblique",		PdfExportWidget::EncodingTypeSinglebyte},
										  {"Helvetica-BoldOblique", PdfExportWidget::EncodingTypeSinglebyte},
										  {"Times-Roman",			PdfExportWidget::EncodingTypeSinglebyte},
										  {"Times-Bold",			PdfExportWidget::EncodingTypeSinglebyte},
										  {"Times-Italic",			PdfExportWidget::EncodingTypeSinglebyte},
										  {"Times-BoldItalic",		PdfExportWidget::EncodingTypeSinglebyte},
										  {"Symbol",				PdfExportWidget::EncodingTypeSinglebyte},
										  {"ZapfDingbats",			PdfExportWidget::EncodingTypeSinglebyte},
										  {"SimSun",				PdfExportWidget::EncodingTypeMultibyte},
										  {"SimHei",				PdfExportWidget::EncodingTypeMultibyte},
										  {"MingLiU",				PdfExportWidget::EncodingTypeMultibyte},
										  {"MS-Mincyo",				PdfExportWidget::EncodingTypeMultibyte},
										  {"MS-Gothic",				PdfExportWidget::EncodingTypeMultibyte},
										  {"MS-PMincyo",			PdfExportWidget::EncodingTypeMultibyte},
										  {"MS-PGothic",			PdfExportWidget::EncodingTypeMultibyte},
										  {"Batang",				PdfExportWidget::EncodingTypeMultibyte},
										  {"Dotum",					PdfExportWidget::EncodingTypeMultibyte},
										  {"BatangChe",				PdfExportWidget::EncodingTypeMultibyte},
										  {"DotumChe",				PdfExportWidget::EncodingTypeMultibyte} };

const void PdfExportWidget::AddTableColumn()
{
    // controls
    sTableColumn stcColumn;
    stcColumn.qleHeader = new QLineEdit(_qwpePdfExport.qwPageTable);
    stcColumn.qleTemplate = new QLineEdit(_qwpePdfExport.qwPageTable);
    stcColumn.qsbWidth = new QSpinBox(_qwpePdfExport.qwPageTable);
    stcColumn.qsbWidth->setMaximum(COLUMN_MAX_WIDTH);
    stcColumn.qsbWidth->setValue(COLUMN_DEFAULTWIDTH);
    stcColumn.qwHeader = new QWidget(_qwpePdfExport.qwPageTable);
    _qlTableColumns.append(stcColumn);

    // header
    QHBoxLayout *qhblHeader = new QHBoxLayout(stcColumn.qwHeader);
    qhblHeader->setContentsMargins(QMargins());
    qhblHeader->addWidget(stcColumn.qleHeader);
    qhblHeader->addWidget(stcColumn.qsbWidth);
    _qwpePdfExport.qglTableColumns->addWidget(stcColumn.qwHeader, TableRowHeader, _qlTableColumns.size() + LABEL_COLUMN);
    // template
    _qwpePdfExport.qglTableColumns->addWidget(stcColumn.qleTemplate, TableRowTemplate, _qlTableColumns.size() + LABEL_COLUMN);
} // AddTableColumn

const void PdfExportWidget::FillEncodings(QComboBox *pComboBox, const QString &pFont) const
{
	// get font encoding type
	eEncodingType eetEncoding;
	for (int iFont = 0; iFont < sizeof(sfiFonts) / sizeof(sFontInfo); iFont++) {
		if (sfiFonts[iFont].qsName == pFont) {
			eetEncoding = sfiFonts[iFont].eetEncoding;
			break;
		} // if
	} // for

	pComboBox->clear();
	for (int iEncoding = 0; iEncoding < sizeof(seiEncodings) / sizeof(sEncodingInfo); iEncoding++) {
		if (seiEncodings[iEncoding].eetEncodingType == eetEncoding) {
			pComboBox->addItem(seiEncodings[iEncoding].qsName);
		} // if
	} // for
} // FillEncodings

const void PdfExportWidget::FillFonts(QComboBox *pComboBox) const
{
	for (int iFont = 0; iFont < sizeof(sfiFonts) / sizeof(sFontInfo); iFont++) {
		pComboBox->addItem(sfiFonts[iFont].qsName);
	} // for
} // FillFonts

const PdfExportWidget::eEncodingSet PdfExportWidget::GetEncodingSet(const QString &pEncoding) const
{
	for (int iEncoding = 0; iEncoding < sizeof(seiEncodings) / sizeof(sEncodingInfo); iEncoding++) {
		if (seiEncodings[iEncoding].qsName == pEncoding) {
			return seiEncodings[iEncoding].eesEncodingSet;
		} // if
	} // for

	return EncodingSetNone;
} // GetEncodingSet

PdfExportWidget::sFontRoleInfo PdfExportWidget::GetFontInfo(const eFontRole &pRole, const int &pNum /* -1 */) const
{
	sFontRoleInfo sfriFontInfo;

	switch (pRole) {
		case FontRoleCategory:
			sfriFontInfo.qsFont = _qwpePdfExport.qcbCategoryFont->currentText();
			sfriFontInfo.qsEncoding = _qwpePdfExport.qcbCategoryEncoding->currentText();
			sfriFontInfo.iSize = _qwpePdfExport.qsbCategoryFontSize->value();
			break;
		case FontRoleTemplate:
			sfriFontInfo.qsFont = _qwpePdfExport.qcbTemplateFont->currentText();
			sfriFontInfo.qsEncoding = _qwpePdfExport.qcbTemplateEncoding->currentText();
			sfriFontInfo.iSize = _qwpePdfExport.qsbTemplateFontSize->value();
			break;
		case FontRoleMark:
			sFontControls sfcControls = _qlFontControls.at(pNum + DEFAULT_FONT_COUNT);

			sfriFontInfo.qsFont = sfcControls.qcbFont->currentText();
			sfriFontInfo.qsEncoding = sfcControls.qcbEncoding->currentText();
			sfriFontInfo.iSize = sfcControls.qsbSize->value();
	} // switch
	sfriFontInfo.eesEncodingSet = GetEncodingSet(sfriFontInfo.qsEncoding);

	return sfriFontInfo;
} // GetFontInfo

const QString PdfExportWidget::GetTextTemplate() const
{
	return _qwpePdfExport.qleTextEdit->text();
} // GetTextTemplate

const void PdfExportWidget::InitMarkFonts()
{
	// mark fonts
	QStringList qslMarks;
	emit VocabularyGetMarks(&qslMarks);
	foreach (QString qsMark, qslMarks) {
		// form label and field
		QLabel *qlMark = new QLabel(qsMark + ':', _qwpePdfExport.qgbFont);
		QHBoxLayout *qhblFontDetails = new QHBoxLayout(_qwpePdfExport.qgbFont);

		// font details
		QComboBox *qcbFont = new QComboBox(_qwpePdfExport.qgbFont);
		QComboBox *qcbEncoding = new QComboBox(_qwpePdfExport.qgbFont);
		QSpinBox *qsbFontSize = new QSpinBox(_qwpePdfExport.qgbFont);

		qhblFontDetails->addWidget(qcbFont);
		qhblFontDetails->addWidget(qcbEncoding);
		qhblFontDetails->addWidget(qsbFontSize);

		FillFonts(qcbFont);
		FillEncodings(qcbEncoding, qcbFont->currentText());
		qsbFontSize->setMinimum(_qwpePdfExport.qsbTemplateFontSize->minimum());
		qsbFontSize->setValue(_qwpePdfExport.qsbTemplateFontSize->value());
		qsbFontSize->setSuffix(_qwpePdfExport.qsbTemplateFontSize->suffix());

		// add new row to form
		_qwpePdfExport.qflFontLayout->addRow(qlMark, qhblFontDetails);

		// font controls
		sFontControls sfcControls;
		sfcControls.qcbFont = qcbFont;
		sfcControls.qcbEncoding = qcbEncoding;
		sfcControls.qsbSize = qsbFontSize;
		_qlFontControls.append(sfcControls);

		connect(qcbFont, SIGNAL(currentIndexChanged(int)), SLOT(on_qcbFont_currentIndexChanged(int)));
	} // foreach
} // InitMarkFonts

const void PdfExportWidget::InitTableColumns()
{
    for (int iColumn = 0; iColumn < _qwpePdfExport.qsbTableColums->value(); iColumn++) {
        AddTableColumn();
    } // for
} // InitTableColumns

const void PdfExportWidget::on_qcbFont_currentIndexChanged(int index) const
{
	// get font controls
	sFontControls sfcControls;
	foreach (sfcControls, _qlFontControls) {
		if (sfcControls.qcbFont == sender()) {
			break;
		} // if
	} // foreach

	// remember old encoding selection
	QString qsOldEncoding = sfcControls.qcbEncoding->currentText();

	// fill new encodings
	FillEncodings(sfcControls.qcbEncoding, sfcControls.qcbFont->currentText());

	// try to select old encoding if possible
	int iPos = sfcControls.qcbEncoding->findText(qsOldEncoding);
	if (iPos != -1) {
		sfcControls.qcbEncoding->setCurrentIndex(iPos);
	} // if
} // on_qcbFont_currentIndexChanged

const void PdfExportWidget::on_qrbStyleTable_clicked(bool checked /* false */) const
{
	_qwpePdfExport.qswStyles->setCurrentIndex(StyleTable);
} // on_qrbStyleTable_clicked

const void PdfExportWidget::on_qrbStyleText_clicked(bool checked /* false */) const
{
    _qwpePdfExport.qswStyles->setCurrentIndex(StyleText);
} // on_qrbStyleText_clicked

const void PdfExportWidget::on_qsbTableColums_valueChanged(int i)
{
    if (i < _qlTableColumns.size()) {
        RemoveTableColumn();
    } else {
        AddTableColumn();
    } // if else
} // on_qsbTableColums_valueChanged

PdfExportWidget::PdfExportWidget(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags)
{
    _qwpePdfExport.setupUi(this);

	FillFonts(_qwpePdfExport.qcbCategoryFont);
	FillEncodings(_qwpePdfExport.qcbCategoryEncoding, _qwpePdfExport.qcbCategoryFont->currentText());
	FillFonts(_qwpePdfExport.qcbTemplateFont);
	FillEncodings(_qwpePdfExport.qcbTemplateEncoding, _qwpePdfExport.qcbTemplateFont->currentText());
    InitTableColumns();

	// font controls
	sFontControls sfcControls;
	sfcControls.qcbFont = _qwpePdfExport.qcbCategoryFont;
	sfcControls.qcbEncoding = _qwpePdfExport.qcbCategoryEncoding;
	sfcControls.qsbSize = _qwpePdfExport.qsbCategoryFontSize;
	_qlFontControls.append(sfcControls);
	sfcControls.qcbFont = _qwpePdfExport.qcbTemplateFont;
	sfcControls.qcbEncoding = _qwpePdfExport.qcbTemplateEncoding;
	sfcControls.qsbSize = _qwpePdfExport.qsbTemplateFontSize;
	_qlFontControls.append(sfcControls);

	connect(_qwpePdfExport.qcbCategoryFont, SIGNAL(currentIndexChanged(int)), SLOT(on_qcbFont_currentIndexChanged(int)));
	connect(_qwpePdfExport.qcbTemplateFont, SIGNAL(currentIndexChanged(int)), SLOT(on_qcbFont_currentIndexChanged(int)));
} // PdfExportWidget

const void PdfExportWidget::RemoveTableColumn()
{
    sTableColumn stcColumn = _qlTableColumns.takeLast();
    // header
    stcColumn.qleHeader->deleteLater();
    stcColumn.qsbWidth->deleteLater();
    stcColumn.qwHeader->deleteLater();
    // template
    stcColumn.qleTemplate->deleteLater();
} // RemoveTableColumn