#ifndef BEDSIDESCALESTESTERTEMPLATE_H
#define BEDSIDESCALESTESTERTEMPLATE_H

#include <QObject>

#include "metodictemplate.h"


/*!
 * \brief Класс шаблона методики тестирования прикроватных весов The BedsideScalesTesterTemplate class
 */
class BedsideScalesTesterTemplate : public MetodicTemplate
{
    Q_OBJECT
public:
    explicit BedsideScalesTesterTemplate(QObject *parent = nullptr);

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
     *  \param testUid - uid теста
     */
    QWidget *visualize(QWidget *parent, const QString &testUid)  override;

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

signals:

public slots:
};

#endif // BEDSIDESCALESTESTERTEMPLATE_H