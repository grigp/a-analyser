#include "baseutils.h"

#include <QDir>

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
    QDir dir("translations");
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot);
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
