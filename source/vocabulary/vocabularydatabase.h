#ifndef VOCABULARYDATABASE_H
#define VOCABULARYDATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtCore/QList>
#include <QtCore/QHash>
#ifdef EDITION_FREE
# include <QtCore/QObject>
#else
# include "../plugins/common/tts-interface.h"
#endif

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
#ifndef EDITION_FREE
      Speech,
#endif
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
#ifndef EDITION_FREE
    enum class LanguageId
    {
      None,
      UserDefined,
      AllOnly,
      All = 0x4
    };
    Q_DECLARE_FLAGS(LanguageIds, LanguageId)
#endif

    using LanguageIdList = QList<FieldLanguage>;

    static const auto NOT_FOUND = -1;

             VocabularyDatabase(QObject *parent = Q_NULLPTR);
    virtual ~VocabularyDatabase() Q_DECL_OVERRIDE;

#ifndef EDITION_FREE
    quintptr                categoryCount              ()                                                         const;
    bool                    categoryEnabled            (quintptr categoryId)                                      const;
    quintptr                categoryId                 (quintptr row)                                             const;
#endif
    QString                 categoryName               (quintptr categoryId)                                      const;
#ifndef EDITION_FREE
    quintptr                categoryPriority           (quintptr categoryId)                                      const;
    bool                    isOpen()                                                                              const;
    LanguageIdList          languageIds                (LanguageIds type)                                         const;
#endif
    QString                 languageLearningTemplate   (FieldLanguage languageId)                                 const;
    QString                 languageName               (FieldLanguage languageId)                                 const;
#ifndef EDITION_FREE
    TTSInterface::TTSPlugin languageSpeech             (FieldLanguage languageId)                                 const;
    QString                 languageTrayTemplate       (FieldLanguage languageId)                                 const;
    QString                 languageVoice              (FieldLanguage languageId)                                 const;
#endif
    QString                 name                       ()                                                         const;
    quintptr                row                        (quintptr recordId, quintptr categoryId)                   const;
    quintptr                search                     (const QString &word, quintptr startId)                    const;
#ifndef EDITION_FREE
    void                    setCategoryEnabled         (quintptr categoryId, bool enabled)                        const;
    void                    setCategoryPriority        (quintptr categoryId, quintptr priority)                   const;
    void                    setLanguageLearningTemplate(FieldLanguage languageId, const QString &templateText)    const;
#endif
    void                    setLanguageName            (FieldLanguage languageId, const QString &name)            const;
#ifndef EDITION_FREE
    void                    setLanguageSpeech          (FieldLanguage languageId, TTSInterface::TTSPlugin speech) const;
    void                    setLanguageTrayTemplate    (FieldLanguage languageId, const QString &templateText)    const;
    void                    setLanguageVoice           (FieldLanguage languageId, const QString &voice)           const;
#endif
    void                    setSettings                (const QString &key, const QString &value)                 const;
    QString                 settings                   (const QString &key)                                       const;
#ifndef EDITION_TRY
    const QString          &vocabularyFile             ()                                                         const;
#endif

  protected:
    using RecordIdList = QList<quintptr>;

    quintptr        addCategory           (const QString &name)                                                      const;
#ifndef EDITION_FREE
    quintptr        addField              ()                                                                         const;
#endif
    quintptr        addRecord             (quintptr categoryId)                                                      const;
#ifndef EDITION_FREE
    quintptr        addRecord             (quintptr categoryId, const QStringList &data)                             const;
#endif
    void            beginEdit             ();
    void            close                 ();
    void            endEdit               (bool save = true);
    CategoryIdList  categoryIds           ()                                                                         const;
    QString         dataText              (quintptr categoryId, quintptr row, quintptr fieldId)                      const;
    QString         dataText              (quintptr recordId, quintptr fieldId)                                      const;
    RecordDataHash *dataText              () const;
    FieldAttributes fieldAttributes       (quintptr fieldId)                                                         const;
