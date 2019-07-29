#ifndef FACTORSDEFINES_H
#define FACTORSDEFINES_H

#include <QString>

namespace FactorsDefines {

/*!
 * \brief Класс информации о показателе FactorInfo class
 */
class FactorInfo
{
public:
    FactorInfo(const QString &uid, const QString &groupUid,
               const QString &name, const QString &shortName,
               const QString &measure, const int format)
        : m_uid(uid), m_groupUid(groupUid),
          m_name(name), m_shortName(shortName),
          m_measure(measure), m_format(format)
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

private:
    QString m_uid;
    QString m_groupUid;
    QString m_name;
    QString m_shortName;
    QString m_measure;
    int m_format;
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
 * \brief Класс значения показателя с описанием FactorValueDescript class
 * Описание нужно, еслии берутся два одинаковых показателя из разных проб
 * Выводиться может, например, так <name> (<description>): КФР (Открытые глаза)
 */
class FactorValueDescript : public FactorValue
{
public:
    FactorValueDescript(const QString &uid,
                        const double value,
                        const QString &description)
        : FactorValue(uid, value)
        , m_description(description)
    {}
    FactorValueDescript() {}

    QString description() const {return m_description;}
    void setDescription(const QString &description) {m_description = description;}

private:
    QString m_description;
};

}

#endif // FACTORSDEFINES_H
