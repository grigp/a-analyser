#ifndef AMESSAGEBOX_H
#define AMESSAGEBOX_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AMessageBox;
}


/*!
 * \brief Класс сообщения о событии. Аналог QApplication::messageBox The AMessageBox class
 * Там сложности с русификацией
 */
class AMessageBox : public QDialog
{
    Q_OBJECT

public:

    /*!
     * \brief Типы сообщения The KindMessage enum
     */
    enum KindMessage
    {
          Information = 0   ///< Информация
        , Question          ///< Запрос
        , Warning           ///< Предупреждение
        , Critical          ///< Ошибка
    };

    explicit AMessageBox(KindMessage kind, QWidget *parent = nullptr);
    ~AMessageBox();

    static QMessageBox::StandardButton information(QWidget *parent, const QString &title,
         const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    static QMessageBox::StandardButton question(QWidget *parent, const QString &title,
         const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    static QMessageBox::StandardButton warning(QWidget *parent, const QString &title,
         const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    static QMessageBox::StandardButton critical(QWidget *parent, const QString &title,
         const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);


    QMessageBox::StandardButton result() {return m_result;}

private slots:
    void on_OKClicked();
    void on_NoClicked();
    void on_CancelClicked();

private:
    Ui::AMessageBox *ui;

    static QMessageBox::StandardButton message(KindMessage kind, QWidget *parent, const QString &title,
         const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);


    void setTitle(const QString& title);
    void setText(const QString& text);
    void setButtons(QMessageBox::StandardButtons buttons);

    KindMessage m_kind;
    QMessageBox::StandardButton m_result;
    QMessageBox::StandardButtons m_buttons;
};

#endif // AMESSAGEBOX_H
