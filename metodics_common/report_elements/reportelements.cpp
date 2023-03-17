#include "reportelements.h"

#include <QApplication>
#include <QDebug>

#include "baseutils.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "settingsprovider.h"
#include "dataprovider.h"
#include "datadefines.h"
#include "skgpainter.h"
#include "stabilogram.h"

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
                      "a-analyser 1.0   " +
                      email->value().toString() + "    " +
                      website->value().toString());
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
        auto title = model->headerData(j, Qt::Horizontal).toString();
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

void ReportElements::drawSKG(QPainter *painter,
                             const QRect &rect,
                             const QString &testUid,
                             const QString &probeUid,
                             const QString &channelId,
                             const double ratio)
{
    Q_UNUSED(testUid);
    SKGPainter skg(painter, rect);
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid, channelId, baStab))
    {
        Stabilogram stab(baStab);
        skg.setSignal(&stab);
        auto diap = BaseUtils::scaleAbove(stab.absMaxValue());
        skg.setDiap(diap);
        skg.doPaint(ratio);
    }
}
