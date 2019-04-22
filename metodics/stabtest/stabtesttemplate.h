#ifndef STABTESTTEMPLATE_H
#define STABTESTTEMPLATE_H

#include <QObject>
#include "metodictemplate.h"

/*!
 * \brief Шаблон методики стабилографического тестирования StabTestTemplate class
 */
class StabTestTemplate : public MetodicTemplate
{
    Q_OBJECT
public:
    explicit StabTestTemplate(QObject *parent = nullptr);

    /*!
     * \brief Возвращает уникальный идектификатор методики
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
     *  \param params - параметры методики
     */
    QWidget *visualize(QWidget *parent, const QJsonObject &params)  override;

    /*!
     * \brief Редактирование параметров методики
     *  \param params - параметры методики
     *  \return true, если нужно сохранить параметры
     */
    bool editParams(QJsonObject &params)  override;
};

#endif // STABTESTTEMPLATE_H
