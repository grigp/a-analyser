#ifndef REPORTELEMENTS_H
#define REPORTELEMENTS_H

#include <QObject>
#include <QPainter>
#include <QStandardItemModel>

namespace ReportElements
{

/*!
 * \brief Рисует заголовок отчета.
 * Данные берет из настроек пользователя и текущего открытого теста
 * \param painter - указатель на текущий рисователь
 * \param testUid - uid открытого теста
 * \param rect - прямоугольник, в котором надо рисовать
 */
void drawHeader(QPainter *painter, const QString &testUid, QRect rect);

/*!
 * \brief Рисует заголовок отчета.
 * Данные берет из настроек пользователя
 * \param painter - указатель на текущий рисователь
 * \param testUid - uid открытого теста
 * \param rect - прямоугольник, в котором надо рисовать
 */
void drawFooter(QPainter *painter, const QString &testUid, QRect rect);

/*!
 * \brief Рисует виджет на отчете по образцу
 * \param painter - указатель на текущий рисователь
 * \param widget - рисуемый виджет
 * \param w, h - ширина и высота виджета
 * \param x, y - позиция виджета
 */
void drawWidget(QPainter *painter, QWidget *widget, const int w, const int h, const int x, const int y);

/*!
 * \brief Рисует таблицу на печатном отчете,
 * используя указатель на модель, содержащую данные таблицы
 * \param painter - указатель на текущий рисователь
 * \param model - указатель на модель
 * \param rect - прямоугольник, в котором надо рисовать
 * \param pointSize - Размер шрифта
 * \param weight - толщина шрифта в таблице (QFont::Bold)
 * \param titleWeight - толщина шрифта в заголовке (QFont::Bold)
 */
void drawTable(QPainter *painter, QStandardItemModel *model, QRect rect,
               QList<int> columnStretch = QList<int>(),
               int pointSize = -1, int weight = -1, int titleWeight = -1);


};

#endif // REPORTELEMENTS_H
