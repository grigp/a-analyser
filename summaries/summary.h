#ifndef SUMMARY_H
#define SUMMARY_H

#include <QStandardItemModel>

#include "basedefines.h"

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


private:
    QString m_fileName {""};

    QString getMethodName(const QString& metUid);

    /*!
     * \brief Рассчитывает показатели для объекта теста (теста, пробы или канала)
     * \param testUid - uid теста
     * \param probeUid - uid пробы
     * \param channelId - uid канала
     * \return Список рассчитанных групп показателей для объекта
     */
    QList<MultiFactor*> calculateFactors(const QString &testUid, const QString &probeUid = "", const QString &channelId = "");

    /*!
     * \brief Добавление показателей рассчтанных показателей для объекта в модель
     * \param fgs - список рассчитанных групп показателей
     * \param level - уровень
     */
    void addCalculatedFactors(QList<MultiFactor*> fgs, BaseDefines::TestLevel level);

};

#endif // SUMMARY_H
