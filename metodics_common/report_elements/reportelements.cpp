#include "reportelements.h"

#include <QApplication>
#include <QDebug>

#include "baseutils.h"
#include "channelsutils.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "settingsprovider.h"
#include "dataprovider.h"
#include "datadefines.h"
#include "skgpainter.h"
#include "graphpainter.h"
#include "stabilogram.h"
#include "bilateralresultdata.h"
#include "classicfactors.h"
#include "dynamicdiagrampainter.h"

void ReportElements::drawHeader(QPainter *painter, const QString &testUid, QRect rect)
{
    auto fio = new SettingsValue("UserData", "FIO", "");
    auto enterprice = new SettingsValue("UserData", "Enterprice", "");
    auto address = new SettingsValue("UserData", "Address", "");
    auto sity = new SettingsValue("UserData", "Sity", "");
    auto phones = new SettingsValue("UserData", "Phones", "");
    auto email = new SettingsValue("UserData", "Email", "");
    auto website = new SettingsValue("UserData", "Website", "");
    auto logotip = new SettingsValue("UserData", "Logotip");
    QPixmap p;
    p.loadFromData(logotip->value().toByteArray(), "PNG");

    painter->setPen(Qt::black);
    //! Подбор высоты шрифта
    int n = 14;
    int h = 0;
    do
    {
        painter->setFont(QFont("Sans", n, 0, false));
        QFontMetrics fm = painter->fontMetrics();
        h = fm.height();
        n--;
    }
    while (h * 0.8 > rect.height() / 7);

    painter->drawText(rect.x() + rect.height() + 70, rect.y(), enterprice->value().toString());
    painter->drawText(rect.x() + rect.height() + 70, rect.y() + rect.height() / 7, sity->value().toString() + "   " + address->value().toString());
    painter->drawText(rect.x() + rect.height() + 70, rect.y() + rect.height() / 7 * 2,
                      QCoreApplication::tr("Тел") + ":" + phones->value().toString() + "   " +
                      QCoreApplication::tr("e-mail") + ":" + email->value().toString() + "   " +
                      website->value().toString());
    painter->drawText(rect.x() + rect.height() + 70, rect.y() + rect.height() / 7 * 3, fio->value().toString());

    DataDefines::TestInfo ti;
    DataProvider::getTestInfo(testUid, ti);
    DataDefines::PatientKard patient;
    DataProvider::getPatient(ti.patientUid, patient);
    auto* methods = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    auto mi = methods->metodic(ti.metodUid);

    painter->drawText(rect.x() + rect.height() + 70, rect.y() + rect.height() / 7 * 4,
                      QCoreApplication::tr("Протокол теста") + " - \"" + mi.name + "\"");
    painter->drawText(rect.x() + rect.height() + 70, rect.y() + rect.height() / 7 * 5,
                      QCoreApplication::tr("Пациент") + " - " + patient.fio + "(" + patient.born.toString("dd.MM.yyyy") + ")");
    painter->drawText(rect.x() + rect.height() + 70, rect.y() + rect.height() / 7 * 6,
                      QCoreApplication::tr("Дата и время проведения") + " - (" + ti.dateTime.toString("dd.MM.yyyy hh:mm") + ")");

    painter->drawLine(rect.x(), rect.y() + rect.height(), rect.x() + rect.width(), rect.y() + rect.height());
    painter->drawPixmap(rect.x(), rect.y(), static_cast<int>(rect.height() * 0.9),
                        static_cast<int>(rect.height() * 0.9 * (static_cast<double>(p.height())/static_cast<double>(p.width()))),
                        p,
                        0, 0, p.width(), p.height());
}

void ReportElements::drawFooter(QPainter *painter, const QString &testUid, QRect rect)
{
    Q_UNUSED(testUid);
    auto enterprice = new SettingsValue("UserData", "Enterprice", "");
    auto email = new SettingsValue("UserData", "Email", "");
    auto website = new SettingsValue("UserData", "Website", "");

    painter->setPen(Qt::black);
    painter->setFont(QFont("Sans", 12, 0, false));
    painter->drawLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y());

    painter->drawText(rect.x(), rect.y() + rect.height() / 2,
                      enterprice->value().toString() + "    " +
                      email->value().toString() + "    " +
                      website->value().toString());
    QString s = QApplication::applicationName() + " " + QApplication::applicationVersion();
    auto size = BaseUtils::getTextSize(painter, s);
    painter->drawText(rect.x() + rect.width() - size.width(), rect.y() + rect.height() / 2, s);
}

void ReportElements::drawWidget(QPainter *painter, QWidget *widget, const int w, const int h, const int x, const int y)
{
    double xscale = static_cast<double>(w) / static_cast<double>(widget->width());
    double yscale = static_cast<double>(h) / static_cast<double>(widget->height());
    double scale = qMin(xscale, yscale);
    painter->translate(x, y);
    painter->scale(scale, scale);
    widget->render(painter);
    painter->scale(1/scale, 1/scale);
    painter->translate(-x, -y);
};

