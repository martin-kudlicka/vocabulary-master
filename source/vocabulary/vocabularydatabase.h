#ifndef VOCABULARYDATABASE_H
#define VOCABULARYDATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtCore/QList>
#include <QtCore/QHash>
#include "../plugins/common/tts-interface.h"

const auto VARIABLE_MARK = "$";

class VocabularyDatabase : public QObject
{
  Q_OBJECT

  public:
    using DataHash       = QHash<quintptr, QString>;
    using RecordDataHash = QHash<quintptr, DataHash>;
    using CategoryIdList = QList<quintptr>;
    using FieldIdList    = QList<quintptr>;

    enum class FieldAttribute
    {
      None,
      Speech,
      Show    = 0x2,
      BuiltIn = 0x4
    };
    Q_DECLARE_FLAGS(FieldAttributes, FieldAttribute)
    enum class FieldBuiltIn
    {
      None,
      Enabled,
      Priority
    };
    enum class FieldLanguage
    {
      Unknown,
      Left,
      Right,
      All
    };
    enum class FieldType
    {
      Unknown,
      LineEdit,
      CheckBox,
      SpinBox
    };
    enum class LanguageId
    {
      None,
      UserDefined,
      AllOnly,
      All = 0x4
    };
    Q_DECLARE_FLAGS(LanguageIds, LanguageId)

    using LanguageIdList = QList<FieldLanguage>;

    static const auto NOT_FOUND = -1;

             VocabularyDatabase(QObject *parent = Q_NULLPTR);
    virtual ~VocabularyDatabase() Q_DECL_OVERRIDE;

    quintptr                categoryCount              ()                                                         const;
    bool                    categoryEnabled            (quintptr categoryId)                                      const;
    quintptr                categoryId                 (quintptr row)                                             const;
    QString                 categoryName               (quintptr categoryId)                                      const;
    quintptr                categoryPriority           (quintptr categoryId)                                      const;
    bool                    isOpen()                                                                              const;
    LanguageIdList          languageIds                (LanguageIds type)                                         const;
    QString                 languageLearningTemplate   (FieldLanguage languageId)                                 const;
    QString                 languageName               (FieldLanguage languageId)                                 const;
    TTSInterface::TTSPlugin languageSpeech             (FieldLanguage languageId)                                 const;
    QString                 languageTrayTemplate       (FieldLanguage languageId)                                 const;
    QString                 languageVoice              (FieldLanguage languageId)                                 const;
    QString                 name                       ()                                                         const;
    quintptr                row                        (quintptr recordId, quintptr categoryId)                   const;
    quintptr                search                     (const QString &word, quintptr startId)                    const;
    void                    setCategoryEnabled         (quintptr categoryId, bool enabled)                        const;
    void                    setCategoryPriority        (quintptr categoryId, quintptr priority)                   const;
    void                    setLanguageLearningTemplate(FieldLanguage languageId, const QString &templateText)    const;
    void                    setLanguageName            (FieldLanguage languageId, const QString &name)            const;
    void                    setLanguageSpeech          (FieldLanguage languageId, TTSInterface::TTSPlugin speech) const;
    void                    setLanguageTrayTemplate    (FieldLanguage languageId, const QString &templateText)    const;
    void                    setLanguageVoice           (FieldLanguage languageId, const QString &voice)           const;
    void                    setSettings                (const QString &key, const QString &value)                 const;
    QString                 settings                   (const QString &key)                                       const;
    const QString          &vocabularyFile             ()                                                         const;

  protected:
    using RecordIdList = QList<quintptr>;

