#ifndef APNOEFACTORSVALUEDISPLAY_H
#define APNOEFACTORSVALUEDISPLAY_H

#include <QDialog>
#include <QTextEdit>

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

    /*!
     * \brief Задает список приступов апноэ
     * \param list - список
     */
    void setApnoeList(const QStringList &list);
    /*!
     * \brief Задает лог распознавания приступов апноэ
     * \param log - лог
     */
    void setLog(const QStringList &log);

private slots:
    void on_showDetail();
    void on_wtfApnoe();
    void on_wtfLog();


private:
    Ui::ApnoeFactorsValueDisplay *ui;

    void writeToFile(QTextEdit* text, const QString& subjName);
};

#endif // APNOEFACTORSVALUEDISPLAY_H