void ReportElements::drawTable(QPainter *painter, QStandardItemModel *model, QRect rect,
                               QList<int> columnStretch,
                               bool isReplaceTitle,
                               const Table::VerticalStretch vStretch,
                               const int pointSize, const int weight, const int titleWeight)
{
    //! Заполнение реальными дынными растяжения
    int sSum = 0;
    QList<int> cs;
    for (int j = 0; j < model->columnCount(); ++j)
    {
        int sv = 1;
        if ((j < columnStretch.size()) && (columnStretch.at(j) > 0))
            sv = columnStretch.at(j);
        cs << sSum;
        sSum += sv;
    }

    painter->setFont(QFont("Sans", pointSize, weight, false));
    double dh = rect.height() / (model->rowCount() + 1);
    if (vStretch == Table::tvsCompressed)
    {
        QFontMetrics fm = painter->fontMetrics();
        dh = fm.height();
    }
    painter->setPen(Qt::black);

    //! Заголовок
    painter->setFont(QFont("Sans", pointSize, titleWeight, false));
    for (int j = 0; j < model->columnCount(); ++j)
    {
        QString title = "";
        if (isReplaceTitle && (j > 0))
            title = "(" + QString::number(j) + ")";
        else
            title = model->headerData(j, Qt::Horizontal).toString();
        painter->drawText(static_cast<int>(rect.x() + rect.width() * cs.at(j) / sSum), rect.y(), title);
    }
    //! Таблица
    painter->setFont(QFont("Sans", pointSize, weight, false));
    for (int i = 0; i < model->rowCount(); ++i)
        for (int j = 0; j < model->columnCount(); ++j)
        {
            auto label = model->index(i, j).data().toString();
            painter->drawText(static_cast<int>(rect.x() + rect.width() * cs.at(j) / sSum), static_cast<int>(rect.y() + (i + 1) * dh), label);
        }
}

int computeDiap(BilateralResultData& bData)
{
    auto plate1 = bData.platform(0);
    auto plate2 = bData.platform(1);
    int xMin = plate1.x();
    if (plate2.x() < plate1.x())
        xMin = plate2.x();
    int xMax = plate1.x() + plate1.width();
    if (plate2.x() + plate2.width() > plate1.x() + plate1.width())
        xMax = plate2.x() + plate2.width();

    int yMax = plate1.y();
    if (plate2.y() > plate1.y())
        yMax = plate2.y();
    int yMin = plate1.y() - plate1.height();
    if (plate2.y() - plate2.height() < plate1.y() - plate1.height())
        yMin = plate2.y() - plate2.height();

    int diap = qMax(abs(xMin), abs(xMax));
    diap = qMax(diap, abs(yMin));
    diap = qMax(diap, abs(yMax));

    return diap;
}

