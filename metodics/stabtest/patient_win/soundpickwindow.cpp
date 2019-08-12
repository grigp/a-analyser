#include "soundpickwindow.h"
#include "ui_soundpickwindow.h"

SoundPickWindow::SoundPickWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::SoundPickWindow)
{
    ui->setupUi(this);

    m_player.setMedia(QUrl("qrc:/sound/03.wav"));
}

SoundPickWindow::~SoundPickWindow()
{
    delete ui;
}

void SoundPickWindow::setDiap(const int diap)
{
    Q_UNUSED(diap);
}

void SoundPickWindow::setMarker(const double x, const double y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
}

void SoundPickWindow::run()
{
    int interval = qrand() % (2000 - 750) + 750;
    m_tm = startTimer(interval);
}

void SoundPickWindow::stop()
{
    killTimer(m_tm);
}

QVariant SoundPickWindow::result()
{
    return m_soundCount;
}

void SoundPickWindow::timerEvent(QTimerEvent *event)
{
    m_player.play();
    ++m_soundCount;
    PatientWindow::timerEvent(event);
}
