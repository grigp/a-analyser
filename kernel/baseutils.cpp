#include "baseutils.h"

#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QtMath>

#include "basedefines.h"

QString BaseUtils::getTimeBySecCount(const int secCnt, const bool isHour)
{
    int min = secCnt / 60;
    int sec = secCnt % 60;
    if (isHour)
    {
        int hour = min / 60;
        min = min % 60;
        return QString::number(hour).rightJustified(2, '0') + ":" +
               QString::number(min).rightJustified(2, '0') + ":" +
               QString::number(sec).rightJustified(2, '0');
    }
    return QString::number(min).rightJustified(2, '0') + ":" +
           QString::number(sec).rightJustified(2, '0');
}

bool BaseUtils::getTranslatorFileName(QString &fileName, QString &langCode)
{
    QDir dir(QCoreApplication::applicationDirPath() + "/translations");
    QFileInfoList list = dir.entryInfoList(); //QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        {
            if (fileInfo.fileName().contains("a-analyser_") &&
                fileInfo.completeSuffix() == "qm")
            {
                fileName = fileInfo.absoluteFilePath();
                langCode = fileInfo.baseName().remove("a-analyser_");
                return true;
            }
        }
    return false;
}

double BaseUtils::getAngleByCoord(const double x, const double y)
{
    if (x == 0 && y >= 0)
        return 0;
    else
    if (x > 0 && y == 0)
        return M_PI_2;
    else
    if (x == 0 && y < 0)
        return M_PI;
    else
    if (x < 0 && y == 0)
        return 3   * M_PI_2;
    if (x > 0 && y > 0)
        return atan(x / y);
    else
    if (x > 0 && y < 0)
        return M_PI_2 + atan(fabs(y / x));
    else
    if (x < 0 && y < 0)
        return M_PI + atan(fabs(x / y));
    else
    if (x < 0 && y > 0)
        return 3 * M_PI / 2 + atan(fabs(y / x));
    else
        return 0;
}

double BaseUtils::getDifferenceAngles(const double a1, const double a2)
{
    if (a1 < M_PI_2 && a2 > 3 * M_PI_2)
        return -(a1 + 2 * M_PI - a2);
    else
    if (a2 < M_PI_2 && a1 > 3 * M_PI_2)
        return 2 * M_PI - a1 + a2;
    else
        return a2 - a1;
}

QSize BaseUtils::getTextSize(QPainter *painter, const QString &text)
{
    QFontMetrics fm(painter->font());
    int w = fm.width(text);
    int h = fm.height();
    return QSize(w, h);
}



QString BaseUtils::colorToRGBAStr(const QColor color)
{
    unsigned int cc = color.rgba();
    return QString::number(cc, 16);
}

QColor BaseUtils::strRGBAToColor(const QString rgbaStr)
{
    bool ok;
    unsigned int colCode = rgbaStr.toLongLong(&ok, 16);
    return QColor::fromRgba(colCode);
}


void BaseUtils::setColoredPicture(QPixmap &pixmap, const QColor &color)
{
    int o_r = color.red();
    int o_g = color.green();
    int o_b = color.blue();
    auto image = pixmap.toImage();
    for (int i = 0; i < pixmap.width(); ++i)
        for (int j = 0; j < pixmap.height(); ++j)
        {
            auto rgb = image.pixel(i, j);
            auto pixelColor = qRed(rgb);
            auto pixelA = qAlpha(rgb);
            if (pixelColor > 0)
            {
                int r = o_r * pixelColor / 255;
                int g = o_g * pixelColor / 255;
                int b = o_b * pixelColor / 255;
                image.setPixel(i, j, qRgba(r, g, b, pixelA));
            }
        }
    pixmap.convertFromImage(image);
}


/*!
 * \brief Заменяет в строке str символы \n на пробелы
 * \return Возвращает строку с пробелами
 */
QString enterToSpace(const QString &str)
{
    auto sl = str.split('\n');
    QString retval = "";
    foreach (auto s, sl)
        retval = retval + s + " ";
    return retval;
}

void BaseUtils::modelToMSExcel(const QAbstractItemModel *model, const QString &fileName)
{
    QFile csvFile(fileName);
    if (csvFile.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&csvFile);
        QStringList sl;

        sl.clear();
        for (int j = 0; j < model->columnCount(); ++j)
            sl << enterToSpace(model->headerData(j, Qt::Horizontal).toString());
        ts << sl.join(";") + "\n";

        for (int i = 0; i < model->rowCount(); ++i)
        {
            sl.clear();

            for (int j = 0; j < model->columnCount(); ++j)
                sl << model->index(i, j).data(Qt::DisplayRole).toString();

            ts << sl.join(";") + "\n";
        }

        csvFile.close();
    }
}

void BaseUtils::modelToText(const QAbstractItemModel *model, const QString &fileName, const QString& divider)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);
        QStringList sl;

        sl.clear();
        for (int j = 0; j < model->columnCount(); ++j)
            sl << enterToSpace(model->headerData(j, Qt::Horizontal).toString()) << divider;
        ts << sl.join("         ") + "\n";

        for (int i = 0; i < model->rowCount(); ++i)
        {
            sl.clear();

            for (int j = 0; j < model->columnCount(); ++j)
                sl << enterToSpace(model->index(i, j).data(Qt::DisplayRole).toString()) << divider;

            ts << sl.join("        ") + "\n";
        }

        file.close();
    }
}

