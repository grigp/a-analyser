#include "reportelements.h"

#include <QApplication>
#include <QDebug>

#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "settingsprovider.h"
#include "dataprovider.h"
#include "datadefines.h"

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
    painter->setFont(QFont("Sans",12,0,0));
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
    painter->drawLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y());

    painter->drawText(rect.x(), rect.y() + rect.height() / 2,
                      enterprice->value().toString() + "    " +
                      "a-analyser 1.0   " +
                      email->value().toString() + "    " +
                      website->value().toString());
}
