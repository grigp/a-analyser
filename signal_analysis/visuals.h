#ifndef VISUALS_H
#define VISUALS_H

#include <QObject>

class Visual;

/*!
 * \brief Класс описателя базового класса визуализаторов The BaseVisual class
 */
class BaseVisual
{
public:
    BaseVisual();
    virtual ~BaseVisual();

    virtual QString uid() = 0;
    virtual QString name() = 0;

    virtual bool isValid() = 0;
    virtual Visual* getVisualWidget() = 0;
};

/*!
 * \brief Класс описателя базового класса визуализаторов уровня теста The TestVisual class
 */
class TestVisual : public BaseVisual
{
public:
    TestVisual(const QString& testUid);

    QString testUid() {return m_testUid;}

private:
    QString m_testUid {""};
};


/*!
 * \brief Класс описателя базового класса визуализаторов уровня пробы The ProbeVisual class
 */
class ProbeVisual : public TestVisual
{
public:
    ProbeVisual(const QString& testUid, const QString& probeUid);

    QString probeUid() {return m_probeUid;}

private:
    QString m_probeUid {""};
};

/*!
 * \brief Класс описателя базового класса визуализаторов уровня канала The ChannelVisual class
 */
class ChannelVisual : public ProbeVisual
{
public:
    ChannelVisual(const QString& testUid, const QString& probeUid, const QString& channelUid);

    QString channelUid() {return m_channelUid;}

private:
    QString m_channelUid {""};
};

#endif // VISUALS_H
