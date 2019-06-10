#ifndef STABTESTVISUALIZE_H
#define STABTESTVISUALIZE_H

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class StabTestVisualize;
}

/*!
 * \brief Виджет показа результатов стабилометрического тестирования StabTestVisualize class
 */
class StabTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit StabTestVisualize(QWidget *parent = 0);
    ~StabTestVisualize();

    void setTest(const QString &testUid);

private:
    Ui::StabTestVisualize *ui;

    /*!
     * \brief Показывает сигналы в пробе
     * \param num - номер пробы
     * \param uid - uid пробы
     * \param edit - куда выводить данные
     */
    void showProbeResult(const int num, const QString uid, QTextEdit *edit);
};

#endif // STABTESTVISUALIZE_H
