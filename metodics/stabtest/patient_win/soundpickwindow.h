#ifndef SOUNDPICKWINDOW_H
#define SOUNDPICKWINDOW_H

#include <QDialog>
#include "patientwindow.h"

#include <QMediaPlayer>

namespace Ui {
class SoundPickWindow;
}

/*!
 * \brief Форма пациента "Звуковые сигалы" SoundPickWindow class
 */
class SoundPickWindow : public PatientWindow
{
    Q_OBJECT

public:
    explicit SoundPickWindow(QWidget *parent = nullptr);
    ~SoundPickWindow() override;

    /*!
     * \brief Задает параметры стимуляции
     */
    void setParams(const QJsonObject& params) override;

    void setDiap(const int diap) override;
    void setMarker(const double x, const double y) override;

    void run() override;
    void stop() override;

    /*!
     * \brief Функция, помещающая комментарий поверх окна
     * \param comment - текст комментария
     */
    void setFrontComment(const QString& comment) override;

    /*!
     * \brief Возвращает результат работы окна пациента
     */
    QVariant result() override;

protected:
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::SoundPickWindow *ui;

    QMediaPlayer m_player;
    int m_tm;
    int m_soundCount {0};
};

#endif // SOUNDPICKWINDOW_H
