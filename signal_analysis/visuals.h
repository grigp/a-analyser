#ifndef VISUALS_H
#define VISUALS_H

#include <QWidget>

class VisualDescriptor;

/*!
 * \brief Базовый класс виджета визуализатора The Visual class
 */
class Visual : public QWidget
{
    Q_OBJECT

public:
    explicit Visual(VisualDescriptor* visual, QWidget *parent = nullptr);
    ~Visual() override;

    virtual bool isValid() = 0;
    virtual void calculate() = 0;

private:

    VisualDescriptor* m_visual {nullptr};
};

/*!
 * \brief Класс описателя базового класса визуализаторов уровня теста The TestVisual class
 */
class TestVisual : public Visual
{
public:
    TestVisual(VisualDescriptor* visual, const QString& testUid, QWidget *parent = nullptr);

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
    ProbeVisual(VisualDescriptor* visual, const QString& testUid, const QString& probeUid, QWidget *parent = nullptr);

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
    ChannelVisual(VisualDescriptor* visual, const QString& testUid, const QString& probeUid, const QString& channelId, QWidget *parent = nullptr);

    QString channelId() {return m_channelId;}

private:
    QString m_channelId {""};
};

/*!
 * \brief Класс описателя базового класса визуализаторов уровня секции The SectionVisual class
 */
class SectionVisual : public ChannelVisual
{
public:
    SectionVisual(VisualDescriptor* visual,
                  const QString& testUid, const QString& probeUid, const QString& channelId, const QString& sectionNumber,
                  QWidget *parent = nullptr);

    QString sectionNumber() {return m_sectionNumber;}

private:
    QString m_sectionNumber {""};
};

#endif // VISUALS_H
