#ifndef STEPOFFSETTEMPLATE_H
#define STEPOFFSETTEMPLATE_H

#include <QObject>

#include "metodictemplate.h"

class StepOffsetTemplate : public MetodicTemplate
{
    Q_OBJECT
public:
    explicit StepOffsetTemplate(QObject *parent = nullptr);

    /*!
     * \brief Возвращает уникальный идентификатор методики
     */
    QString uid() override;

    /*!
     * \brief Возвращает название методики
     */
    QString name()  override;

    /*!
     * \brief Создает и возвращает виджет выполнения теста по методике
     *  \param params - параметры методики
     */
    QWidget *execute(QWidget *parent, const QJsonObject &params)  override;

    /*!
     * \brief Создает и возвращает виджет визуализации результатов тестирования по методике
     *  \param testUid - параметры методики
     */
    QWidget *visualize(QWidget *parent, const QString &testUid)  override;

    /*!
     * \brief Прорисовывает предварительные результаты с помощью заданного рисователя
     * \param painter - рисователь
     * \param rect - зона прорисовки
     * \param testUid - uid теста
     */
    void paintPreview(QPainter* painter, QRect& rect, const QString& testUid) override;

    /*!
     * \brief Печать результатов теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    void print(QPrinter *printer, const QString &testUid) override;

    /*!
     * \brief Редактирование параметров методики
     *  \param params - параметры методики
     *  \return true, если нужно сохранить параметры
     */
    bool editParams(QWidget *parent, QJsonObject &params)  override;
};

#endif // STEPOFFSETTEMPLATE_H
