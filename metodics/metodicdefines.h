#ifndef METODICDEFINES_H
#define METODICDEFINES_H

#include <QCoreApplication>
#include <QString>
#include <QList>
#include <QJsonObject>

namespace MetodicDefines
{
    static const QString MetUid_StabTest = "met_stabtest_uid";
    static const QString MetUid_TrenTakePut = "met_trentakeput_uid";
    static const QString MetUid_TrenTetris = "met_trentetris_uid";
    static const QString MetUid_Extend3DGamesTest = "met_external_3D_games_uid";
    static const QString MetUid_JumpTest = "met_jumptest_uid";
    static const QString MetUid_JumpHeightTest = "met_jumpheighttest_uid";
    static const QString MetUid_TeppingTest = "met_teppingtest_uid";
    static const QString MetUid_DropTest = "met_droptest_uid";

    static const QString MetUid_Cross = "met_cross_uid";
    static const QString MetUid_StepOffset = "met_step_offset_uid";
    static const QString MetUid_StepDeviatoin = "met_step_deviatoin_uid";
    static const QString MetUid_Triangle = "met_triangle_uid";
    static const QString MetUid_Evolventa = "met_evolventa_uid";
    static const QString MetUid_Octaedron = "met_octaedron_uid";
    static const QString MetUid_BoxerDodging = "met_boxer_dodging_uid";

    static const QString MetUid_BedsideScalesTester = "met_bedside_scales_tester_uid";

    static const QString MetName_StabTest = "Стабилометрическое тестирование";
    static const QString MetName_TrenTakePut = "Тренажер захвата - укладки";
    static const QString MetName_TrenTetris = "Тренажер \"Тетрис\"";
    static const QString MetName_Extend3DGamesTest = "Вызов внешней программы тестирования";
    static const QString MetName_JumpTest = "Измерение высоты прыжка";
    static const QString MetName_JumpHeightTest = "Анализ высоты серии прыжков";
    static const QString MetName_TeppingTest = "Теппинг тест";
    static const QString MetName_DropTest = "Дроп тест";

    static const QString MetName_Cross = "Тест на устойчивость";
    static const QString MetName_StepOffset = "Тест \"Ступени\"";
    static const QString MetName_StepDeviation = "Тест прироста";
    static const QString MetName_Triangle = "Тест \"Треугольник\"";
    static const QString MetName_Evolventa = "Тест \"Эвольвента\"";
    static const QString MetName_Octaedron = "Тест \"Октаэдр\"";
    static const QString MetName_BoxerDodging = "Тренажер с уклонением для боксеров";

    static const QString MetName_BedsideScalesTester = "Прикроватные весы. Испытания";

    /*!
     * \brief Структура описания методики MetodicInfo struct
     */
    struct MetodicInfo
    {
        QString uid;            ///< uid
        QString name;           ///< Название
        QString imageName;      ///< Название ресурса картинки
        QString templateId;     ///< id шаблона методики
        QString kindUid;        ///< uid типа методики
        int condition;          ///< Условия проведения
        bool buildNorms;        ///< Строить нормы по методике
        QJsonObject params;     ///< Параметры
        MetodicInfo()
        {
            uid = "";
            name = "";
            templateId = "";
            kindUid = "";
            params = QJsonObject();
        }
    };

    /*!
     * \brief Структура описания типа методики  MetodicKindInfo struct
     */
    struct MetodicKindInfo
    {
        QString uid;
        QString name;
        QString imageName;
        MetodicKindInfo()
        {
            uid = "";
            name = "";
        }
    };

    /*!
     * \brief Этапы статической пробы для автоматического режима The AutoModeStaticStages enum
     */
    enum AutoModeStaticStages
    {
          amssLatent0 = 0    ///< Латентный период от старта до начала центровки
        , amssZeroingWait    ///< Ожидание центровки
        , amssLatent1        ///< Латентный период от начала центровки до начала записи
        , amssRecordingWait  ///< Ожидание записи
        , amssRecording      ///< Запись
    };

    /*!
     * \brief Заголовки этапов статической пробы для автоматического режима The AutoModeStageTitle
     */
    static QMap<AutoModeStaticStages, QString> AutoModeStageTitle {
          std::pair<AutoModeStaticStages, QString> (amssLatent0, QCoreApplication::tr(""))
        , std::pair<AutoModeStaticStages, QString> (amssZeroingWait, QCoreApplication::tr("Центровка через"))
        , std::pair<AutoModeStaticStages, QString> (amssLatent1, QCoreApplication::tr(""))
        , std::pair<AutoModeStaticStages, QString> (amssRecordingWait, QCoreApplication::tr("Запись через"))
        , std::pair<AutoModeStaticStages, QString> (amssRecording, QCoreApplication::tr(""))
    };

    ///< Список этапов для статической пробы основной
    const QList<AutoModeStaticStages> AutoStagesBase {amssLatent0, amssZeroingWait, amssLatent1, amssRecordingWait, amssRecording};
    ///< Список этапов для статической пробы без центровки
    const QList<AutoModeStaticStages> AutoStagesWithoutZeroing {amssLatent0, amssRecordingWait, amssRecording};

    static const QString AutoModeMessageStyleMain = "font-size: 60pt; color: rgb(180, 0, 0);";
    static const QString AutoModeMessageStylePatient = "font-size: 68pt; color: rgb(180, 0, 0);";
}

#endif // METODICDEFINES_H
