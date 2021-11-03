#ifndef REPORTELEMENTS_H
#define REPORTELEMENTS_H

#include <QObject>
#include <QPainter>

namespace ReportElements
{

void drawHeader(QPainter *painter, const QString &testUid, QRect rect);

void drawFooter(QPainter *painter, const QString &testUid, QRect rect);
};

#endif // REPORTELEMENTS_H
