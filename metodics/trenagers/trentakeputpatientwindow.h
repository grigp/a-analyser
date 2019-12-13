#ifndef TRENTAKEPUTPATIENTWINDOW_H
#define TRENTAKEPUTPATIENTWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QResizeEvent>

namespace Ui {
class TrenTakePutPatientWindow;
}

/*!
 * \brief Класс окна пациентов тренажеров захвата - укладки TrenTakePutPatientWindow class
 */
class TrenTakePutPatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrenTakePutPatientWindow(QWidget *parent = 0);
    ~TrenTakePutPatientWindow();

    void setScene(QGraphicsScene *scene);

    double prop() const;
    double propX() const;
    double propY() const;

    void setScore(const QString &score);
    void setErrors(const QString &errors);

protected:
    void resizeEvent(QResizeEvent* event);


private:
    Ui::TrenTakePutPatientWindow *ui;
    double m_prop = 1; ///< Пропорция для пересчера базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    double m_propX = 1;
    double m_propY = 1;
};

#endif // TRENTAKEPUTPATIENTWINDOW_H
