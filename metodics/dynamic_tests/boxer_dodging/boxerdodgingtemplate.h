#ifndef BOXERDODGINGTEMPLATE_H
#define BOXERDODGINGTEMPLATE_H

#include <QObject>

#include "metodictemplate.h"

/*!
 * \brief Класс шаблона методики тренажера с уклонением для боксеров BoxerDodgingTemplate class
 */
class BoxerDodgingTemplate : public MetodicTemplate
{
    Q_OBJECT
public:
    explicit BoxerDodgingTemplate(QObject *parent = nullptr);

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
     * \brief Редактирование параметров методики
     *  \param params - параметры методики
     *  \return true, если нужно сохранить параметры
     */
    bool editParams(QWidget *parent, QJsonObject &params)  override;
};

#endif // BOXERDODGINGTEMPLATE_H