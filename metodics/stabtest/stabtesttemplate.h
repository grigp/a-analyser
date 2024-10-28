#ifndef STABTESTTEMPLATE_H
#define STABTESTTEMPLATE_H

#include <QObject>
#include "metodictemplate.h"
#include "stabtestparams.h"

class TestCalculator;

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
     *  \param testUid - uid теста
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
    bool editParams(QWidget *parent, QJsonObject &params, const QString& testName)  override;

    static TestCalculator* calculator(const QString &testUid);

    /*!
     * \brief Возвращает список кодов типов проб
     */
    static QList<StabTestParams::ProbeKinds> probeKinds();

    /*!
     * \brief Возвращает название типа кода проб по коду
     * \param pk - код типа пробы
     */
    static QString getProbeKindName(StabTestParams::ProbeKinds pk);
};

#endif // STABTESTTEMPLATE_H
