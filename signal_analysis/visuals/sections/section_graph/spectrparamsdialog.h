#ifndef SPECTRPARAMSDIALOG_H
#define SPECTRPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class SpectrParamsDialog;
}

/*!
 * \brief Класс диалога выбора параметров спектра The SpectrParamsDialog class
 */
class SpectrParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpectrParamsDialog(const int size, const int freq, QWidget *parent = nullptr);
    ~SpectrParamsDialog();

    int points() const;
    void setPoints(const int points);

    bool isAveraging() const;
    void setIsAveraging(const bool ia);

    int averagingOffset() const;
    void setAveragingOffset(const int offs);

    double maxFrequency() const;
    void setMaxFrequency(const double maxFreq);

    QJsonObject params() const;
    void setParams(const QJsonObject params);

private slots:
    void on_averagingClicked(bool val);
    void on_changeMaxFrequencyVal(double val);

private:
    Ui::SpectrParamsDialog *ui;

    void assignPointsCount();

    const int m_size {0};
    const int m_freq {50};
    double m_freqPrev {0};
};

#endif // SPECTRPARAMSDIALOG_H
