#include "trianglefactors.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "triangleresultdata.h"

namespace
{
static const int PercentPointInCorner = 2;

}

TriangleFactors::TriangleFactors(const QString &testUid,
                                 const QString &probeUid,
                                 QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

bool TriangleFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool TriangleFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanTriangleResult);
    return de && se;
}

void TriangleFactors::calculate()
{
    getTriangleData();
    readSignal();
    computeTrianglesBounds();
    computeTriangles();
    computeSKOValues();

    addFactors();
}

void TriangleFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(TriangleFactorsDefines::GroupUid, tr("Показатели теста \"Треугольник\""));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::TimeUid, TriangleFactorsDefines::GroupUid,
                           tr("Средняя длительность прохода (обучение)"), tr("LenTest"), tr("сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::TimeQUid, TriangleFactorsDefines::GroupUid,
                           tr("Разброс длительности прохода (обучение)"), tr("LenQTest"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SquareUid, TriangleFactorsDefines::GroupUid,
                           tr("Средняя площадь треугольников (обучение)"), tr("SqrTest"), tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SquareQUid, TriangleFactorsDefines::GroupUid,
                           tr("Разброс площади треугольников (обучение)"), tr("SqrQTest"), tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SpeedUid, TriangleFactorsDefines::GroupUid,
                           tr("Средняя скорость прохождения (обучение)"), tr("SpdTest"), tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SpeedQUid, TriangleFactorsDefines::GroupUid,
                           tr("Разброс скорости прохождения (обучение)"), tr("SpdQTest"), tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MXUid, TriangleFactorsDefines::GroupUid,
                           tr("Среднее смещение треуг. по фронтали (обучение)"), tr("TrXTest"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MYUid, TriangleFactorsDefines::GroupUid,
                           tr("Среднее смещение треуг. по сагиттали (обучение)"), tr("TrYTest"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);


    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::TimeUid, TriangleFactorsDefines::GroupUid,
                           tr("Средняя длительность прохода (анализ)"), tr("LenAnal"), tr("сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::TimeQUid, TriangleFactorsDefines::GroupUid,
                           tr("Разброс длительности прохода (анализ)"), tr("LenQAnal"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SquareUid, TriangleFactorsDefines::GroupUid,
                           tr("Средняя площадь треугольников (анализ)"), tr("SqrAnal"), tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SquareQUid, TriangleFactorsDefines::GroupUid,
                           tr("Разброс площади треугольников (анализ)"), tr("SqrQAnal"), tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SpeedUid, TriangleFactorsDefines::GroupUid,
                           tr("Средняя скорость прохождения (анализ)"), tr("SpdAnal"), tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SpeedQUid, TriangleFactorsDefines::GroupUid,
                           tr("Разброс скорости прохождения (анализ)"), tr("SpdQAnal"), tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MXUid, TriangleFactorsDefines::GroupUid,
                           tr("Среднее смещение треуг. по фронтали (анализ)"), tr("TrXAnal"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MYUid, TriangleFactorsDefines::GroupUid,
                           tr("Среднее смещение треуг. по сагиттали (анализ)"), tr("TrYTest"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);

}

int TriangleFactors::freq() const
{
    if (m_resData)
        return m_resData->freq();
    return 128;
}

int TriangleFactors::diap() const
{
    if (m_resData)
        return m_resData->diap();
    return 128;
}

int TriangleFactors::trainingLength() const
{
    if (m_resData)
        return m_resData->trainingLength();
    return 0;
}

int TriangleFactors::signalLength() const
{
    return m_x.size();
}

TriangleDefines::Triangle TriangleFactors::triangleOriginal()
{
    if (m_resData)
        return TriangleDefines::Triangle(m_resData->topCorner(),
                                         m_resData->leftDownCorner(),
                                         m_resData->rightDownCorner());
    return TriangleDefines::Triangle(QPointF(), QPointF(), QPointF());
}

TriangleDefines::Triangle TriangleFactors::triangleTraining()
{
    return m_triangleAverageTraining;
}

TriangleDefines::Triangle TriangleFactors::triangleAnalysis()
{
    return m_triangleAverageAnalysis;
}

int TriangleFactors::trianglesCount() const
{
    return m_triangleSections.size();
}

BaseUtils::Section TriangleFactors::triangleSection(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_triangleSections.size());
    return m_triangleSections.at(idx);
}

TriangleDefines::Triangle TriangleFactors::triangle(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_triangles.size());
    return m_triangles.at(idx);
}

void TriangleFactors::readSignal()
{
    m_x.clear();
    m_xf.clear();
    m_y.clear();
    m_yf.clear();

    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);
        m_freq = stab.frequency();

        for (int i = 0; i < stab.size(); ++i)
        {
            double x = stab.value(0, i);
            double y = stab.value(1, i);
            m_x << x;
            m_y << y;
            m_xf << x;
            m_yf << y;
        }

        //! Фильтрация
        auto filtration = [&](QVector<double> &buffer)
        {
            BaseUtils::filterLowFreq(buffer, m_freq, 0.2, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
            BaseUtils::swapVector(buffer);
            BaseUtils::filterLowFreq(buffer, m_freq, 0.2, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
            BaseUtils::swapVector(buffer);
        };

        filtration(m_xf);
        filtration(m_yf);

//        BaseUtils::vectorToText(m_x, "c:/1/x.txt");
//        BaseUtils::vectorToText(m_xf, "c:/1/x_f.txt");
//        BaseUtils::vectorToText(m_y, "c:/1/y.txt");
//        BaseUtils::vectorToText(m_yf, "c:/1/y_f.txt");
    }
}

void TriangleFactors::computeTrianglesBounds()
{
    m_triangleSections.clear();

    int dir = -2;
    int begin = -1;
    int end = -1;
    for (int i = 0; i < m_yf.size(); ++i)
        if (i > 0)
        {
            if ((m_yf.at(i) < m_yf.at(i - 1)) && (dir == 0 || dir == 1))
            {
                end = i;
                if (begin > -1 && end > -1)
                    m_triangleSections << BaseUtils::Section(begin, end);
                //! Первый треугольник на этапе анализа
                if (begin < m_resData->trainingLength() && end >= m_resData->trainingLength())
                    m_firstAnalysisTriangle = m_triangleSections.size();
                if (begin == m_resData->trainingLength())
                    m_firstAnalysisTriangle = m_triangleSections.size() - 1;
                begin = i;
            }

            if (m_yf.at(i) > m_yf.at(i - 1))
                dir = 1;
            else
            if (m_yf.at(i) < m_yf.at(i - 1))
                dir = -1;
            else
                dir = 0;
        }
}

void TriangleFactors::computeTriangles()
{

    int n = 0;
    m_triangleAverageTraining = TriangleDefines::Triangle();
    m_triangleAverageAnalysis = TriangleDefines::Triangle();
    foreach (auto section, m_triangleSections)
    {
        //! Векторы: исходный и повернутые на +120 и -120 градусов
        QVector<QPointF> vn;
        QVector<QPointF> vrr;
        QVector<QPointF> vrl;
        double l = 0;
        double xp = 0;
        double yp = 0;
        for (int i = section.begin; i < section.end; ++i)
        {
            double x = m_x.at(i);
            double y = m_y.at(i);
            vn << QPointF(x, y);

            double xr = x;
            double yr = y;
            BaseUtils::rotatePoint(x, y, - 2 * M_PI / 3, xr, yr);
            vrr << QPointF(xr, yr);

            BaseUtils::rotatePoint(x, y, 2 * M_PI / 3, xr, yr);
            vrl << QPointF(xr, yr);

            if (i > section.begin)
                l = l + sqrt(pow(x - xp, 2) + pow(y - yp, 2));
            xp = x;
            yp = y;
        }

        //! Расчет координат вершин
        auto t = computeCorner(vn);
        auto ld = computeCorner(vrl);
        auto rd = computeCorner(vrr);

        //! Обратное вращение
        double x = ld.x();
        double y = ld.y();
        double xr = x;
        double yr = y;
        BaseUtils::rotatePoint(x, y, - 2 * M_PI / 3, xr, yr);
        ld.setX(xr);
        ld.setY(yr);
        x = rd.x();
        y = rd.y();
        BaseUtils::rotatePoint(x, y, 2 * M_PI / 3, xr, yr);
        rd.setX(xr);
        rd.setY(yr);

        //! Заполнение массива координат вершин
        TriangleDefines::Triangle trngl(t, ld, rd);
        double tm = static_cast<double>(vn.size()) / m_resData->freq();
        trngl.setTimeFactors(tm, l / tm);
        m_triangles << trngl;

        //! Расчет координат вершин усредненных треугольников
        auto averaging = [&](TriangleDefines::Triangle &triangle)
        {
            triangle.setTopCorner(QPointF(triangle.topCorner().x() + t.x(), triangle.topCorner().y() + t.y()));
            triangle.setLeftDownCorner(QPointF(triangle.leftDownCorner().x() + ld.x(), triangle.leftDownCorner().y() + ld.y()));
            triangle.setRightDownCorner(QPointF(triangle.rightDownCorner().x() + rd.x(), triangle.rightDownCorner().y() + rd.y()));
            triangle.setTimeFactors(triangle.time() + trngl.time(), triangle.speed() + trngl.speed());
        };
        if (n < m_firstAnalysisTriangle)
            averaging(m_triangleAverageTraining);
        else
            averaging(m_triangleAverageAnalysis);

        ++n;
    }

    //! Расчет координат вершин усредненных треугольников - деление на количество
    auto averaging = [&](TriangleDefines::Triangle  &triangle, const int count)
    {
        triangle.setTopCorner(QPointF(triangle.topCorner().x() / count, triangle.topCorner().y() / count));
        triangle.setLeftDownCorner(QPointF(triangle.leftDownCorner().x() / count, triangle.leftDownCorner().y() / count));
        triangle.setRightDownCorner(QPointF(triangle.rightDownCorner().x() / count, triangle.rightDownCorner().y() / count));
        triangle.calculate();
        triangle.setTimeFactors(triangle.time() / count, triangle.speed() / count);
    };
    averaging(m_triangleAverageTraining, m_firstAnalysisTriangle);
    averaging(m_triangleAverageAnalysis, m_triangles.size() - m_firstAnalysisTriangle);

}

bool coordinateTopLessThan(QPointF &p1, QPointF &p2)
{
    return p1.y() < p2.y();
}

QPointF TriangleFactors::computeCorner(QVector<QPointF> &stab)
{
    //! Сортируем массив
    std::sort(stab.begin(), stab.end(), coordinateTopLessThan);

    //! Кол-во точек
    int n = stab.size() / (100 / PercentPointInCorner);

    //! Выбор первых N% точек
    double mx = 0;
    double my = 0;
    for (int i = stab.size() - 1; i > stab.size() - 1 - n; --i)
    {
        mx = mx + stab.at(i).x();
        my = my + stab.at(i).y();
    }
    mx = mx / n;
    my = my / n;

    return QPointF(mx, my);
}

void TriangleFactors::computeSKOValues()
{
//    double timeMo = 0;
//    double squareMo = 0;
//    double speedMo = 0;
//    for (int i = 0; i < m_triangles.size(); ++i)
//    {
//        timeMo += m_triangles.at(i).time();
//        squareMo += m_triangles.at(i).square();
//        speedMo += m_triangles.at(i).speed();

//        if (i == m_firstAnalysisTriangle)
//        {

//        }

//    }

//    TriangleFactorsDefines::FactorValues
}

void TriangleFactors::getTriangleData()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanTriangleResult, baData))
        m_resData = new TriangleResultData(baData);
}

