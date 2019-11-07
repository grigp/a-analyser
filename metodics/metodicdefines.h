#ifndef METODICDEFINES_H
#define METODICDEFINES_H

#include <QString>
#include <QJsonObject>

namespace MetodicDefines
{
    static const QString MetUid_StabTest = "met_stabtest_uid";
    static const QString MetUid_JumpTest = "met_jumptest_uid";
    static const QString MetUid_TeppingTest = "met_teppingtest_uid";
    static const QString MetUid_Trenager = "met_trenager_uid";

    static const QString MetName_StabTest = "Стабилометрическое тестирование";
    static const QString MetName_JumpTest = "Измерение высоты прыжка";
    static const QString MetName_TeppingTest = "Теппинг тест";
    static const QString MetName_Trenager = "Тренажер с БОС";

    /*!
     * \brief Структура описания методики MetodicInfo struct
     */
    struct MetodicInfo
    {
        QString uid;            ///< uid
        QString name;           ///< Название
        QString templateId;     ///< id шаблона методики
        int condition;          ///< Условия проведения
        bool buildNorms;        ///< Строить нормы по методике
        QJsonObject params;     ///< Параметры
        MetodicInfo()
        {
            uid = "";
            name = "";
            templateId = "";
            params = QJsonObject();
        }
    };

}

#endif // METODICDEFINES_H
