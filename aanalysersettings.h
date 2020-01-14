#ifndef AANALYSERSETTINGS_H
#define AANALYSERSETTINGS_H

#include <QDialog>

namespace Ui {
class AAnalyserSettings;
}

/*!
 * \brief Класс диалога настройки общих параметров приложения AAnalyserSettings class
 * Пока простой
 */
class AAnalyserSettings : public QDialog
{
    Q_OBJECT

public:
    explicit AAnalyserSettings(QWidget *parent = 0);
    ~AAnalyserSettings();

public slots:
    int exec() override;
    void accept() override;


private:
    Ui::AAnalyserSettings *ui;

    void load();
    void save();
};

#endif // AANALYSERSETTINGS_H
