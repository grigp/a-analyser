#ifndef FACTORSDEFINES_H
#define FACTORSDEFINES_H

#include <QString>

namespace FactorsDefines {

/*!
 * \brief Направление нормы NormSide enum
 */
enum NormSide
{
    nsBelow = -1,   ///< Нижняя. Норма то, что ниже значения
    nsDual = 0,     ///< Двусторонняя. Норма то, что внутри
    nsAbove = 1     ///< Верхняя. Норма то, что выше значения
};

/*!
 * \brief Класс информации о показателе FactorInfo class
 */
class FactorInfo
{
public:
    FactorInfo(const QString &uid, const QString &groupUid,
               const QString &name, const QString &shortName,
               const QString &measure, const int format,
               const double multiplier, const NormSide side, const int agingPeriod)
        : m_uid(uid), m_groupUid(groupUid),
          m_name(name), m_shortName(shortName),
          m_measure(measure), m_format(format),
          m_multiplier(multiplier), m_side(side), m_agingPeriod(agingPeriod)
    {}
    FactorInfo() {}

    QString uid() const {return m_uid;}
    void setUid(const QString &uid) {m_uid = uid;}

    QString groupUid() const {return m_groupUid;}
    void setGroupUid(const QString &groupUid) {m_groupUid = groupUid;}

    QString name() const {return m_name;}
    void setName(const QString &name) {m_name = name;}

    QString shortName() const {return m_shortName;}
    void setshortName(const QString &shortName) {m_shortName = shortName;}

    QString measure() const {return m_measure;}
    void setMeasure(const QString &measure) {m_measure = measure;}

    int format() const {return m_format;}
    void setFormat(const int format) {m_format = format;}

    double multiplier() const {return m_multiplier;}
    void setMultiplier(const double multiplier) {m_multiplier = multiplier;}

    NormSide side() const {return m_side;}
    void setSide(const NormSide side) {m_side = side;}

    int agingPeriod() const {return m_agingPeriod;}
    void setAgingPeriod(const int agingPeriod) {m_agingPeriod = agingPeriod;}

private:
    QString m_uid;
    QString m_groupUid;
    QString m_name;
    QString m_shortName;
    QString m_measure;
    int m_format;
    double m_multiplier;
    NormSide m_side;
    int m_agingPeriod;      ///< Период устаревания в месяцах
};


/*!
 * \brief Класс показателя со значением FactorValue class
 * Содержит uid показателя и его значение
 * По uid можно получить информацию о показателе из FactorInfo
 */
class FactorValue
{
public:
    FactorValue(const QString &uid, const double value)
        : m_uid(uid), m_value(value)
    {}
    FactorValue() {}

    QString uid() const {return m_uid;}
    void setUid(const QString &uid) {m_uid = uid;}

    double value() const {return m_value;}
    void setValue(const double value) {m_value = value;}

private:
    QString m_uid;
    double m_value;
};


/*!
 * \brief Класс значения показателя с данными о его положении в тесте FactorValueAdvanced class
 * Описание нужно, еслии берутся два одинаковых показателя из разных проб
 * Выводиться может, например, так <name> (<description>): КФР (Открытые глаза)
 */
class FactorValueAdvanced : public FactorValue
{
public:
    FactorValueAdvanced(const QString &uid,
                        const double value,
                        const int probeNum,
                        const QString &channelId,
                        const QString &description)
        : FactorValue(uid, value)
        , m_probeNum(probeNum)
        , m_channelId(channelId)
        , m_description(description)
    {}
    FactorValueAdvanced() {}

    QString description() const {return m_description;}
    void setDescription(const QString &description) {m_description = description;}

    QString channelId() const {return m_channelId;}
    void setChannelId(const QString &channelId) {m_channelId = channelId;}

    int probeNum() const {return m_probeNum;}
    void setProbeNum(const int probeNum) {m_probeNum = probeNum;}

private:
    int m_probeNum;
    QString m_channelId;
    QString m_description;
};

}

#endif // FACTORSDEFINES_H