void BaseUtils::vectorToText(QVector<double> &vector, const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);

        for (int i = 0; i < vector.size(); ++i)
        {
            auto s = QString::number(vector[i]);
            ts << s << "\n";
        }


        file.close();
    }
}


void BaseUtils::pointsToTextSeparate(QVector<QPointF> &vector, const QString &fileName, const char separator)
{
    Q_UNUSED(separator);
    QLocale locale;
    QFile fX(fileName + ".x");
    QFile fY(fileName + ".y");
    if (fX.open(QIODevice::WriteOnly) && fY.open(QIODevice::WriteOnly))
    {
        QTextStream tsX(&fX);
        QTextStream tsY(&fY);

        for (int i = 0; i < vector.size(); ++i)
        {
            auto s = QString::number(vector[i].x());
            s.replace(".", locale.decimalPoint());
            tsX << s << "\n";
            s = QString::number(vector[i].y());
            s.replace(".", locale.decimalPoint());
            tsY << s << "\n";
        }

        fX.close();
        fY.close();
    }
}

void BaseUtils::swapVector(QVector<double> &vector)
{
    QVector<double> v;
    v.clear();
    for (int i = vector.size() - 1; i >= 0; --i)
        v << vector.at(i);
    vector.clear();
    for (int i = 0; i < v.size(); ++i)
        vector << v.at(i);
}

double getCoef1(const BaseUtils::FilterKind tf)
{
    switch (tf) {
    case BaseUtils::fkCriticalAttenuation: return 1.2872;
    case BaseUtils::fkBessel: return 1.3617;
    case BaseUtils::fkBatterwort: return 1.4142;
    case BaseUtils::fkChebyshev: return 1.3614;
    }
}

double getCoef2(const BaseUtils::FilterKind tf)
{
    switch (tf) {
    case BaseUtils::fkCriticalAttenuation: return 0.4142;
    case BaseUtils::fkBessel: return 0.618;
    case BaseUtils::fkBatterwort: return 1.0;
    case BaseUtils::fkChebyshev: return 1.3827;
    }
}

void BaseUtils::filterLowFreq(QVector<double> &buffer, const double fd, const double fc, const BaseUtils::FilterKind tf, const int n1, const int n2)
{
    double w = fd / fc;
    double alf = M_PI / w;
    double l = qCos(alf) / qSin(alf);
    double dd0 = 1;
    double dd1 = 0;
    double dd2 = dd1;
    double cc0 = 1;

    double cc1 = getCoef1(tf);
    double cc2 = getCoef2(tf);

    double d0 = (dd0 - dd1 * l + dd2 * l * l) / (cc0 + cc1 * l + cc2 * l * l);
    double d1 = 2 * (dd0 - dd2 * l * l) / (cc0 + cc1 * l + cc2 * l * l);
    double d2 = (dd0 + dd1 * l + dd2 * l * l) / (cc0 + cc1 * l + cc2 * l * l);
    double c0 = (cc0 - cc1 * l + cc2 * l * l) / (cc0 + cc1 * l + cc2 * l * l);
    double c1 = 2 * (cc0 - cc2 * l * l) / (cc0 + cc1 * l + cc2 * l * l);

    QVector<double> X;
    X.resize(buffer.size() + 40);
    for (int i = 0; i < 20; ++i)
        X[i] = buffer[0];
    for (int i = n1; i < buffer.size(); ++i)
        X[20 + i] = buffer[i];
    for (int i = 0; i < 20; ++i)
        X[n2 + i] = buffer[buffer.size() - 1];

    QVector<double> Y;
    Y.resize(X.size());

    Y[0] = 0;
    int n22 = Y.size() - 1;
    for (int i = 0; i < 0 + 2; ++i)
        Y[0] = Y[0] + X[i];
    Y[n22-1] = Y[0] / 3;
    Y[n22] = Y[n22-1];

    for (int k = 1; k <= l; ++k) //! кратность фильтра
    {
        for (int i = 0; i <= n22; ++i)
        {
            int i1 = i-1;
            int i2 = i-2;
            if (i == 0)
            {
                i1 = n22;
                i2 = n22 - 1;
            }
            else
            if (i == 0 + 1)
            {
                i1 = 0;
                i2 = n22;
            }

            Y[i] = (d2 * X[i] + d1 * X[i1] - c1 * Y[i1] + d0 * X[i2] - c0 * Y[i2]);  // фильтр Пол.
        }
    }

    for (int i = n1; i <= n2; ++i)
        buffer[i] = Y[20 + i];
}



int BaseUtils::sign(const double number)
{
    double num = number;
    if (fabs(num) < 1)
    {
        if (num > 0)
            num = num + 1;
        else
            num = num - 1;
    }
    int retval = static_cast<int>(num);
    return retval / abs(retval);
}

