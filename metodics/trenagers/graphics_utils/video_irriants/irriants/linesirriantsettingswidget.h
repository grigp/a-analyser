#ifndef LINESIRRIANTSETTINGSWIDGET_H
#define LINESIRRIANTSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class LinesIrriantSettingsWidget;
}

class LinesIrriantSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LinesIrriantSettingsWidget(QWidget *parent = nullptr);
    ~LinesIrriantSettingsWidget();

private:
    Ui::LinesIrriantSettingsWidget *ui;
};

#endif // LINESIRRIANTSETTINGSWIDGET_H
