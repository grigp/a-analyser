#ifndef JUMPTESTTEMPLATE_H
#define JUMPTESTTEMPLATE_H

#include <QObject>
#include <QJsonObject>

#include "metodictemplate.h"

class JumpTestTemplate : public MetodicTemplate
{
    Q_OBJECT
public:
    explicit JumpTestTemplate(QObject *parent = nullptr);

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

#endif // JUMPTESTTEMPLATE_H