void BaseUtils::convertDecartToPolar(const double x, const double y, double &r, double &ph)
{
    r = sqrt(pow(x, 2) + pow(y, 2));

    if (x > 0 && y >= 0)
        ph = qAtan(x/y);
    else
    if (x > 0 && y <= 0)
        ph = M_PI / 2 + qAtan(fabs(y/x));
    else
    if (x <= 0 && y < 0)
        ph = M_PI + qAtan(fabs(x/y));
    else
    if (x < 0 && y > 0)
        ph = 3 * M_PI / 2 + qAtan(fabs(y/x));
    else
        ph = 0;
}

void BaseUtils::convertPolarToDecart(const double r, const double ph, double &x, double &y)
{
    if (ph >= 0 && ph <= M_PI_2)
    {
        x = r * sin(ph);
        y = r * cos(ph);
    }
    else
    if (ph > M_PI_2 && ph <= M_PI)
    {
        x = r * cos(ph - M_PI_2);
        y = - r * sin(ph - M_PI_2);
    }
    else
    if (ph > M_PI && ph <= 3 * M_PI_2)
    {
        x = - r * sin(ph - M_PI);
        y = - r * cos(ph - M_PI);
    }
    else
    if (ph > 3 * M_PI_2 && ph <= 2 * M_PI)
    {
        x = - r * cos(ph - 3 * M_PI_2);
        y = r * sin(ph - 3 * M_PI_2);
    }
    else
    {
        x = 0;
        y = 0;
    }
}

void BaseUtils::rotatePoint(const double x, const double y, const double alfa, double &xr, double &yr)
{
    double r = 0;
    double ph = 0;
    convertDecartToPolar(x, y, r, ph);

    ph += alfa;
    if (ph < 0)
        ph = 2 * M_PI - fabs(ph);
    else
    if (ph > 2 * M_PI)
        ph = ph - 2 * M_PI;

    convertPolarToDecart(r, ph, xr, yr);
}

void BaseUtils::setCorrectionsDominanceResume(const double cdv, QString &resume, QString &resumeColor)
{
    QString korr = "";
    resumeColor = "color: rgb(100, 100, 100)";
    if (fabs(cdv) <= 10)
        resumeColor = "color: rgb(32, 88, 103)";
    else
    if ((fabs(cdv) > 10) && (fabs(cdv) <= 30))
    {
        korr = QCoreApplication::tr("незначительно");
        resumeColor = "color: rgb(0, 150, 0)";
    }
    else
    if ((fabs(cdv) > 30) && (fabs(cdv) <= 50))
    {
        korr = QCoreApplication::tr("умеренно");
        resumeColor = "color: rgb(150, 150, 0)";
    }
    else
    {
        korr = QCoreApplication::tr("выражено");
        resumeColor = "color: rgb(150, 0, 0)";
    }

    resume = "";
    if (cdv > 10)
        resume = QCoreApplication::tr("Преобладание когнитивных коррекций") + ' ' + korr;
    else
    if (cdv < -10)
      resume = QCoreApplication::tr("Преобладание моторных коррекций") + ' ' + korr;
    else
      resume = QCoreApplication::tr("Нет преобладания типа коррекции");
}

void BaseUtils::setOutrunningResume(const double orv, QString &resume, QString &resumeColor)
{
    QString rate = "";
    resumeColor = "color: rgb(100, 100, 100)";
    if (fabs(orv) <= 10)
        resumeColor = "color: rgb(32, 88, 103)";
    else
    if ((fabs(orv) > 10) && (fabs(orv) <= 30))
    {
        rate = QCoreApplication::tr("незначительно");
        resumeColor = "color: rgb(0, 150, 0)";
    }
    else
    if ((fabs(orv) > 30) && (fabs(orv) <= 50))
    {
        rate = QCoreApplication::tr("умеренно");
        resumeColor = "color: rgb(150, 150, 0)";
    }
    else
    {
        rate = QCoreApplication::tr("выражено");
        resumeColor = "color: rgb(150, 0, 0)";
    }

    resume = "";
    if (orv > 10)
        resume = QCoreApplication::tr("Маркер опережает цель") + ' ' + rate;
    else
    if (orv < -10)
      resume = QCoreApplication::tr("Маркер отстает от цели") + ' ' + rate;
    else
      resume = QCoreApplication::tr("Маркер на цели");
}


int BaseUtils::scaleMultiplier(const int idx)
{
    int retval = 1;
    for (int i = 0; i < idx; ++i)
        retval *= 2;
    return retval;
}

void BaseUtils::MidAndStandardDeviation::calculate(double &mid, double &stdDev) const
{
    mid = 0;
    stdDev = 0;

    if (m_values.size() > 0)
    {
        foreach (auto val, m_values)
            mid += val;
        mid /= m_values.size();

        foreach (auto val, m_values)
            stdDev += pow(fabs(val - mid), 2) / (m_values.size() - 1);
        if (stdDev > 0)
            stdDev = sqrt(stdDev);
    }
}

int BaseUtils::sign(const int value)
{
    if (value < 0)
        return BaseDefines::NegativeValue;
    else
    if (value > 0)
        return BaseDefines::NegativeValue;
    return BaseDefines::ZeroValue;
}
