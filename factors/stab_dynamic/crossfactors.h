#ifndef CROSSFACTORS_H
#define CROSSFACTORS_H

#include <QObject>

#include "baseutils.h"
#include "multifactor.h"
#include "signalsdefines.h"
#include "crossdefines.h"

namespace CrossFactorsDefines
{
static const QString GroupUid = "{6072DE41-F36D-4E87-9C34-10CD3F896E1B}";
static const QString LUpUid = "{7817AA0C-D311-422E-AFC6-89B01B39D24F}";
static const QString LDnUid = "{F85C7159-FD3B-4DE7-8FF9-2698B8F367E9}";
static const QString LRtUid = "{EE01B0CE-DFB5-4DC1-A899-221D3895E4D4}";
static const QString LLfUid = "{39AD698B-FE0F-4F64-BCDA-F10C4E04F28B}";
static const QString SZoneUid = "{23FF25C3-91B4-4C07-96D5-DA540670406C}";
static const QString UDUid = "{0E81D341-A33C-4262-B82D-FC23B4871B6D}";
static const QString RLUid = "{69CB9D08-8A58-42AA-9726-0700A2C01F5B}";
static const QString SFUid = "{6613B36C-EDD1-4E1D-B405-E94D6843FFC7}";
}

class CrossResultData;

/*!
 * \brief Класс показателей теста на устойчивость CrossFactors class
 */
class CrossFactors : public ProbeMultifactor
{
public:
    explicit CrossFactors(const QString &testUid,
                          const QString &probeUid,
                          QObject *parent = nullptr);

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * uid теста задается в конструкторе
     */
    bool isValid()  const override;
    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * Аналог для вызова без создания экземпляра объекта
     */
    static bool isValid(const QString &testUid,
                        const QString &probeUid);

    /*!
     * \brief Рассчитывает показатели для теста с заданным uid
     * uid теста задается в конструкторе
     */
    void calculate() override;

    /*!
     * \brief Регистрирует факторсгруппу и показатели
     */
    static void registerFactors();

    int diap() const ;

private:
    /*!
     * \brief Получает список событийных меток
     */
    void getEventLablels();
    /*!
     * \brief Заполняет буферы сигналов
     */
    void fillBuffers();

    /*!
     * \brief Рассчитывает амплитуду отклонения в одну из сторон
     * \param buffer - массив усредненных данных отклонения
     * \param direction - направление
     * \return отклонение в миллиметрах
     */
    int computeDistance(QList<SignalsDefines::StabRec> &buffer, const BaseUtils::Directions direction);

    CrossResultData *m_crdata {nullptr};  ///< Сырые данные меток на сигнале

    ///< Буфера сигнала
    QList<SignalsDefines::StabRec> m_bufferUp;
    QList<SignalsDefines::StabRec> m_bufferDn;
    QList<SignalsDefines::StabRec> m_bufferLf;
    QList<SignalsDefines::StabRec> m_bufferRt;

    double m_LUp {0};
    double m_LDn {0};
    double m_LLf {0};
    double m_LRt {0};
    double m_SZone {0};
    double m_UD {0};
    double m_RL {0};
    double m_SF {0};

};

#endif // CROSSFACTORS_H
