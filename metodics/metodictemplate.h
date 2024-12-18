#ifndef METODICTEMPLATE_H
#define METODICTEMPLATE_H

#include <QObject>
#include <QJsonObject>
#include <QPainter>
#include <QPrinter>

/*!
 * \brief Базовый класс шаблона методики MetodicTemplate class
 */
class MetodicTemplate : public QObject
{
    Q_OBJECT
public:
    explicit MetodicTemplate(QObject *parent = nullptr);

    /*!
     * \brief Возвращает уникальный идектификатор методики
     */
    virtual QString uid() = 0;

    /*!
     * \brief Возвращает название методики
     */
    virtual QString name() = 0;

    /*!
     * \brief Создает и возвращает виджет выполнения теста по методике
     *  \param params - параметры методики
     */
    virtual QWidget *execute(QWidget *parent, const QJsonObject &params) = 0;

    /*!
     * \brief Создает и возвращает виджет визуализации результатов тестирования по методике
     *  \param testUid - uid теста
     */
    virtual QWidget *visualize(QWidget *parent, const QString &testUid) = 0;

    /*!
     * \brief Прорисовывает предварительные результаты с помощью заданного рисователя
     * \param painter - рисователь
     * \param rect - зона прорисовки
     * \param testUid - uid теста
     */
    virtual void paintPreview(QPainter* painter, QRect& rect, const QString& testName) = 0;

    /*!
     * \brief Печать результатов теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    virtual void print(QPrinter *printer, const QString &testUid) = 0;

    /*!
     * \brief Редактирование параметров методики
     *  \param params - параметры методики
     *  \return true, если нужно сохранить параметры
     */
    virtual bool editParams(QWidget *parent, QJsonObject &params, const QString& name = "") = 0;

signals:

public slots:
};

#endif // METODICTEMPLATE_H
