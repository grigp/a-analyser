#ifndef TESTPROPERTYDIALOG_H
#define TESTPROPERTYDIALOG_H

#include <QDialog>

namespace Ui {
class TestPropertyDialog;
}

class TestPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestPropertyDialog(QWidget *parent = 0);
    ~TestPropertyDialog();

    QString comment();
    void setComment(const QString &comment);

    QString condition();
    void setCondition(const QString &condition);

    bool normContained();
    void setNormContained(const bool nc);

private:
    Ui::TestPropertyDialog *ui;
};

#endif // TESTPROPERTYDIALOG_H
