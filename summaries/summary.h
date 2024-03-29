#ifndef SUMMARY_H
#define SUMMARY_H

#include <QStandardItemModel>
#include <QUuid>

#include "basedefines.h"
#include "summarydefines.h"
#include "metodicdefines.h"

class MultiFactor;
class MSExcelExporter;

/*!
 * \brief Класс сводки показателей The Summary class
 */
class Summary : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit Summary(QObject *parent = nullptr);

    /*!
     * \brief Структура, содержащая информацию об объединении ячеек The SpanCellsInfo struct
     */
    struct SpanCellsInfo
    {
        int row {0};
        int col {0};
        int width {0};
        SpanCellsInfo(int r, int c, int w) : row(r), col(c), width(w){}
        SpanCellsInfo();
    };

    /*!
     * \brief Строки заголовка сводки The SummaryRows enum
     */
    enum SummaryRows
    {
          RowProbes = 0
        , RowChannels
        , RowMultifactors
        , RowFactors
        , RowPrimaryFactors = 0
    };

    /*!
     * \brief Роли для доступа к элементам в модели сводок The SummaryRoles enum
     */
    enum SummaryRoles
    {
          SummaryUidRole = Qt::UserRole + 1   ///< uid сводки. Располагается в итеме [0, 0]. QString
        , SummaryKindRole                     ///< тип сводки. Располагается в итеме [0, 0]. int
        , MethodicUIdRole                     ///< uid методики. Располагается в итеме [0, 0]. QString
        , MethodicNameRole                    ///< название методики. Располагается в итеме [0, 0]. QString
        , VersionRole                         ///< номер версии сводки. Располагается в итеме [0, 0]. int
        , ProbeNumberRole                     ///< номер пробы. Располагается в итемах с названием проб на строке 0. QString
        , ProbeNameRole                       ///< название пробы. Располагается в итемах с названием проб на строке 0. QString
        , ChannelIdRole                       ///< идентификатор канала. Располагается в итемах с названием каналов на строке 1. QString
        , ChannelNameRole                     ///< название канала. Располагается в итемах с названием каналов на строке 1. QString
        , MultiFactorUidRole    ///< идентификатор группы показателей. Располагается в итемах с названием групп показателей на строке 2. QString
        , MultiFactorNameRole   ///< название группы показателей. Располагается в итемах с названием групп показателей на строке 2. QString
        , FactorUidRole         ///< идентификатор показателя. Располагается в итемах с названием показателя на строке 3. QString
        , FactorNameRole        ///< название показателя. Располагается в итемах с названием показателя на строке 3. QString
        , FactorShortNameRole   ///< короткое название показателя. Располагается в итемах с названием показателя на строке 3. QString
        , FactorMeasureRole     ///< единица измерения показателя. Располагается в итемах с названием показателя на строке 3. QString
        , FactorFormatRole      ///< формат показателя. Располагается в итемах с названием показателя на строке 3. int
        , FactorValueRole    ///< значение показателя. Располагается в итемах со значениями показателей по всей сводке на строках, начиная с 4. double
        , PatientFioRole        ///< ФИО пациента. Располагается в итемах первого столбца для тестов, занесенных в сводку. QString
        , TestDateTimeRole      ///< Дата и время теста. Располагается в итемах первого столбца для тестов, занесенных в сводку. QString
    };

    /*!
     * \brief Добавляет запись о тесте как строку в сводку
     * Если кол-во строк = 0, то добавляется и заголовок в виде трех строк
     * \param testUid - uid теста
     */
    void addTest(const QString &testUid);

    /*!
     * \brief Удаляет строку с тестом из сводки
     * \param row - строка теста в модели
     */
    void removeTest(const int row);

    /*!
     * \brief Возвращает заголовок сводки с ее параметрами, читая их из файла
     * \param fileName - имя файла сводки
     * \param name - имя сводки
     * \param uidMethodic - uid методики
     * \param kind - тип сводки
     */
    static void getHeader(const QString &fileName,
                          QString &name, QString &uidMethodic, QString &nameMethodic, SummaryDefines::Kind &kind);

    /*!
     * \brief Открывает сводку из файла
     * \param fileName - имя файла
     */
    void open(const QString &fileName);

    /*!
     * \brief Сохраняет сводку в файл
     * имя файла m_fileName
     */
    void save();

    /*!
     * \brief Сохраняет сводку в файл
     * \param fName - имя файла
     */
    void save(const QString& fName);

    /*!
     * \brief Сохраняет сводку в текстовый файл
     * \param fName - имя файла
     */
    void saveAsText(const QString& fName);

#ifdef Q_OS_WIN32
    /*!
     * \brief Открывает сводку в MS Excel
     */
    void openInMSExcel();
