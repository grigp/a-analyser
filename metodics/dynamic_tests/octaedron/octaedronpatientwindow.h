#ifndef OCTAEDRONPATIENTWINDOW_H
#define OCTAEDRONPATIENTWINDOW_H

#include <QDialog>

#include "stabdynamictestpatientwindow.h"
#include "basedefines.h"

namespace Ui {
class OctaedronPatientWindow;
}

class OctaedronPatientWindow : public StabDynamicTestPatientWindow
{
    Q_OBJECT

public:
    explicit OctaedronPatientWindow(const int diap, QList<BaseDefines::TargetInfo> targets, QWidget *parent = nullptr);
    ~OctaedronPatientWindow() override;

    /*!
     * \brief Установка маркера
     * \param x, y - координаты маркера
     */
    void setMarker(const double x, const double y) override;

    /*!
     * \brief Действия при начале записи
     */
    void run() override;

    /*!
     * \brief Действия при завершении / прерывании записи
     */
    void stop() override;

    /*!
     * \brief Перекрытая функция установки диапазона изменения координат
     * \param diap - диапазон
     */
    void setDiap(const int diap) override;

private:
    Ui::OctaedronPatientWindow *ui;

    void createTargets();

    int m_diap {128};
    QList<BaseDefines::TargetInfo> m_targets;
};

#endif // OCTAEDRONPATIENTWINDOW_H
