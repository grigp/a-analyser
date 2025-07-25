#ifndef TRENAGERPATIENTWINDOW_H
#define TRENAGERPATIENTWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QLabel>
#include <QList>

namespace Ui {
class TrenagerPatientWindow;
}

class TrenagerPatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrenagerPatientWindow(QWidget *parent = nullptr);
    ~TrenagerPatientWindow() override;

    void setScene(QGraphicsScene *scene);

    double prop() const;
    double propX() const;
    double propY() const;

    QSize sceneSize() const;

    /*!
     * \brief Добавляет метку для отображения игрового параметра
     * \param text - имя метки
     * \param styleSheet - визуальный стиль
     */
    void setGameParamLabel(const QString text, const QString styleSheet, const bool isVisible);
    /*!
     * \brief Изменяет значение текста метки игрового параметра по индексу параметра
     * \param idxParam - индекс параметра
     * \param value - значение
     */
    void setGameParamLabelValue(const int idxParam, const QString value);

    /*!
     * \brief Добваляет виджет для отображения подсказок
     * \param widget - указатель на добавляемый виджет
     */
    void addHintWidget(QWidget* widget);

    /*!
     * \brief Возвращает ширину фрейма параметров
     */
    int getFrameParamsWidth();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::TrenagerPatientWindow *ui;

    double m_prop = 1; ///< Пропорция для пересчера базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    double m_propX = 1;
    double m_propY = 1;

    QList<QLabel*> m_gameParamLabels;
};

#endif // TRENAGERPATIENTWINDOW_H
