#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "vocabulary/vocabularydatabase.h"

class Settings;

class Vocabulary : public VocabularyDatabase
{
  public:
             Vocabulary(const Settings *settings);
    virtual ~Vocabulary() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    quintptr        addCategory           (const QString &name);
    void            addField              ();
    void            addRecord             (quintptr categoryId);
    void            addRecord             (quintptr categoryId, const QStringList &data);
    void            beginEdit             ();
    void            close                 ();
    void            endEdit               (bool save = true);
    bool            fieldHasAttribute     (quintptr fieldId, FieldAttribute attribute)          const;
    CategoryIdList  categoryIds           ()                                                    const;
    QString         dataText              (quintptr categoryId, quintptr row, quintptr fieldId) const;
    QString         dataText              (quintptr recordId, quintptr fieldId)                 const;
    FieldAttributes fieldAttributes       (quintptr fieldId)                                    const;
    FieldBuiltIn    fieldBuiltIn          (quintptr fieldId)                                    const;
    quintptr        fieldCount            ()                                                    const;
    quintptr        fieldId               (quintptr position)                                   const;
    FieldIdList     fieldIds              ()                                                    const;
    FieldLanguage   fieldLanguage         (quintptr fieldId)                                    const;
    QString         fieldName             (quintptr fieldId)                                    const;
    QString         fieldTemplateName     (quintptr fieldId)                                    const;
    FieldType       fieldType             (quintptr fieldId)                                    const;
    QStringList     record                (quintptr recordId)                                   const;
    quintptr        recordCategory        (quintptr recordId)                                   const;
    quintptr        recordCount           ()                                                    const;
    quintptr        recordCount           (quintptr categoryId)                                 const;
    quintptr        recordCount           (quintptr categoryId, bool enabled)                   const;
    quintptr        recordCount           (bool enabled)                                        const;
    bool            recordEnabled         (quintptr recordId)                                   const;
    quintptr        recordId              (quintptr row)                                        const;
    quintptr        recordId              (quintptr categoryId, quintptr row)                   const;
    RecordIdList    recordIds             (quintptr categoryId)                                 const;
    void            new2                  (const QString &filePath);
    void            open                  (const QString &filePath);
    void            removeCategory        (quintptr categoryId);
    void            removeField           (quintptr fieldId);
    void            removeRecord          (quintptr categoryId, quintptr row);
    void            setDataText           (quintptr categoryId, quintptr row, quintptr fieldId, const QString &data);
    void            setDataText           (quintptr recordId, quintptr fieldId, const QString &data);
    void            setFieldAttributes    (quintptr fieldId, FieldAttributes attributes);
    void            setFieldLanguage      (quintptr fieldId, FieldLanguage language);
    void            setFieldName          (quintptr fieldId, const QString &name);
    void            setFieldTemplateName  (quintptr fieldId, const QString &templateName);
    void            setRecordByRowCategory(quintptr oldCategoryId, quintptr recordRow, quintptr newCategoryId);
    void            swapFields            (quintptr sourceId, quintptr destinationId);

  private:
    struct FieldData
    {
      QString         templateName;
      QString         name;
      FieldType       type;
      FieldAttributes attributes;
      FieldBuiltIn    builtIn;
      FieldLanguage   language;
    };

    using CategoryRecordsHash = QHash<quintptr, RecordIdList>;
    using FieldDataHash       = QHash<quintptr, FieldData>;

          bool                _cacheEnabled;
          CategoryRecordsHash _categoryRecords;
          FieldDataHash       _fieldData;
          FieldDataHash       _fieldDataBackup;
          RecordDataHash     *_recordData;
          RecordDataHash      _recordDataBackup;
    const Settings           *_settings;

    void      clearCache();
    FieldData fieldData (quintptr fieldId) const;
    void      initCache ();
};

#endif