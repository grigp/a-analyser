#ifndef STABDYNAMICTESTPATIENTWINDOW_H
#define STABDYNAMICTESTPATIENTWINDOW_H

#include <QDialog>

namespace Ui {
class StabDynamicTestPatientWindow;
}

class StabDynamicTestPatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StabDynamicTestPatientWindow(QWidget *parent = nullptr);
    ~StabDynamicTestPatientWindow();

    /*!
     * \brief Действия при начале записи
     */
    virtual void run();

    /*!
     * \brief Действия при завершении / прерывании записи
     */
    virtual void stop();

    /*!
     * \brief Виртуальная функция установки диапазона изменения координат
     * \param diap - диапазон
     */
    virtual void setDiap(const int diap) {m_diap = diap;}
    int diap() const {return  m_diap;}

    /*!
     * \brief Установка маркера
     * \param x, y - координаты маркера
     */
    virtual void setMarker(const double x, const double y);
    double x() const {return m_X;}
    double y() const {return m_Y;}

    /*!
     * \brief Управление видимостью маркера
     */
    virtual void setVisibleMarker(const bool isVisible);
    bool isVisibleMarker() const {return m_isVisibleMarker;}

private:
    Ui::StabDynamicTestPatientWindow *ui;

    int m_diap {128};
    double m_X {0};
    double m_Y {0};
    bool m_isVisibleMarker {false};
};

#endif // STABDYNAMICTESTPATIENTWINDOW_H
