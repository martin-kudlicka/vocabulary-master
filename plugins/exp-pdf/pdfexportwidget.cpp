#include "pdfexportwidget.h"

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
	// remember selection
	QString qsCurrent = pComboBox->currentText();

	pComboBox->clear();
	pComboBox->addItems(QStringList() << "Courier" << "Courier-Bold" << "Courier-Oblique" << "Courier-BoldOblique" << "Helvetica" << "Helvetica-Bold" << "Helvetica-Oblique" << "Helvetica-BoldOblique" << "Times-Roman" << "Times-Bold" << "Times-Italic" << "Times-BoldItalic" << "Symbol" << "ZapfDingbats");

	// try to select previous font
	int iIndex = pComboBox->findText(qsCurrent);
	if (iIndex == -1) {
		iIndex = 0;
	} // if
	pComboBox->setCurrentIndex(iIndex);
} // FillFonts

const QString PdfExportWidget::GetCategoryFont() const
{
	return _qwpePdfExport.qcbCategoryFont->currentText();
} // GetCategoryFont

const int PdfExportWidget::GetCategoryFontSize() const
{
	return _qwpePdfExport.qsbCategoryFontSize->value();
} // GetCategoryFontSize

const QString PdfExportWidget::GetRecordFont() const
{
	return _qwpePdfExport.qcbRecordFont->currentText();
} // GetRecordFont

const int PdfExportWidget::GetRecordFontSize() const
{
	return _qwpePdfExport.qsbRecordFontSize->value();
} // GetRecordFontSize

const QString PdfExportWidget::GetTextTemplate() const
{
	return _qwpePdfExport.qleTextEdit->text();
} // GetTextTemplate

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
	FillFonts(_qwpePdfExport.qcbRecordFont);
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