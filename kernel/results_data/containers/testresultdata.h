#ifndef TESTRESULTDATA_H
#define TESTRESULTDATA_H

#include <QList>
#include <QDateTime>
#include <QJsonObject>

class ProbeResultData;
class SignalData;
class ProbeResultInfo;

/*!
 * \brief Класс, хранящий в себе результаты тестирования TestResultData class
 * Используется как при записи сигналов, так и при получении их из базы для обработки.
 */
class TestResultData
{
public:
    explicit TestResultData();

    ///< Режим работы с результатами
    enum Mode
    {
          mdUndefined = 0  ///< Неопределен
        , mdRun            ///< Проведение теста
        , mdHandle         ///< Обработка результатов
    };

    /*!
     * \brief Начинает проведение нового теста.
     * Если mode == undefined, то устанавливает mode в run иначе сбой
     * \param patient - uid пациента
     * \param metodicName - uid методики
     */
    void newTest(const QString &patientUid, const QString &metodicUid);

    /*!
     * \brief Начинает новую пробу, начиная со второй. Первая начинается по newTest()
     * Если mode != run, то сбой
     * \param probeName - название пробы
     */
    void newProbe(const QString &probeName);

    /*!
     * \brief Закрывает тест для записи. Сохраняет результаты в БД.
     * Если mode != run, то сбой.
     * После выполнения mode == undefined, данные освобождены.
     */
    void saveTest();

    /*!
     * \brief Открывает тест для обработки.
     * Если mode == undefined, то устанавливает mode в handle, иначе сбой
     * \param uid - уникальный идентификатор теста
     */
    void openTest(const QString &uid);

    /*!
     * \brief Закрывает тест, устанавливает mode = undefined.
     * Если mode == run, то сброс буферов без сохранения в БД.
     * Если mode == handle, то сброс буферов.
     */
    void closeTest();

    /*!
     * \brief Добавляет сигнал к данным пробы
     * Если mode != run, то сбой
     *
     * Stabilogram* stb = new Stabilogram(chanId);
     * testRes->addSignal(stb);
     * stb->addValue(rec);
     * ...
     *
     * \param signal - добавляемый сигнал
     */
    void addSignal(SignalData *signal);

    /*!
     * \brief Возвращает кол-во проб
     */
    int probesCount() const;

    ProbeResultInfo* probe(const int number) const;

    /*!
     * \brief Возвращает параметры на момент проведения
     * mode == handle, иначе сбой
     */
    QJsonObject getParams() const;

private:
   Mode m_mode;

   QString m_uid;
   QString m_patientUid;
   QString m_metodicUid;
   QDateTime m_dt;
   QJsonObject m_params;

   QList<ProbeResultData*>  m_probes;
};

#endif // TESTRESULTDATA_H
