#ifndef TRENSTABEXECUTE_H
#define TRENSTABEXECUTE_H

#include <QWidget>
#include <QPushButton>

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

protected:
    /*!
     * \brief Заполнение своими управляющими элементами панели игровых управляющих элементов
     * \param frame - указатель на фрейм, куда добавлять
     */
    void fillGameControl(QFrame *frame) override;


private slots:
    void on_zeroing();

private:
    Ui::TrenStabExecute *ui;

    QPushButton* m_btnZeroing {nullptr};

};

#endif // TRENSTABEXECUTE_H
