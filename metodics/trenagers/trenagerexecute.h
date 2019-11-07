#ifndef TRENAGEREXECUTE_H
#define TRENAGEREXECUTE_H

#include <QWidget>

namespace Ui {
class TrenagerExecute;
}

class TrenagerExecute : public QWidget
{
    Q_OBJECT

public:
    explicit TrenagerExecute(QWidget *parent = 0);
    ~TrenagerExecute();

private:
    Ui::TrenagerExecute *ui;
};

#endif // TRENAGEREXECUTE_H
