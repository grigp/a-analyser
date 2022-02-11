#ifndef VISUALDESCRIPTOR_H
#define VISUALDESCRIPTOR_H

#include <QObject>

class Visual;

/*!
 * \brief Класс описателя визуализаторов The VisualDescriptor class
 */
class VisualDescriptor
{
public:
    /*!
     * \brief Уровень визуализатора The Level enum
     */
    enum Level
    {
          vlNone = 0    ///< Не задан
        , vlTest        ///< Теста
        , vlProbe       ///< Пробы
        , vlChannel     ///< Канала
    };

    VisualDescriptor(Level level);
    virtual ~VisualDescriptor();

    virtual QString uid() = 0;
    virtual QString name() = 0;

    virtual Visual* getVisualWidget(QWidget *parent = nullptr,
                                    const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") = 0;

    Level level() {return m_level;}

private:
    Level m_level {vlNone};
};

#endif // VISUALDESCRIPTOR_H
