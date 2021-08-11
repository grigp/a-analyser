#ifndef TRENTETRISPATIENTWINDOW_H
#define TRENTETRISPATIENTWINDOW_H

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class TrenTetrisPatientWindow;
}

/*!
 * \brief Класс окна пациентов тренажеров типа Тетрис TrenTetrisPatientWindow class
 */
class TrenTetrisPatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrenTetrisPatientWindow(QWidget *parent = nullptr);
    ~TrenTetrisPatientWindow() override;

    void setScene(QGraphicsScene *scene);

    double prop() const;
    double propX() const;
    double propY() const;

    void setScore(const QString &score);
    void setDeleteRows(const QString &rows);

    void setSamplePixmap(const QPixmap &pixmap);

    QSize sceneSize() const;

protected:
    void resizeEvent(QResizeEvent* event) override;


private:
    Ui::TrenTetrisPatientWindow *ui;

    double m_prop = 1; ///< Пропорция для пересчера базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    double m_propX = 1;
    double m_propY = 1;
};

#endif // TRENTETRISPATIENTWINDOW_H