#ifndef EDITION_FREE
    FieldBuiltIn    fieldBuiltIn          (quintptr fieldId)                                                         const;
    quintptr        fieldCount            ()                                                                         const;
#endif
    quintptr        fieldId               (quintptr position)                                                        const;
    FieldIdList     fieldIds              ()                                                                         const;
    FieldLanguage   fieldLanguage         (quintptr fieldId)                                                         const;
    QString         fieldName             (quintptr fieldId)                                                         const;
    QString         fieldTemplateName     (quintptr fieldId)                                                         const;
    FieldType       fieldType             (quintptr fieldId)                                                         const;
    void            new2                  (
#ifndef EDITION_TRY
                                           const QString &filePath
#endif
                                          );
    quintptr        recordCategory        (quintptr recordId)                                                        const;
    quintptr        recordCount           ()                                                                         const;
    quintptr        recordCount           (quintptr categoryId)                                                      const;
#ifndef EDITION_FREE
    quintptr        recordCount           (quintptr categoryId, bool enabled)                                        const;
#endif
    quintptr        recordCount           (bool enabled)                                                             const;
    quintptr        recordId              (quintptr row)                                                             const;
    quintptr        recordId              (quintptr categoryId, quintptr row)                                        const;
    RecordIdList    recordIds             (quintptr categoryId)                                                      const;
#ifndef EDITION_TRY
    void            open                  (const QString &filePath);
#endif
    void            removeCategory        (quintptr categoryId)                                                      const;
#ifndef EDITION_FREE
    void            removeField           (quintptr fieldId)                                                         const;
#endif
    void            removeRecord          (quintptr categoryId, quintptr row)                                        const;
    void            setDataText           (quintptr categoryId, quintptr row, quintptr fieldId, const QString &data) const;
    void            setDataText           (quintptr recordId, quintptr fieldId, const QString &data)                 const;
    void            setFieldAttributes    (quintptr fieldId, FieldAttributes attributes)                             const;
#ifndef EDITION_FREE
    void            setFieldLanguage      (quintptr fieldId, FieldLanguage language)                                 const;
    void            setFieldName          (quintptr fieldId, const QString &name)                                    const;
    void            setFieldTemplateName  (quintptr fieldId, const QString &templateName)                            const;
    void            setRecordByRowCategory(quintptr oldCategoryId, quintptr recordRow, quintptr newCategoryId)       const;
    void            SetRecordCategory     (quintptr recordId, quintptr categoryId)                                   const;
    void            swapFields            (quintptr sourceId, quintptr destinationId)                                const;
#endif

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

  #ifdef EDITION_TRY
    static quintptr     _memoryVocabularies;
           quintptr     _vocabularyNumber;
  #endif
           QSqlDatabase _database;
  #ifndef EDITION_TRY
           QString      _vocabularyFile;
  #endif

    quintptr     addField      (const QString &templateText, const QString &name, const FieldType &type, FieldAttributes attributes, FieldBuiltIn builtIn, FieldLanguage language) const;
    void         addLanguage   (const QString &name, const QString &learningTemplate
  #ifndef EDITION_FREE
                                , const QString &trayTemplate, TTSInterface::TTSPlugin ttsPlugin, const QString &voice
  #endif
                                )                                                                                                                                                  const;
    void         closeDatabase ();
  #ifndef EDITION_TRY
    RecordIdList recordIds     ()                                                                                                                                                  const;
  #endif
    void         initialize    ()                                                                                                                                                  const;
    void         openDatabase  ();
    void         removeRecord  (quint32 recordId)                                                                                                                                  const;
  #ifndef EDITION_FREE
    void         update        (const QString &table, quintptr columnId, const QSqlRecord &record)                                                                                 const;
  #endif
  #ifndef EDITION_TRY
    void         updateDatabase();
  #endif

  Q_SIGNALS:
    void setOpenProgressValue(quintptr value)      const;
    void setOpenProgressMax  (quintptr max)        const;
    void setVocabularyName   (const QString &name) const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::FieldAttributes)
#ifndef EDITION_FREE
Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::LanguageIds)
#endif

#endif