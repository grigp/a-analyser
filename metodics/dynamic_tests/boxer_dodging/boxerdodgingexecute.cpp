#include "boxerdodgingexecute.h"

#include "boxerdodgingpatientwindow.h"
#include "aanalyserapplication.h"

#include <QDesktopWidget>
#include <QTimer>

BoxerDodgingExecute::BoxerDodgingExecute(QWidget *parent)
    : StabDynamicTestExecute (parent)
{
    setTitle(tr("Тренажер с уклонением для боксеров"));
    isShowValues(false);
    isTraceControl(false);
    setVisibleRecordLength(false);

    QTimer::singleShot(10, [=]
    {
        createMarker();
    });
}

BoxerDodgingExecute::~BoxerDodgingExecute()
{

}

void BoxerDodgingExecute::setParams(const QJsonObject &params)
{
    StabDynamicTestExecute::setParams(params);
}

StabDynamicTestPatientWindow *BoxerDodgingExecute::createPatientWindow()
{
//    m_patientWin = new OctaedronPatientWindow(static_cast<int>(m_radius * 1.2), m_targets, this);
//    return m_patientWin;

    return nullptr;
}

void BoxerDodgingExecute::finishTest()
{
    StabDynamicTestExecute::finishTest();
}

void BoxerDodgingExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);
}

void BoxerDodgingExecute::start()
{
    StabDynamicTestExecute::start();

    if (QApplication::desktop()->screenCount() > 1)
        createAndShowPatientWindow();
}

void BoxerDodgingExecute::recording()
{
    StabDynamicTestExecute::recording();

    if (isRecording())
    {
        if (QApplication::desktop()->screenCount() == 1)
            createAndShowPatientWindow();
    }
    else
    {
        if (QApplication::desktop()->screenCount() == 1)
            hidePatientWindow();
    }
}

void BoxerDodgingExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
}

void BoxerDodgingExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

void BoxerDodgingExecute::createMarker()
{
    addMarker();
}

void BoxerDodgingExecute::createAndShowPatientWindow()
{
    m_patientWin = new BoxerDodgingPatientWindow(this);

    if (m_patientWin)
    {
        auto rect = static_cast<AAnalyserApplication*>(QApplication::instance())->getPatientWindowGeometry();
        m_patientWin->resize(rect.size());
        m_patientWin->move(rect.x(), rect.y());
        m_patientWin->show();
    }
}

void BoxerDodgingExecute::hidePatientWindow()
{
    if (m_patientWin)
    {
        BoxerDodgingPatientWindow *pw = m_patientWin;
        m_patientWin = nullptr;
        delete pw;
    }
}
