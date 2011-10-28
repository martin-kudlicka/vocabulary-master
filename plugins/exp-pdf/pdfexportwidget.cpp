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

const void PdfExportWidget::FillFonts(QComboBox *pComboBox) const
{
	for (int iFont = 0; iFont < sizeof(sfiFonts) / sizeof(sFontInfo); iFont++) {
		pComboBox->addItem(sfiFonts[iFont].qsName);
	} // for
} // FillFonts

PdfExportWidget::sFontRoleInfo PdfExportWidget::GetFontInfo(const eFontRole &pRole, const int &pNum /* -1 */) const
{
	sFontRoleInfo sfriFontInfo;

	switch (pRole) {
		case FontRoleCategory:
			sfriFontInfo.qsFont = _qwpePdfExport.qcbCategoryFont->currentText();
			sfriFontInfo.iSize = _qwpePdfExport.qsbCategoryFontSize->value();
			break;
		case FontRoleTemplate:
			sfriFontInfo.qsFont = _qwpePdfExport.qcbTemplateFont->currentText();
			sfriFontInfo.iSize = _qwpePdfExport.qsbTemplateFontSize->value();
			break;
		case FontRoleMark:
			// get correct mark row in form layout
			QLayoutItem *qliItem = _qwpePdfExport.qflFontLayout->itemAt(pNum + DEFAULT_FONT_COUNT, QFormLayout::FieldRole);
			const QHBoxLayout *qhblFontDetails = qobject_cast<const QHBoxLayout *>(qliItem->layout());

			// font
			QLayoutItem *qliFont = qhblFontDetails->itemAt(FontDetailsFont);
			const QComboBox *qcbFont = qobject_cast<const QComboBox *>(qliFont->widget());
			sfriFontInfo.qsFont = qcbFont->currentText();

			// size
			QLayoutItem *qliFontSize = qhblFontDetails->itemAt(FontDetailsSize);
			const QSpinBox *qsbFontSize = qobject_cast<const QSpinBox *>(qliFontSize->widget());
			sfriFontInfo.iSize = qsbFontSize->value();
	} // switch

	return sfriFontInfo;
} // GetFontInfo

const QString PdfExportWidget::GetTextTemplate() const
{
	return _qwpePdfExport.qleTextEdit->text();
} // GetTextTemplate

const void PdfExportWidget::InitMarkFonts() const
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
		qsbFontSize->setMinimum(_qwpePdfExport.qsbTemplateFontSize->minimum());
		qsbFontSize->setValue(_qwpePdfExport.qsbTemplateFontSize->value());
		qsbFontSize->setSuffix(_qwpePdfExport.qsbTemplateFontSize->suffix());

		// add new row to form
		_qwpePdfExport.qflFontLayout->addRow(qlMark, qhblFontDetails);
	} // foreach
} // InitMarkFonts

const void PdfExportWidget::InitTableColumns()
{
    for (int iColumn = 0; iColumn < _qwpePdfExport.qsbTableColums->value(); iColumn++) {
        AddTableColumn();
    } // for
} // InitTableColumns

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
	FillFonts(_qwpePdfExport.qcbTemplateFont);
    InitTableColumns();
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