#endif

    /*!
     * \brief Доступ к m_uid
     */
    QString uid() const {return m_uid;}
    void setUid(const QString uid);

    /*!
     * \brief Доступ к m_fileName
     */
    QString fileName() const {return m_fileName;}
    void setFileName(const QString &fileName);

    /*!
     * \brief Доступ к m_name
     */
    QString name() const;
    void setName(const QString &name);

    /*!
     * \brief Доступ к m_uidMethodic
     */
    QString uidMethodic() const {return m_uidMethodic;}
    void setUidMethodic(const QString uid);

    /*!
     * \brief Доступ к m_kind
     */
    SummaryDefines::Kind kind() const {return m_kind;}
    void setKind(const SummaryDefines::Kind kind);

    /*!
     * \brief Доступ к списку ячеек, которые надо соединить
     */
    int spanCellsCount() const;
    SpanCellsInfo spanCells(const int idx) const;

    /*!
     * \brief Возвращает номер версии сводки
     */
    int version() const {return 1;}

    bool isModify() {return m_isModify;}

signals:
    void modify(const bool isModify);

private:
    /*!
     * \brief Добавляет запись о тесте как строку в сводку всех показателей
     * Если кол-во строк = 0, то добавляется и заголовок в виде трех строк
     * \param testUid - uid теста
     */
    void addTestAll(const QString &testUid);

    /*!
     * \brief Добавляет запись о тесте как строку в сводку первичных показателей
     * Если кол-во строк = 0, то добавляется и заголовок в виде трех строк
     * \param testUid - uid теста
     */
    void addTestPrimary(const QString &testUid);

    QString getMethodName(const QString& metUid);

    /*!
     * \brief Рассчитывает показатели для объекта теста (теста, пробы или канала)
     * \param testUid - uid теста
     * \param probeUid - uid пробы
     * \param channelId - uid канала
     * \param count - суммарное кол-во показателей
     * \return Список рассчитанных групп показателей для объекта
     */
    QList<MultiFactor*> calculateFactors(int& count, const QString &testUid, const QString &probeUid = "", const QString &channelId = "");

    /*!
     * \brief Добавление показателей рассчтанных показателей для объекта в модель
     * \param fgs - список рассчитанных групп показателей
     * \param level - уровень
     * \param line - заполняемый список итемов
     */
    void addCalculatedFactors(QList<MultiFactor*> &fgs, BaseDefines::TestLevel level,
                              QList<QStandardItem*> &line,
                              QList<QStandardItem*> &lineHdrProbes,
                              QList<QStandardItem*> &lineHdrChannels,
                              QList<QStandardItem*> &lineHdrMultiFactors,
                              QList<QStandardItem*> &lineHdrFactors);

    /*!
     * \brief Освобождает список групп показателей
     * \param fgs - список групп показателей
     */
    void doneFactors(QList<MultiFactor*> &fgs);

    /*!
     * \brief Создает итем и добавляет его в строку - список итемов для последующего занесения в модель
     * \param line - строка итемов
     * \param text - отображаемый текст итема
     * \return указатель на созданый итем. Потом в него можно записать всякие данные через роли
     */
    QStandardItem* createItem(QList<QStandardItem*>& line, const QString text);

    void addRootItem(QList<QStandardItem *> &line, MetodicDefines::MetodicInfo &mi);

    /*!
     * \brief Сортирует элементы таблицы - значения показателей в соотвествии с имеющимися показателями в модели
     * \param line - список несортированных показателей
     * \return список сортированных показателей
     */
    QList<QStandardItem*> sortItems(QList<QStandardItem*> &line);

    /*!
     * \brief Сохраняет сводку со всеми показателями
     * \param fName - имя файла
     */
    void saveAll(const QString &fName) const;
    /*!
     * \brief Сохраняет сводку с первичными показателями
     * \param fName - имя файла
     */
    void savePrimary(const QString &fName) const;

    /*!
     * \brief Читает сводку со всеми показателями
     * \param objSumm - объект, содержащий записи сводки
     */
    void readAll(QJsonObject& objSumm);

    /*!
     * \brief Читает сводку с первичными показателями
     * \param objSumm - объект, содержащий записи сводки
     */
    void readPrimary(QJsonObject& objSumm);

    void fillItemFactor(QJsonObject& cell, QStandardItem* item);
    void fillItemValue(const int col, QJsonObject& cell, QStandardItem* item);

    /*!
     * \brief Читает список соединяемых ячеек
     * \param objSumm - объект, содержащий записи сводки
     */
    void readSpanCells(QJsonObject& objSumm);

    void changeModyfy(const bool isModify);

    QString m_uid {QUuid().toString()};
    QString m_fileName {""};
    QString m_name {""};
    QString m_uidMethodic {QUuid().toString()};
    SummaryDefines::Kind m_kind {SummaryDefines::skNone};
    QList<SpanCellsInfo> m_spanCells;

    bool m_isModify {false};   ///< Признак того, что сводка была изменена
};

#endif // SUMMARY_H
