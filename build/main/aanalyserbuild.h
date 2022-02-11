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


/*!
 * \brief Регистрирует визуализаторы, вызывая метод VisualsFactory::registerVisual для каждого визуализатора
 */
void registerVisuals();

}


#endif // AANALYSERBUILD_H
