#ifndef IDSWIDGET_H
#define IDSWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QAudioOutput>
#include <QTimerEvent>
#include <QItemSelectionModel>
#include <QPrinter>

namespace Ui {
class IDSWidget;
}

class IDSCalculator;
class SoundGenerator;
class TestResultData;
class AnySignal;

/*!
 * \brief Класс виджета визуализатора динамической стабилизации в рамках стабилографического теста IDSWidget class
 */
class IDSWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IDSWidget(QWidget *parent = nullptr);
    ~IDSWidget() override;

    ///< Дополнительные роли для хранения данных в модели
    enum
    {
        FactorUidRole = Qt::UserRole + 0, ///< uid показателя Для колонок, начинающихся с 1 (пробы). QString
        FrequencyRole                     ///< Частота звука. Для колонок, начинающихся с 1 (пробы), строка, содержащая значение частоты звука. double
    };

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(IDSCalculator *calculator, const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    void print(QPrinter *printer, const QString &testUid);

protected:
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void splitterMoved(int pos,int index);
    void tableClicked(const QModelIndex &index);
    void on_play(const double frequency);
    void setVolume(int volume);

    void onSKGZoomIn();
    void onSKGZoomOut();
    void onSKGZeroing(bool isZeroing);

    void onFDSZoomIn();
    void onFDSZoomOut();

    void animToBegin();
    void animBack();
    void animPlay();
    void animStop();
    void animForward();

private:
    Ui::IDSWidget *ui;

    void addFactorsFromMultifactor(IDSCalculator *calculator);

    void saveSplitterPosition();
    void restoreSplitterPosition();

    void initAudio(const double frequency, const int duration);
    void startAudio();
    void assignVolume();
    void doneAudio();

    void showSKG(const int probeNum);
    void showFDS(const int probeNum);

    void actionsEnabledOnPlay(const bool play);

    void resizeColumns();

    void setSKGSize();

    QStandardItemModel m_mdlTable;

    int m_tmStopSound {-1};
    int m_curRow {-1};
    int m_curCol {-1};
    IDSCalculator *m_calculator {nullptr};
    QString m_testUid;
    TestResultData* m_trd {nullptr};
    AnySignal* m_fds {nullptr};

    SoundGenerator* m_soundGenerator {nullptr};
    QAudioOutput* m_audioOutput {nullptr};
    QAudioFormat m_audioFormat;
    int m_volume {50};

    ///< Анимация
    int m_tmAnimate {-1};
    int m_animCurPos {-1};
};


/*!
 * \brief Класс делегата, содержащего кнопку с запуском проигрывания звука PlaySoundDelegate class
 */
class PlaySoundDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PlaySoundDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

signals:
    void play(const double frequency) const;
};

#endif // IDSWIDGET_H
