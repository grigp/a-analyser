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
 * \brief возвращает путь к папке данных для всех приложений a-analyser
 */
QString aanalyserDataPath();

/*!
 * \brief возвращает путь к папке данных копии приложения
 */
QString appCopyPath();

/*!
 * \brief возвращает путь к папке с базой данных
 */
QString dataBasesPath();


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
