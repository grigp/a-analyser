#ifndef IDSWIDGET_H
#define IDSWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QAudioOutput>
#include <QTimerEvent>

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
    explicit IDSWidget(QWidget *parent = 0);
    ~IDSWidget();

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

protected:
    void timerEvent(QTimerEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void splitterMoved(int pos,int index);
    void on_play(const double frequency);

private:
    Ui::IDSWidget *ui;

    void addFactorsFromMultifactor(IDSCalculator *calculator);

    void saveSplitterPosition();
    void restoreSplitterPosition();

    void initAudio(const double frequency);
    void doneAudio();

    void showSKG(const int probeNum);
    void showFDS(const int probeNum);

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
