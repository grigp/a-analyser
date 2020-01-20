#ifndef METODICDEFINES_H
#define METODICDEFINES_H

#include <QString>
#include <QJsonObject>

namespace MetodicDefines
{
    static const QString MetUid_StabTest = "met_stabtest_uid";
    static const QString MetUid_TrenTakePut = "met_trentakeput_uid";
    static const QString MetUid_JumpTest = "met_jumptest_uid";
    static const QString MetUid_JumpHeightTest = "met_jumpheighttest_uid";
    static const QString MetUid_TeppingTest = "met_teppingtest_uid";
    static const QString MetUid_DropTest = "met_droptest_uid";

    static const QString MetName_StabTest = "Стабилометрическое тестирование";
    static const QString MetName_TrenTakePut = "Тренажер захвата - укладки";
    static const QString MetName_JumpTest = "Измерение высоты прыжка";
    static const QString MetName_JumpHeightTest = "Анализ высоты серии прыжков";
    static const QString MetName_TeppingTest = "Теппинг тест";
    static const QString MetName_DropTest = "Дроп тест";

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
