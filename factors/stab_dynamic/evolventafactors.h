#ifndef EVOLVENTAFACTORS_H
#define EVOLVENTAFACTORS_H

#include <QObject>

#include "baseutils.h"
#include "multifactor.h"
#include "signalsdefines.h"
#include "evolventadefines.h"

namespace EvolventaFactorsDefines
{

static const QString GroupUid = "{8A2DA8DD-7560-498A-9AB7-7B2DC4614343}";

namespace Frontal {
static const QString SummErr = "{8DE01E39-D491-4113-A01D-9A5367C6EC2C}";   ///< Суммарная ошибка
static const QString MidErr = "{4907F954-76E6-48D7-A0DF-F256FCE9D47C}";   ///< Средняя ошибка
static const QString MidErrE = "{66AE00BD-E4E2-4383-905B-164229C0CB88}";   ///< Средняя ошибка на экстремуме
static const QString MidErrB = "{EF4B2F65-3CD9-4C92-A389-7B0FEEA2FC23}";   ///< Средняя ошибка на переходе
static const QString CrsCntE = "{0B5B98C6-DFC5-4003-9FB3-0B35A05824EE}";   ///< Сред. кол-во пересечений на экстремуме
static const QString CrsCntB = "{C3F29A31-29D6-4218-8FC6-56CA1D670AA3}";   ///< Сред. кол-во пересечений на переходе

static const QString ExtrCntE = "{8A59D4EF-2BBE-42B0-8E4B-177DC20DE9B3}";   ///< Среднее кол-во пиков на экстремуме
static const QString ExtrCntB = "{A5932391-F6DC-4FB6-804B-86F1B2920C49}";   ///< Среднее кол-во пиков на переходе
static const QString MidAmplE = "{1FE0F3A3-D406-45F5-A12D-88009C696A47}";   ///< Средняя амплитуда между пиками на экстремуме
static const QString MidAmplB = "{6FB2F58E-6B9C-436C-9246-EAED464C5518}";   ///< Средняя амплитуда между пиками на переходе

static const QString CMidErrE = "{97D24A27-7766-4C8E-BF2E-5FA49538257B}";   ///< Изменение средней ошибки на экстремуме
static const QString CMidAmpE = "{507F4217-7513-4576-A8E2-1B636AAC8FED}";   ///< Изменение средней амплитуды на экстремуме

// R - retention
static const QString InterCntR = "{F1D6C5A6-81FF-417A-BD42-848ABEDD7E95}";   ///< Кол-во пересечений траектории цели при удержании
static const QString InterLenR = "{88BA04FB-0B70-4503-91D5-9655A357C318}";   ///< Длит. между пересечениями траектории цели при удержании

static const QString SemiWavLen  = "{01FD6473-AC99-4A58-AF16-1F51063F4A19}";   ///< Длительность полуволны
static const QString SemiWavAmpl = "{22BCF336-BC9E-4571-A4ED-A0C16EB555D6}";   ///< Амплитуда полуволны
}

namespace Sagittal {
static const QString SummErr = "{5150A24B-1FD8-4EBD-BC73-233353B64912}";   ///< Суммарная ошибка
static const QString MidErr = "{28579B65-D5EF-4E7C-9CB1-CCE9ED61FF3F}";   ///< Средняя ошибка
static const QString MidErrE = "{D0DDAF96-647C-496F-BC7C-F7E53829B98C}";   ///< Средняя ошибка на экстремуме
static const QString MidErrB = "{72B54237-EEB0-4A39-B2DE-7D5D7CF78140}";   ///< Средняя ошибка на переходе
static const QString CrsCntE = "{DABE21CD-7CE9-4344-8A48-DDA70E82F908}";   ///< Сред. кол-во пересечений на экстремуме
static const QString CrsCntB = "{174549A7-A0E0-4108-9D10-4FC91631E6B7}";   ///< Сред. кол-во пересечений на переходе

static const QString ExtrCntE = "{76743F36-A4CD-4D0F-846D-30317776BAA4}";   ///< Среднее кол-во пиков на экстремуме
static const QString ExtrCntB = "{1E6BC893-7223-45F4-9DB6-FC02D9AEF8DC}";   ///< Среднее кол-во пиков на переходе
static const QString MidAmplE = "{24AEDFD8-ACEB-48F8-A73F-54DF6E07B7E0}";   ///< Средняя амплитуда между пиками на экстремуме
static const QString MidAmplB = "{39F90FCD-BB94-4276-BCD6-C61E3A154F9A}";   ///< Средняя амплитуда между пиками на переходе

static const QString CMidErrE = "{F9755F6B-A19F-487F-BE01-9853A5CD7326}";   ///< Изменение средней ошибки на экстремуме
static const QString CMidAmpE = "{3912D71C-BBDC-4358-843D-34EDAADF2E6E}";   ///< Изменение средней амплитуды на экстремуме

static const QString InterCntR = "{04A95915-99B2-41CE-91FB-11C8927C760B}";   ///< Кол-во пересечений траектории цели при удержании
static const QString InterLenR = "{4A21FD78-BDD3-48EB-B59B-4833085F4748}";   ///< Длит. между пересечениями траектории цели при удержании

static const QString SemiWavLen  = "{EC60D216-41E2-4976-9DF7-4FD9DC839566}";   ///< Длительность полуволны
static const QString SemiWavAmpl = "{C98C5768-A189-4ACD-8906-4D6B11C0C871}";   ///< Амплитуда полуволны
}

static const QString DAPercent      = "{FCB7C8DD-6399-4D84-860E-BAF2A5841D8D}"; ///< Опережение маркера цели
static const QString DAhead         = "{45B4A53C-F917-46FE-9FBD-0229756F1193}"; ///< Суммарное опережение маркера цели
static const QString SemiWavLenDAC  = "{48054FEF-E300-441B-A7EC-C5AFD6AE1B7D}"; ///< Длительность полуволны при опережении
static const QString SemiWavAmplDAC = "{D910A2f5-3E4A-4BA0-B37C-57CB2781EC42}"; ///< Амплитуда полуволны при опережении
static const QString KorrCount      = "{81E931D0-0FE2-490F-898B-CEE3F0C8630E}"; ///< Общее количество коррекций

namespace CorrectionsMotor
{
static const QString Percent   = "{61A8888E-9856-4252-9AE6-0F6EEE82F7E9}"; ///< Процент коррекций
static const QString Error     = "{B0715B7F-EF62-41F1-9A42-C4511EC84DFA}"; ///< Средняя ошибка траектории
static const QString Amplitude = "{52FDE64B-1595-4C9C-B43D-3FBB955737FF}"; ///< Средняя амплитуда
static const QString TimeSumm  = "{57AB1A6B-4127-433E-A0FC-F30EFECBF3F3}"; ///< Суммарная длительность
static const QString TimeMid   = "{899D15C8-66F0-41F1-AECE-6E71E677A148}"; ///< Средняя длительность
static const QString Power     = "{0FE1C024-128B-4774-BCC1-140044C82218}"; ///< Мощность коррекций
}

namespace CorrectionsKognitive
{
static const QString Percent   = "{A568B999-0A76-49F2-A328-8AFF47311509}"; ///< Процент коррекций
static const QString Error     = "{15F51F00-0B3A-4A3A-89AA-B157C0A941CA}"; ///< Средняя ошибка траектории
static const QString Amplitude = "{661D5136-9CE2-438C-AB50-C37301894940}"; ///< Средняя амплитуда
static const QString TimeSumm  = "{7A834207-FE64-483A-8F3E-09E407E4BF5F}"; ///< Суммарная длительность
static const QString TimeMid   = "{F27A6793-1E69-494A-A3DE-541858024A05}"; ///< Средняя длительность
static const QString Power     = "{D09F6375-EC9D-4BB4-A035-B406B98B8814}"; ///< Мощность коррекций
}

}

class EvolventaResultData;

/*!
 * \brief Класс показателей теста Эвольвента EvolventaFactors class
 */
class EvolventaFactors : public ProbeMultifactor
{
public:
    explicit EvolventaFactors(const QString &testUid,
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
};

#endif // EVOLVENTAFACTORS_H
