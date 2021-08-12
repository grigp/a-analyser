#ifndef TRENAGERPATIENTWINDOW_H
#define TRENAGERPATIENTWINDOW_H

#include <QDialog>
#include <QGraphicsScene>

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

    void setSamplePixmap(const QPixmap &pixmap);

    QSize sceneSize() const;

protected:
    void resizeEvent(QResizeEvent* event) override;


private:
    Ui::TrenagerPatientWindow *ui;

    double m_prop = 1; ///< Пропорция для пересчера базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    double m_propX = 1;
    double m_propY = 1;
};

#endif // TRENAGERPATIENTWINDOW_H
