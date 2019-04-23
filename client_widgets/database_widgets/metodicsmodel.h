#ifndef METODICSMODEL_H
#define METODICSMODEL_H

#include <QObject>
#include <QStandardItemModel>

class MetodicsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit MetodicsModel(QObject *parent = nullptr);

    ///< Номера колонок
    enum Columns
    {
          ColName = 0
    };

    enum MetodicsModelRoles
    {
          TemplateUidRole = Qt::UserRole + 1  ///< uid шаблона методики. QString в итеме названия ColName
        , MetodicUidRole                      ///< uid методики. QString в итеме названия ColName
        , MetodicParamsRole                   ///< параметры методики. QJsonObject в итеме названия ColName
    };

    /*!
     * \brief Редактировать параметры методики
     * \param rowNum - номер строки в списке методик
     */
    void editMetodicParams(QWidget *parent, const int rowNum);

    void load();
};

#endif // METODICSMODEL_H
