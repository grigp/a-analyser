#ifndef AANALYSERBUILD_H
#define AANALYSERBUILD_H

#include <QList>

class MetodicTemplate;

namespace AAnalyserBuild
{

/*!
 * \brief Возвращает список шаблонов методик для данной сборки
 */
QList<MetodicTemplate*> getBuildTemplates(QObject *parent = nullptr);

/*!
 * \brief Регистрирует показатели, вызывая статический метод registerFactors() для всех факторсгрупп
 */
void registerFactors();


}


#endif // AANALYSERBUILD_H
