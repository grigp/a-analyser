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
     * \brief Вызывает диалог редактирования параметров методики по uid методики
     * \param templateUid - uid шаблона методики
     * \param params - редактируемые параметры
     * \return true, если в диалоге нажали OK и параметры надо сохранить
     */
    bool editMetodicParams(QWidget *parent, const QString &metUid);

signals:

public slots:

private:
    void assignTemplates();
    void assignMetodics();
    void saveMetodics();

    int getMetodicIndexByUid(const QString &uid) const;

    QList<MetodicTemplate*> m_templates;
    QList<MetodicDefines::MetodicInfo> m_metodics;
};

#endif // METODICSFACTORY_H
