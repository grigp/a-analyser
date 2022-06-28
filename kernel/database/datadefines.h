#ifndef DATADEFINES_H
#define DATADEFINES_H

#include <QString>
#include <QMap>
#include <QDate>
#include <QStandardPaths>
#include <QApplication>
#include <QCoreApplication>
#include <QJsonObject>

/*!
 * Набор типов данных из БД
 */
namespace DataDefines
{

static const QString DataBaseVersionCode = "1.0";

/*!
 * \brief Пол пациента
 */
enum Sex
{
      male
    , female
};

static QMap<Sex, QString> SexToText{
    std::pair<Sex, QString> (male, QCoreApplication::tr("муж"))
  , std::pair<Sex, QString> (female, QCoreApplication::tr("жен"))
};

/*!
 * \brief Карточка пациента
 */
struct PatientKard
{
    QString uid;
    QString fio;
    QDate born;
    Sex sex;
    int massa;
    int height;
    PatientKard() {}
};

/*!
 * \brief Структура информации о тесте TestInfo struct
 */
struct TestInfo
{
    QString uid;              ///< uid теста
    QString patientUid;       ///< uid пациента
    QString metodUid;         ///< uid методики
    QDateTime dateTime;       ///< дата и время проведения
    QStringList probes;       ///< список проб
    QJsonObject params;       ///< параметры на момент проведения
    QString comment;          ///< комментарий к тесту
    QString condition;        ///< условия проведения
    bool isNormContained;     ///< нормообразующее
    bool isOpening;           ///< открывающийся. Ставится в false перед открытием и в true после него.
                              ///< Если перед открытием false, то предыдущая попытка была неудачной

    TestInfo() {}
};

/*!
 * \brief Структура информации о канале ChannelInfo struct
 */
struct ChannelInfo
{
    QString uid;          ///< uid канала
    QString channelId;    ///< Идентификатор канала
    ChannelInfo(QString _uid, QString chId)
        : uid(_uid), channelId(chId)
    {}
    ChannelInfo() {}
};

/*!
 * \brief Структура информации о пробе ProbeInfo struct
 */
struct ProbeInfo
{
    QString uid;                  ///< uid пробы
    QString name;                 ///< название пробы
    int step;                     ///< номер пробы в тесте
    QString testUid;              ///< uid теста
    QList<ChannelInfo> channels;  ///< каналы

    ProbeInfo() {}
};

/*!
 * \brief Структура, содержащая информацию о базе данных и ее расположении DatabaseInfo struct
 */
struct DatabaseInfo
{
    QString name;        ///< Название папки с БД
    QString comment;     ///< Комментарий
    QString version;     ///< Версия БД
    DatabaseInfo() {}
};

/*!
 * \brief Структура, содержащая информацию об условиях проведения теста TestConditionInfo struct
 */
struct TestConditionInfo
{
    QString uid;             ///< uid условия проведения
    QString name;            ///< название
    QString description;     ///< описание
    bool norms_enabled;      ///< можно ли строить нормы для данного условия проведения
    TestConditionInfo() {}
};

/*!
 * \brief Структура, содержащая информацию об индивидуальной норме PersonalNormInfo struct
 */
struct PersonalNormInfo
{
    QString patientUid;       ///< uid пациента
    QString methodicUid;      ///< uid методики
    QString conditionUid;     ///< uid условия проведения
    QString factorUid;        ///< uid показателя
    int probeNum;             ///< номер пробы
    double value;             ///< значение нормы
    double stdDeviation;      ///< среднеквадратическое отклонение
    PersonalNormInfo() {}
};

/*!
 * \brief Структура, содержащая информацию о групповой норме GroupNormInfo struct
 */
struct GroupNormInfo
{
    QString methodicUid;       ///< uid методики
    QString conditionUid;      ///< uid условия проведения
    QString factorUid;         ///< uid показателя
    int probeNum;              ///< номер пробы
    double border;             ///< Значение порога нормы
    double conditionBorder;    ///< Значение порога условной нормы
};

/*!
 * \brief Варианты значений нормы NormValue enum
 */
enum NormValue
{
      MissingNorm = -2        ///< Норма отсутствует
    , NotNormal = -1          ///< Значение не в норме
    , ConditionNormal = 0     ///< Значение в условной норме
    , Normal = 1              ///< Значение в норме
};

/*!
 * \brief возвращает путь к папке данных для всех приложений А-Мед
 */
QString amedDataPath();

/*!
 * \brief возвращает путь к папке данных для всех приложений a-analyser
 */
QString aanalyserDataPath();

/*!
 * \brief возвращает путь к папке документов для всех приложений a-analyser
 */
QString aanalyserDocumentsPath();

/*!
 * \brief возвращает путь к папке данных копии приложения
 */
QString appCopyPath();

/*!
 * \brief возвращает путь к папке с базой данных
 */
QString dataBasesPath();

/*!
 * \brief возвращает путь к папке сводок для всех приложений a-analyser
 */
QString aanalyserSummariesPath();

/*!
 * \brief возвращает путь к папке временных файлов для всех приложений a-analyser
 */
QString aanalyserTemporaryPath();

/*!
 * \brief Возвращает uid копии приложения
 */
QString appCopyUid();

/*!
 * \brief Возвращает список баз данных, доступных для подключения
 */
QList<DatabaseInfo> getDatabases();

/*!
 * \brief Возвращает данные о БД по пути к ней
 * \param dbFolder - путь к БД
 */
DatabaseInfo getDatabaseInfo(const QString &dbFolder);

/*!
 * \brief Изменяет комментарий для базы данных
 * \param dbFolder - путь к БД
 * \param comment - новый комментарий
 */
void setDatabaseComment(const QString &dbFolder, const QString &comment);

/*!
 * \brief По коду значения нормы возвращает строковое значение нормы
 * \param val - код нормы
 */
QString normValueToString(const NormValue val);

/*!
 * \brief По коду значения нормы возвращает цвет прорисовки нормы
 * \param val - код нормы
 */
QColor normValueToColor(const NormValue val);

/*!
 * \brief По коду значения нормы возвращает темный цвет прорисовки нормы
 * \param val - код нормы
 */
QColor normValueToColorDark(const NormValue val);

///< ----------------------------------------------------------------
///< Локализация

static const QString LANG_CODE_RUS = "ru_RU";
static const QString LANG_CODE_ENGUSA = "en_US";

static QMap<QString, QString> LanguageName{
    std::pair<QString, QString> (LANG_CODE_RUS, QCoreApplication::tr("Русский"))
  , std::pair<QString, QString> (LANG_CODE_ENGUSA, QCoreApplication::tr("English (USA)"))
};

///< ----------------------------------------------------------------

}

#endif // DATADEFINES_H
