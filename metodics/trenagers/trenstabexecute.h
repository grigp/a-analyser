#ifndef TRENSTABEXECUTE_H
#define TRENSTABEXECUTE_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

#include "trenexecute.h"

namespace Ui {
class TrenStabExecute;
}

/*!
 * \brief Базовый класс стабилографических тренажеров TrenStabExecute class
 */
class TrenStabExecute : public TrenExecute
{
    Q_OBJECT

public:
    explicit TrenStabExecute(QWidget *parent = nullptr);
    ~TrenStabExecute() override;

    void setParams(const QJsonObject &params) override;

protected slots:
    void start() override;

protected:
    /*!
     * \brief Заполнение своими управляющими элементами панели игровых управляющих элементов
     * \param frame - указатель на фрейм, куда добавлять
     */
    void fillGameControl(QFrame *frame) override;

    /*!
     * \brief Взаимодействие элементов
     * Все управление сценой, маркеры и т.д.
     * \param data - поступившие данные от прибора
     */
    void elementsInteraction(DeviceProtocols::DeviceData *data) override;

private slots:
    void on_zeroing();
    void on_scaleChange(int scaleIdx);

private:
    Ui::TrenStabExecute *ui;

    DeviceProtocols::DecartCoordControl* m_dcControl;  ///< Управление декартовыми данными в драйвере
    QPushButton* m_btnZeroing {nullptr};
    QComboBox* m_cbScale {nullptr};

    int m_scale {0};   ///< Индекс масштаба, установленный в параметрах
};

#endif // TRENSTABEXECUTE_H
