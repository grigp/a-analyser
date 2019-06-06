#ifndef METODICSFACTORY_H
#define METODICSFACTORY_H

#include <QObject>

#include "metodicdefines.h"

class MetodicTemplate;

/*!
 * \brief Класс фабрики классов методик MetodicsFactory class
 */
class MetodicsFactory : public QObject
{
    Q_OBJECT
public:
    explicit MetodicsFactory(QObject *parent = nullptr);
    ~MetodicsFactory();

    /*!
     * \brief Возвращает кол-во методик
     */
    int metodicsCount() const;

    /*!
     * \brief Возвращает методику по номеру
     * \param i - номер методики
     */
    MetodicDefines::MetodicInfo metodic(const int i) const;

    /*!
     * \brief Возвращает методику по uid
     * \param metUid - uid методики
     */
    MetodicDefines::MetodicInfo metodic(const QString &metUid) const;

    /*!
     * \brief Вызывает диалог редактирования параметров методики по uid методики
     * \param parent - родительский виджет для вызываемого диалогового окна
     * \param metUid - uid методики
     * \return true, если в диалоге нажали OK и параметры надо сохранить
     */
    bool editMetodicParams(QWidget *parent, const QString &metUid);

    /*!
     * \brief выполнение методики
     * \param parent - родительский виджет для размещения виджета выплнения пробы
     * \param metUid - uid методики
     */
    void execute(QWidget *parent, const QString &metUid) const;

    /*!
     * \brief Визуализация результатов теста
     * \param parent - родительский виджет для размещения виджета результатов
     * \param testUid - uid теста
     * \return указатель на виджет результатов
     */
    QWidget *visualize(QWidget *parent, const QString &testUid) const;

signals:

public slots:

private:
    void assignTemplates();
    void assignMetodics();
    void saveMetodics();

    int getMetodicIndexByUid(const QString &uid) const;

    MetodicTemplate *getMetodicTemplate(const QString &metUid) const;

    QList<MetodicTemplate*> m_templates;
    QList<MetodicDefines::MetodicInfo> m_metodics;
};

#endif // METODICSFACTORY_H
