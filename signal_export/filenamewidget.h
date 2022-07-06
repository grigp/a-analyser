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
    /*!
     * \brief Режим выбора The Mode enum
     */
    enum Mode
    {
          mdNone = 0   ///< Не задан
        , mdFile       ///< файла
        , mdFolder     ///< папки
    };

    explicit FileNameWidget(const Mode mode, QWidget *parent = nullptr);
    ~FileNameWidget() override;

    void setTitle(const QString &title);

    QString fileName() const;
    void setFileName(const QString &fn);

    bool active() const {return m_active;}
    void setActive(const bool active);

private slots:
    void browseFile();

private:
    Ui::FileNameWidget *ui;

    Mode m_mode {mdNone};
    bool m_active {true};
};

#endif // FILENAMEWIDGET_H