void ReportElements::drawSKG(QPainter *painter,
                             const QRect &rect,
                             const QString &testUid,
                             const int probeNum,
                             const double ratio,
                             const int diap,
                             const int begin,
                             const int end,
                             QList<SKGDefines::BrokenLine> brokenLines)
{
    //! Признаки наличия каналов: основного, правого и левого
    QString csMain = "";
    QString csLeft = "";
    QString csRight = "";
    QString probeUid = "";
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        if (probeNum >= 0 && probeNum < ti.probes.size())
        {
            probeUid = ti.probes.at(probeNum);
            if (DataProvider::channelExists(probeUid, ChannelsDefines::chanStab))
                csMain = ChannelsDefines::chanStab;
            if (DataProvider::channelExists(probeUid, ChannelsDefines::chanStabLeft))
                csLeft = ChannelsDefines::chanStabLeft;
            if (DataProvider::channelExists(probeUid, ChannelsDefines::chanStabRight))
                csRight = ChannelsDefines::chanStabRight;
        }

    //! Есть проба и основной канал
    if (probeUid != "" && csMain != "")
    {
        //! Создаем рисователь СКГ
        SKGPainter skg(painter, rect);
        //! Получаем основной канал
        QByteArray baStab;
        if (DataProvider::getChannel(probeUid, csMain, baStab))
        {
            //! Передаем его в рисователь
            Stabilogram stab(baStab);
            skg.setSignal(&stab, 0, begin, end);
            //! Эллипс
            if (begin == -1 || end == -1)  //! Эллипс строится только если задан полный сигнал
            {
                ClassicFactors fctClassic(testUid, ti.probes.at(probeNum), ChannelsDefines::chanStab);
                skg.setEllipse(fctClassic.ellipse().sizeA, fctClassic.ellipse().sizeB, fctClassic.ellipse().angle);
            }
            //! Ломаные, например, треугольники
            foreach (auto bl, brokenLines)
                skg.addBrokenLine(bl);

            //! Данные канала билатерального теста. Он может быть, а может и нет. Если есть, то рисуем платформы
            QByteArray badBilat;
            if (DataProvider::getChannel(probeUid, ChannelsDefines::chanBilat, badBilat))
            {
                BilateralResultData bData(badBilat);

                //! Получаем канал левый и передаем его в рисователь
                QByteArray baStabLeft;
                if (csLeft != "")
                    DataProvider::getChannel(probeUid, csLeft, baStabLeft);
                Stabilogram stabL(baStabLeft);
                if (baStabLeft.size() > 0)
                {
                    skg.setSignal(&stabL, 1, begin, end);
                    //! Смещение - положение левой платформы
                    skg.setOffset(-bData.platform(0).center().x(), -bData.platform(0).y() + bData.platform(0).height() / 2, 1);
                }

                //! Получаем канал левый и передаем его в рисователь
                QByteArray baStabRight;
                if (csRight!= "")
                    DataProvider::getChannel(probeUid, csRight, baStabRight);
                Stabilogram stabR(baStabRight);
                if (baStabRight.size() > 0)
                {
                    skg.setSignal(&stabR, 2, begin, end);
                    //! Смещение - положение правой платформы
                    skg.setOffset(-bData.platform(1).center().x(), -bData.platform(1).y() + bData.platform(1).height() / 2, 2);
                }

                //! Добавляем платформы в рисователь
                for (int i = 0; i < bData.platformsCount(); ++i)
                    skg.addPlatform(bData.platform(i));
                int diapSKG = computeDiap(bData);  //! И корректируем diap, он должен показавать платформы

                skg.setDiap(diapSKG);    //! Объединять нельзя, вызов должен быть в зоне видимости stabL и stabR
                skg.setZeroing(true);

                skg.doPaint(ratio);
            }
            else
            {
                //! Не билатеральный тест. Просто выведем СКГ
                int diapSKG = BaseUtils::scaleAbove(stab.absMaxValue());
                if (diap != -1)
                    diapSKG = diap;
                skg.setDiap(diapSKG);
                skg.doPaint(ratio);
            }
        }
    }
}

void ReportElements::drawGraph(QPainter *painter,
                               const QRect &rect,
                               const QString &testUid,
                               const int probeNum,
                               const double ratio,
                               const int diap,
                               const int begin,
                               const int end)
{
    Q_UNUSED(diap);
    Q_UNUSED(begin);
    Q_UNUSED(end);

    //! Признаки наличия каналов: основного, правого и левого
    QString csMain = "";
    QString probeUid = "";
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        if (probeNum >= 0 && probeNum < ti.probes.size())
        {
            probeUid = ti.probes.at(probeNum);
            if (DataProvider::channelExists(probeUid, ChannelsDefines::chanStab))
                csMain = ChannelsDefines::chanStab;
        }

    //! Есть проба и основной канал
    if (probeUid != "" && csMain != "")
    {
        //! Создаем рисователь
        GraphPainter gp(painter, rect);
        //! Получаем канал
        QByteArray baStab;
        if (DataProvider::getChannel(probeUid, csMain, baStab))
        {
            //! Передаем его в рисователь
            Stabilogram stab(baStab);
            gp.appendSignal(&stab, "");

            gp.setLegend(0, QStringList() << "Фронталь" << "Сагитталь");
            int diap = 1;
            int step = 0;
            while (diap < stab.absMaxValue())
            {
                diap = diap * 2;
                ++step;
            }
            gp.setDiapazone(-diap, diap);
            gp.doPaint(ratio);
        }
    }
}

void ReportElements::drawDynamicDiag(QPainter *painter,
                                     const QRect &rect,
                                     const double ratio,
                                     QVector<double> &data,
                                     QStringList &labels,
                                     const QString &title,
                                     DynamicDiagramDefines::Kind kind,
                                     DynamicDiagramDefines::Volume volume)
{
    Q_ASSERT(data.size() == labels.size());

    DynamicDiagramPainter dp(painter, rect);
    dp.setKind(kind);
    dp.setVolume(volume);
    dp.setTitle(title);
    dp.setAxisSpaceLeft(30);
    dp.setAxisSpaceBottom(10);

    for (int i = 0; i < data.size(); ++i)
    {
        auto item = new DiagItem(data.at(i), labels.at(i));
        dp.appendItem(item);
    }
    dp.doPaint(ratio);
}

double ReportElements::ratio(const QRect paper, QWidget *widget, const double maxVal)
{
    double retval = static_cast<double>(paper.width()) / static_cast<double>(widget->geometry().width());
    if (static_cast<double>(paper.height()) / static_cast<double>(widget->geometry().height()) < retval)
        retval = static_cast<double>(paper.height()) / static_cast<double>(widget->geometry().height());
    if (maxVal > -1 && retval > maxVal) retval = maxVal;
    return retval;
}


