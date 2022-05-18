#ifndef RATIOPROBESFACTORS_H
#define RATIOPROBESFACTORS_H

#include <QObject>
#include <QCoreApplication>

#include "multifactor.h"

namespace RatioProbesFactorsDefines
{
static const QString GroupUid = "{2A169548-A021-4040-8E86-5808D27A2012}";
static const QString GroupName = QCoreApplication::tr("Показатели отношений");

static const QString Probe2SUid = "{BA799ED3-D98D-475E-ADE1-8B39E5EC3915}";
static const QString Probe2KFRUid = "{0D7A35F4-F8CC-47E2-850D-C9ABC89300FD}";
static const QString Probe3SUid = "{9ED8A747-233D-43C1-84D9-ACCB0EAAFE64}";
static const QString Probe3KFRUid = "{A4CC87B5-2F9A-4EE6-BF30-30CA0347A8DB}";
static const QString Probe4SUid = "{8726AC6F-85E3-453D-9B4D-91BA2C1DB7C9}";
static const QString Probe4KFRUid = "{82A65C5F-EACC-4A73-82F1-DEBDC3E04AF7}";
static const QString Probe5SUid = "{6F5D8B84-008D-4B58-9D42-479A523834F9}";
static const QString Probe5KFRUid = "{DF4655B2-A87B-461B-8D5F-909C6BCC9246}";
}

/*!
 * \brief Класс модуля расчета показателей отношений между пробами RatioProbesFactors class
 */
class RatioProbesFactors : public TestMultifactor
{
    Q_OBJECT
public:
    explicit RatioProbesFactors(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * uid теста задается в конструкторе
     */
    bool isValid()  const override;
    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * Аналог для вызова без создания экземпляра объекта
     */
    static bool isValid(const QString &testUid);

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

#endif // RATIOPROBESFACTORS_H
