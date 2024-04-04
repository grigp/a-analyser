#ifndef CLASSICFACTORS_H
#define CLASSICFACTORS_H

#include <QObject>
#include <QCoreApplication>
#include "multifactor.h"


namespace ClassicFactorsDefines
{
static const QString GroupUid = "{CE4B0A70-953E-486E-B065-54BB2854BB5D}";
static const QString GroupName = QCoreApplication::tr("Классические показатели");

static const QString MoXUid = "{D2E9D021-07F5-49D4-967F-4AB7E05558D0}";
static const QString MoYUid = "{ADEA0860-76A3-4491-A3FB-BE130E1C64DA}";
static const QString QXUid = "{48DB78C0-EDBD-4065-9CC1-F767499B8C90}";
static const QString QYUid = "{4184ED12-B318-4D95-892F-4B20904E2503}";
static const QString RUid = "{E860357D-8493-43F6-9AC4-49A8557D0F15}";
static const QString LUid = "{FC3117F1-7C50-406B-997D-78683E09FF93}";

static const QString AngleUid = "{4580E0C5-9DBF-4261-8024-687C61B5016E}";
static const QString SquareUid = "{B256BAC6-DC9E-4C28-BC7F-D6C3E37BF1B5}";
static const QString ComprRatioUid = "{F7BECEAD-B402-4AE2-895D-6D9F07018455}";
static const QString EllLengthUid = "{CDE2BF99-C4BB-4A4E-87B7-758546F88861}";
static const QString EllWidthUid = "{A5474E08-2605-4D1E-AC7B-3C6199CDCDDC}";
static const QString LFSUid = "{771A4330-E586-48E3-929B-140B18C95C8F}";


/*!
 * \brief Структура параметров эллипса Ellipse struct
 */
struct Ellipse
{
    double sizeA;
    double sizeB;
    double angle;
    double ellS;
    double ellE;
    Ellipse() {}
};

static const double PDotInEllipse = 0.9;

}


class Stabilogram;

/*!
 * \brief Класс модуля расчета классических  показателей ClassicFactors class
 */
class ClassicFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    explicit ClassicFactors(const QString &testUid,
                            const QString &probeUid,
                            const QString &channelId,
                            const int begin = -1,
                            const int end  = -1,
                            QObject *parent = nullptr);

    /*!
     * \brief Возвращает uid группы показателей
     */
    QString uid() const override;

    /*!
     * \brief Возвращает название группы показателей
     */
    QString name() const override;

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
                        const QString &probeUid,
                        const QString &channelId);

    /*!
     * \brief Рассчитывает показатели для теста с заданным uid
     * uid теста задается в конструкторе
     */
    void calculate() override;

    /*!
     * \brief Регистрирует факторсгруппу и показатели
     */
    static void registerFactors();

    /*!
     * \brief Возвращает параметры эллипса
     */
    ClassicFactorsDefines::Ellipse ellipse() {return m_ellipse;}

private:

    /*!
     * \brief Рассчитывает показатели эллипса по Беляеву В.Е.
     * \param stab - стабилограмма
     */
    void computeParamsEllipse(Stabilogram *stab);

    void meanAndStdDev(QVector<double> &data, double &mean, double &stdDev);

    ///< Показатели
    double m_mx {0};
    double m_my {0};
    double m_qx {0};
    double m_qy {0};
    double m_r {0};
    double m_l {0};

    double m_angle {0};
    double m_square {0};
    double m_comprRatio {0};
    double m_ellLength {0};
    double m_ellWidth {0};
    double m_lfs {0};

    ///< Параметры эллипса
    ClassicFactorsDefines::Ellipse m_ellipse;
};

/////<------------------------------------------------------------------------------------
//class ClassicFactorsCreator: public ChannelFactorsCreator
//{
//public:
//    ClassicFactorsCreator() {}
//    ChannelMultifactor* createFactors(const QString &testUid,
//                                      const QString &probeUid,
//                                      const QString &channelId,
//                                      QObject *parent = nullptr) override
//    {return new ClassicFactors(testUid, probeUid, channelId, parent);}

//    bool isValid(const QString &testUid,
//                 const QString &probeUid,
//                 const QString &channelId) const override
//    {
//        return ClassicFactors::isValid(testUid, probeUid, channelId);
//    }

//};

#endif // CLASSICFACTORS_H
