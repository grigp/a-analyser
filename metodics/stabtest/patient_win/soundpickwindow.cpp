#include "soundpickwindow.h"
#include "ui_soundpickwindow.h"

#include "metodicdefines.h"

SoundPickWindow::SoundPickWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::SoundPickWindow)
{
    ui->setupUi(this);

    m_player.setMedia(QUrl("qrc:/sound/03.wav"));
    ui->lblFrontComment->setStyleSheet(MetodicDefines::AutoModeMessageStylePatient);
    setFrontComment("");
}

SoundPickWindow::~SoundPickWindow()
{
    delete ui;
}

void SoundPickWindow::setParams(const QJsonObject &params)
{
    Q_UNUSED(params);
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

void SoundPickWindow::setFrontComment(const QString &comment)
{
    ui->lblFrontComment->setText(comment);
    ui->lblFrontComment->setVisible(comment != "");
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

void SoundPickWindow::resizeEvent(QResizeEvent *event)
{
    ui->lblFrontComment->setGeometry(ui->frMain->geometry());
    PatientWindow::resizeEvent(event);
}
