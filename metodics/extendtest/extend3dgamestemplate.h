#ifndef EXTEND3DGAMESTEMPLATE_H
#define EXTEND3DGAMESTEMPLATE_H

#include <QObject>
#include "metodictemplate.h"
//#include "stabtestparams.h"

class TestCalculator;

/*!
 * \brief Шаблон методики теста с внешним приложением ExtendTestTemplate class
 */
class Extend3DGamesTemplate : public MetodicTemplate
{
    Q_OBJECT
public:
    explicit Extend3DGamesTemplate(QObject *parent = nullptr);

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
     * \brief Редактирование параметров методики
     *  \param params - параметры методики
     *  \return true, если нужно сохранить параметры
     */
    bool editParams(QWidget *parent, QJsonObject &params)  override;

    static TestCalculator* calculator(const QString &testUid);

};

#endif // EXTEND3DGAMESTEMPLATE_H