void TriangleFactors::addFactors()
{
    addFactor(TriangleFactorsDefines::Training::TimeUid, m_triangleAverageTraining.time());
    addFactor(TriangleFactorsDefines::Training::TimeQUid, 0);
    addFactor(TriangleFactorsDefines::Training::SquareUid, m_triangleAverageTraining.square());
    addFactor(TriangleFactorsDefines::Training::SquareQUid, 0);
    addFactor(TriangleFactorsDefines::Training::SpeedUid, m_triangleAverageTraining.speed());
    addFactor(TriangleFactorsDefines::Training::SpeedQUid, 0);
    addFactor(TriangleFactorsDefines::Training::MXUid, m_triangleAverageTraining.mx());
    addFactor(TriangleFactorsDefines::Training::MYUid, m_triangleAverageTraining.my());

    addFactor(TriangleFactorsDefines::Analysis::TimeUid, m_triangleAverageAnalysis.time());
    addFactor(TriangleFactorsDefines::Analysis::TimeQUid, 0);
    addFactor(TriangleFactorsDefines::Analysis::SquareUid, m_triangleAverageAnalysis.square());
    addFactor(TriangleFactorsDefines::Analysis::SquareQUid, 0);
    addFactor(TriangleFactorsDefines::Analysis::SpeedUid, m_triangleAverageAnalysis.speed());
    addFactor(TriangleFactorsDefines::Analysis::SpeedQUid, 0);
    addFactor(TriangleFactorsDefines::Analysis::MXUid, m_triangleAverageAnalysis.mx());
    addFactor(TriangleFactorsDefines::Analysis::MYUid, m_triangleAverageAnalysis.my());
}
