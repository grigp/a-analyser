#ifndef SUMMARY_H
#define SUMMARY_H

#include <QStandardItemModel>

#include "basedefines.h"
#include "summarydefines.h"

class MultiFactor;

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
     * \brief Добавляет запись о тесте как строку в сводку
     * Если кол-во строк = 0, то добавляется и заголовок в виде трех строк
     * \param testUid - uid теста
     */
    void addTest(const QString &testUid);

    /*!
     * \brief Открывает сводку из файла
     * \param fileName - имя файла
     */
    void open(const QString &fileName);

    /*!
     * \brief Сохраняет сводку в файл
     * имя файла m_fileName
     */
    void save() const;

    /*!
     * \brief Доступ к m_fileName
     */
    QString fileName() const {return m_fileName;}
    void setFileName(const QString &fileName) {m_fileName = fileName;}

    /*!
     * \brief Доступ к m_name
     */
    QString name() const;
    void setName(const QString &name);

    /*!
     * \brief Доступ к m_kind
     */
    SummaryDefines::Kind kind() const {return m_kind;}
    void setKind(const SummaryDefines::Kind kind) {m_kind = kind;}

    /*!
     * \brief Соступ к списку ячеек, которые надо соединить
     */
    int spanCellsCount() const;
    SpanCellsInfo spanCells(const int idx) const;

private:
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

    QString m_fileName {""};
    QString m_name {""};
    SummaryDefines::Kind m_kind {SummaryDefines::skNone};
    QList<SpanCellsInfo> m_spanCells;

};

#endif // SUMMARY_H
