#ifndef SETMAXFORCEDIALOG_H
#define SETMAXFORCEDIALOG_H

#include <QDialog>

#include "basedefines.h"

namespace Ui {
class SetMaxForceDialog;
}

/*!
 * \brief Класс диалога выбора отклонения SetMaxForceDialog class
 */
class SetMaxForceDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Режим диалога Mode enum
     */
    enum Mode
    {
          CrossOneLine = 0   ///< Одно из ортагональных направлений
        , Triangle           ///< Треугольник (вверх, вниз вправо, вниз влево)
    };

    explicit SetMaxForceDialog(Mode mode, QWidget *parent = nullptr);
    ~SetMaxForceDialog() override;

    /*!
     * \brief Устанавливает направление, в котором будут проводиться измерения
     * \param direction - код направления
     */
    void setDirection(const BaseDefines::Directions direction);

    /*!
     * \brief Устанавливает заголовочный текст
     * \param text - текст
     */
    void setText(const QString text);

    /*!
     * \brief получение и отображение данных
     * \param x, y - координаты ЦД
     */
    void getData(const double x, const double y);

    /*!
     * \brief Возвращает значение максимального отклонения в единичном режиме
     */
    int value() const;

    /*!
     * \brief Возвращает значение максимального отклонения для заданного направления
     * \param direction - направление
     */
    int value(const int direction) const;

    /*!
     * \brief Устанавливает минимальное значение смещения при выборе диапазона отклонения
     * \param val - значение
     */
    void setMinValueOffset(const int val) {m_minValueOffset = val;}

public slots:
    void resetValue();

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    Ui::SetMaxForceDialog *ui;

    void getDataCrossOneLine(const int x, const int y);

    void getDataTriangle(const int x, const int y);

    BaseDefines::Directions m_direction;
    int m_minValueOffset {50};

    Mode m_mode {CrossOneLine};

    BaseDefines::RunningMode m_runningMode {BaseDefines::rmOperator};
    int m_autoTimeLatent {2};
    int m_autoModeTimerId {-1};   ///< id таймера для автозавершения в режиме автономной работы
};

#endif // SETMAXFORCEDIALOG_H
