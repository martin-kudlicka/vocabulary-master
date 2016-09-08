#include "plaintextexportwidget.h"

#include <QtWidgets/QScrollBar>

PlaintextExportWidget::PlaintextExportWidget(QWidget *parent /* nullptr */, Qt::WindowFlags flags /* 0 */) : QWidget(parent, flags)
{
	_ui.setupUi(this);

	_ui.codecs->setModel(&_codecsModel);
	preselectCodec("UTF-8");
}

PlaintextExportWidget::~PlaintextExportWidget()
{
}

QString PlaintextExportWidget::codec() const
{
    const QModelIndex modelIndex = _ui.codecs->currentIndex();
    return _codecsModel.data(modelIndex).toString();
}

QString PlaintextExportWidget::text() const
{
    return _ui.plainPreview->toPlainText();
}

void PlaintextExportWidget::refresh() const
{
	_ui.plainPreview->clear();

	// categories
	ExpInterface::CategoryIdList categoryIds;
	emit vocabularyGetCategoryIds(&categoryIds);

	// total record count for progress
	quint32 totalRecords = 0;
	for (const quint8 &categoryId : categoryIds)
	{
		quint32 records;
		emit vocabularyGetRecordCount(categoryId, &records);
		totalRecords += records;
	}
	emit progressExportSetMax(totalRecords);

	QStringList marks;
	emit vocabularyGetMarks(&marks);

	// preview
	bool firstLine  = true;
	quint32 records = 0;
	for (const quint8 &categoryId : categoryIds)
	{
		if (firstLine)
		{
			firstLine = false;
		}
		else
		{
			_ui.plainPreview->appendPlainText("");
		}

		QString categoryName;
		emit vocabularyGetCategoryName(categoryId, &categoryName);
		_ui.plainPreview->appendPlainText(categoryName);

		// records
		ExpInterface::RecordIdList recordIds;
		emit vocabularyGetRecordIds(categoryId, &recordIds);
		for (const quint32 &recordId : recordIds)
		{
			QString templateText = _ui.plainEdit->text();

			// replace marks for data
			for (const QString &mark : marks)
			{
				QString data;
				emit vocabularyGetMarkText(recordId, mark, &data);
				templateText.replace(mark, data);
			}

			_ui.plainPreview->appendPlainText(templateText);

			records++;
			emit progressExportSetValue(records);
		}
	}

	_ui.plainPreview->verticalScrollBar()->setValue(0);

	emit progressExportSetValue(0);
}

void PlaintextExportWidget::preselectCodec(const QString &codec) const
{
	for (quint8 codecIndex = 0; codecIndex < _codecsModel.rowCount(); codecIndex++)
	{
		const QModelIndex modelIndex = _codecsModel.index(codecIndex, CodecsModel::ColumnCodec);
		if (codec == _codecsModel.data(modelIndex))
		{
			_ui.codecs->setCurrentIndex(modelIndex);
			return;
		}
	}
}

void PlaintextExportWidget::on_refresh_clicked(bool checked /* false */) const
{
    refresh();
}