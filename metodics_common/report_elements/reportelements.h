#ifndef REPORTELEMENTS_H
#define REPORTELEMENTS_H

#include <QObject>
#include <QPainter>
#include <QStandardItemModel>

namespace ReportElements
{

namespace Table
{
/*!
 * \brief Вертикальное растяжение таблицы VerticalStretch enum
 */
enum VerticalStretch
{
      tvsStretched = 0  ///< Растянутый режим на всю высоту
    , tvsCompressed     ///< Строка за строкой с возможным пустым местом
};

}

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
               const Table::VerticalStretch vStretch = Table::tvsStretched,
               const int pointSize = -1, const int weight = -1, const int titleWeight = -1);

/*!
 * \brief Прорисовывает СКГ на заданной канве
 * \param painter - рисователь
 * \param rect - зона СКГ
 * \param probeId - номер пробы
 * \param channelId - идентификатор канала
 * \param testUid - uid теста
 */
void drawSKG(QPainter *painter,
             const QRect &rect,
             const QString &testUid,
             const QString &probeUid,
             const QString &channelId,
             const double ratio);


};

#endif // REPORTELEMENTS_H
