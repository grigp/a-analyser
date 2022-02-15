#ifndef TESTSTREEMODEL_H
#define TESTSTREEMODEL_H

#include <QStandardItemModel>

///! TODO: УДАЛИТЬ, КАК НЕИСПОЛЬЗУЕМЫЙ

/*!
 * \brief Класс модели данных дерева открытых тестов The TestsTreeModel class
 */
class TestsTreeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit TestsTreeModel(QObject *parent = nullptr);

    ///< Номера колонок
    enum Columns
    {
          ColElement = 0
        , ColCloseBtn
    };

    ///< Роли для доступа к данным
    enum TestsTreeModelRoles
    {
          UidRole = Qt::UserRole + 1  ///< uid элемента (теста, пробы, канала). QString в итеме элеметнов ColElement
        , ChannelUidRole              ///< uid канала, именно реальной записи канала, а не channelID
    };

    /*!
     * \brief Загружает модель, используя список uid последних открытых тестов
     */
    void load();

    /*!
     * \brief Открывает тест и добавляет его в дерево
     * \param testUid - uid открываемого теста
     */
    void openTest(const QString &testUid);

    /*!
     * \brief Закрывает тест
     * \param testUid - uid закрываемого теста
     */
    void closeTest(const QString &testUid);

    /*!
     * \brief Возвращает список uid открытых тестов
     */
    QList<QString> getTests();

    /*!
     * \brief Возвращает true, если тест с заданным uid открыт
     * \param testUid - uid теста
     */
    bool isTestOpened(const QString &testUid);

private:
    void doOpenTest(const QString &testUid);

    QString getMethodName(const QString& metUid);
};

#endif // TESTSTREEMODEL_H