    quintptr        addCategory           (const QString &name)                                                      const;
    quintptr        addField              ()                                                                         const;
    quintptr        addRecord             (quintptr categoryId)                                                      const;
    quintptr        addRecord             (quintptr categoryId, const QStringList &data)                             const;
    void            beginEdit             ();
    void            close                 ();
    void            endEdit               (bool save = true);
    CategoryIdList  categoryIds           ()                                                                         const;
    QString         dataText              (quintptr categoryId, quintptr row, quintptr fieldId)                      const;
    QString         dataText              (quintptr recordId, quintptr fieldId)                                      const;
    RecordDataHash *dataText              () const;
    FieldAttributes fieldAttributes       (quintptr fieldId)                                                         const;
    FieldBuiltIn    fieldBuiltIn          (quintptr fieldId)                                                         const;
    quintptr        fieldCount            ()                                                                         const;
    quintptr        fieldId               (quintptr position)                                                        const;
    FieldIdList     fieldIds              ()                                                                         const;
    FieldLanguage   fieldLanguage         (quintptr fieldId)                                                         const;
    QString         fieldName             (quintptr fieldId)                                                         const;
    QString         fieldTemplateName     (quintptr fieldId)                                                         const;
    FieldType       fieldType             (quintptr fieldId)                                                         const;
    void            new2                  (const QString &filePath);
    quintptr        recordCategory        (quintptr recordId)                                                        const;
    quintptr        recordCount           ()                                                                         const;
    quintptr        recordCount           (quintptr categoryId)                                                      const;
    quintptr        recordCount           (quintptr categoryId, bool enabled)                                        const;
    quintptr        recordCount           (bool enabled)                                                             const;
    quintptr        recordId              (quintptr row)                                                             const;
    quintptr        recordId              (quintptr categoryId, quintptr row)                                        const;
    RecordIdList    recordIds             (quintptr categoryId)                                                      const;
    void            open                  (const QString &filePath);
    void            removeCategory        (quintptr categoryId)                                                      const;
    void            removeField           (quintptr fieldId)                                                         const;
    void            removeRecord          (quintptr categoryId, quintptr row)                                        const;
    void            setDataText           (quintptr categoryId, quintptr row, quintptr fieldId, const QString &data) const;
    void            setDataText           (quintptr recordId, quintptr fieldId, const QString &data)                 const;
    void            setFieldAttributes    (quintptr fieldId, FieldAttributes attributes)                             const;
    void            setFieldLanguage      (quintptr fieldId, FieldLanguage language)                                 const;
    void            setFieldName          (quintptr fieldId, const QString &name)                                    const;
    void            setFieldTemplateName  (quintptr fieldId, const QString &templateName)                            const;
    void            setRecordByRowCategory(quintptr oldCategoryId, quintptr recordRow, quintptr newCategoryId)       const;
    void            SetRecordCategory     (quintptr recordId, quintptr categoryId)                                   const;
    void            swapFields            (quintptr sourceId, quintptr destinationId)                                const;

  private:
    enum class ColumnPosition
    {
      N1,
      N2,
      N3
    };
    enum class Version
    {
      N1,
      N2,
    };

    QSqlDatabase _database;
    QString      _vocabularyFile;

    quintptr     addField      (const QString &templateText, const QString &name, const FieldType &type, FieldAttributes attributes, FieldBuiltIn builtIn, FieldLanguage language) const;
    void         addLanguage   (const QString &name, const QString &learningTemplate, const QString &trayTemplate, TTSInterface::TTSPlugin ttsPlugin, const QString &voice)        const;
    void         closeDatabase ();
    RecordIdList recordIds     ()                                                                                                                                                  const;
    void         initialize    ()                                                                                                                                                  const;
    void         openDatabase  ();
    void         removeRecord  (quintptr recordId)                                                                                                                                 const;
    void         update        (const QString &table, quintptr columnId, const QSqlRecord &record)                                                                                 const;
    void         updateDatabase();

  Q_SIGNALS:
    void setOpenProgressValue(quintptr value)      const;
    void setOpenProgressMax  (quintptr max)        const;
    void setVocabularyName   (const QString &name) const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::FieldAttributes)
Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::LanguageIds)

#endif