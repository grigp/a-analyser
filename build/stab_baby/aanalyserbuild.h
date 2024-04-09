#ifndef AANALYSERBUILD_H
#define AANALYSERBUILD_H

#include <QList>

#include "deviceprotocols.h"

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

/*!
 * \brief Регистрирует визуализаторы индивдуальных программ, вызывая метод PPVisualsFactory::registerVisual для каждого визуализатора
 */
void registerPPVisuals();

/*!
 * \brief Регистрирует преобразователи сигналов, вызывая метод SignalTransformFactory::registerTransformer для преобразователя
 */
void registerSignalTransformers();

/*!
 * \brief Возвращает список допустимых портов для драйвера по его uid
 * \param drvUid - uid драйвера
 */
QList<DeviceProtocols::Ports> getDriverPorts(const QString &drvUid);

/*!
 * \brief Создает список uid протоколов драйвера
 * \param drvUid - uid драйвера
 * \return список
 */
QStringList getDriverProtocols(const QString &drvUid);

/*!
 * \brief Редактирует параметры драйвера
 * \param drvUid - uid драйвера
 * \param params - параметры
 * \return true, если редактируемые параметры надо принять
 */
bool editDriverParams(const QString &drvUid, QJsonObject &params);

/*!
 * \brief Создает драйвер с заданным uid
 * \param drvUid - uid драйвера
 * \return  указатель на созданный драйвер
 */
Driver* createDriver(const QString &drvUid);

/*!
 * \brief Создает список драйверов
 * \param drivers - сам список
 */
void assignDrivers(QMap<QString, QString> &drivers);

/*!
 * \brief Возвращает список индивидуальных программ по умолчанию (папок программ)
 */
QStringList getDefaultPPList();

/*!
 * \brief Возвращает true, если приложение будет работать с индивидуальными программми и false в противном случае
 */
bool isPPEnabled();

}


#endif // AANALYSERBUILD_H
