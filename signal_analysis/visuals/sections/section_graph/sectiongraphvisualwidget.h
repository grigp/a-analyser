#ifndef SECTIONGRAPHVISUALWIDGET_H
#define SECTIONGRAPHVISUALWIDGET_H

#include <QWidget>
#include <QMouseEvent>

#include "visuals.h"

namespace Ui {
class SectionGraphVisualWidget;
}

class AnySignal;
class DiagSpectr;

/*!
 * \brief Виджет визуализатора графика секции The SectionGraphVisualWidget class
 */
class SectionGraphVisualWidget : public SectionVisual
{
    Q_OBJECT

public:
    explicit SectionGraphVisualWidget(VisualDescriptor* visual,
                                      const QString& testUid, const QString& probeUid, const QString& channelId, const QString& sectionNumber,
                                      QWidget *parent = nullptr);
    ~SectionGraphVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnPlusClicked();
    void btnMinusClicked();

    void signalScroll(int pos);

    void on_popupMenuRequested(QPoint pos);

    void on_createSection();

    /*!
     * \brief Сигнал нажатия мышки на теле виджета
     * \param x, y - координаты нажатия
    * \param buttons - нажатые кнопки
     */
    void on_press(const int x, const int y);
    /*!
     * \brief Сигнал отпускания мышки на теле виджета
     * \param x, y - координаты отпускания
    * \param buttons - нажатые кнопки
     */
    void on_release(const int x, const int y);
    /*!
     * \brief Сигнал переноса мышки по телу виджета
     * \param x, y - координаты положения
    * \param buttons - нажатые кнопки
     */
    void on_move(const int x, const int y);

    void on_transformNew();
    void on_transformFile();
    void on_transformMemory();

    void on_revert();

    void on_calculateSpectr();

    void on_saveTransformToFile();
    void on_saveTransformToMemory();

private:
    Ui::SectionGraphVisualWidget *ui;

    void transform(QJsonArray &sheme);

    /*!
     * \brief Расчет спектра с усреднением
     * \param dataSrc - буфер исходного сигнала
     * \param dataRes - буфер преобразованного сигнала
     * \param points - кол-во точее
     * \param offset - смещение при усреднении
     */
    void computeSpectrAveraging(QVector<double> &dataSrc, QVector<double> &dataRes,
                                const int points, const int offset);

    /*!
     * \brief Расчет спектра с прореживанием
     * \param dataSrc - буфер исходного сигнала
     * \param dataRes - буфер преобразованного сигнала
     * \param points - кол-во точек
     */
    void computeSpectrDecimation(QVector<double> &dataSrc, QVector<double> &dataRes, const int points);

    void initData(QVector<double> &data, const int size);

    void showSpectr(DiagSpectr *area, QVector<double> &data, double &maxV,
                    const double duration, const double maxFreq);

    void updateSectionData();

    void setDiapazones();

    void selectionReset();

    void createBtnActions();

    bool m_isCalculate {false};
    AnySignal *m_signal {nullptr};
    double m_diap {0};

    ///< Выделение зоны на спектре
    DiagSpectr* m_selectAreaWidget {nullptr};  ///< Указатель на виджет
    int m_selAreaX {-1};                       ///< Начальная координата X
    int m_selAreaY {-1};                       ///< Начальная координата Y

    bool m_selectionProcess {false};
    QPoint m_selFrom = QPoint(-1, -1);
    QPoint m_selTo = QPoint(-1, -1);
};

#endif // SECTIONGRAPHVISUALWIDGET_H
