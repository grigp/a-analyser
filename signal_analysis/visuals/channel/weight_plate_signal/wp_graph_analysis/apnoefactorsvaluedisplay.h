#ifndef APNOEFACTORSVALUEDISPLAY_H
#define APNOEFACTORSVALUEDISPLAY_H

#include <QDialog>

namespace Ui {
class ApnoeFactorsValueDisplay;
}

class ApnoeFactorsValueDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit ApnoeFactorsValueDisplay(QWidget *parent = nullptr);
    ~ApnoeFactorsValueDisplay();

    /*!
     * \brief Присваивает и выводит значения показателей апноэ
     * \param count - кол-во приступов
     * \param timeAverage - средняя длительность приступа
     * \param timeMax - максимальная длительность приступа
     */
    void assignValues(const int count, const double timeAverage, const double timeMax);

private:
    Ui::ApnoeFactorsValueDisplay *ui;
};

#endif // APNOEFACTORSVALUEDISPLAY_H
