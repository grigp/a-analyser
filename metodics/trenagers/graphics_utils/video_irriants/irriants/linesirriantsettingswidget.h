#ifndef LINESIRRIANTSETTINGSWIDGET_H
#define LINESIRRIANTSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class LinesIrriantSettingsWidget;
}

class LinesIrriant;

class LinesIrriantSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LinesIrriantSettingsWidget(LinesIrriant* iritant, QWidget *parent = nullptr);
    ~LinesIrriantSettingsWidget();

private slots:
    void on_cbDirectionChanged(int idx);
    void on_widthChanged(int value);
    void on_speedChanged(int speed);
    void on_dutyCicleChanged(int dc);
    void on_btnSetColorClicked();

private:
    Ui::LinesIrriantSettingsWidget *ui;

    LinesIrriant* m_irritant {nullptr};
};

#endif // LINESIRRIANTSETTINGSWIDGET_H
