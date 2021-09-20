#ifndef BUBBLESIRRIANTSETTINGSWIDGET_H
#define BUBBLESIRRIANTSETTINGSWIDGET_H

#include <QWidget>

class BubblesIrriant;

namespace Ui {
class BubblesIrriantSettingsWidget;
}

class BubblesIrriantSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BubblesIrriantSettingsWidget(BubblesIrriant* irritant, QWidget *parent = nullptr);
    ~BubblesIrriantSettingsWidget();

private slots:
    void on_bubblesCountChanged(int value);


private:
    Ui::BubblesIrriantSettingsWidget *ui;

    BubblesIrriant* m_irritant {nullptr};
};

#endif // BUBBLESIRRIANTSETTINGSWIDGET_H
