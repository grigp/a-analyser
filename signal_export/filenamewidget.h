#ifndef FILENAMEWIDGET_H
#define FILENAMEWIDGET_H

#include <QWidget>

namespace Ui {
class FileNameWidget;
}

/*!
 * \brief Класс виджета ввода имени файла The FileNameWidget class
 */
class FileNameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileNameWidget(QWidget *parent = nullptr);
    ~FileNameWidget();

    void setTitle(const QString &title);

    QString fileName() const;
    void setFileName(const QString &fn);

private slots:
    void browseFile();

private:
    Ui::FileNameWidget *ui;
};

#endif // FILENAMEWIDGET_H
