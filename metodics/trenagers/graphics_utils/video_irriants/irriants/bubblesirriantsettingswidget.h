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
    void on_MaxSizeChanged(int size);
    void on_btnSetColorClicked();


private:
    Ui::BubblesIrriantSettingsWidget *ui;

    void setSampleColor(const QColor color);

    BubblesIrriant* m_irritant {nullptr};
};

#endif // BUBBLESIRRIANTSETTINGSWIDGET_H
