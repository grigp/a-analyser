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

    enum StandardButton {
        NoButton           = QMessageBox::NoButton,
        Ok                 = QMessageBox::Ok,
        Save               = QMessageBox::Save,
        SaveAll            = QMessageBox::SaveAll,
        Open               = QMessageBox::Open,
        Yes                = QMessageBox::Yes,
        YesToAll           = QMessageBox::YesToAll,
        No                 = QMessageBox::No,
        NoToAll            = QMessageBox::NoToAll,
        Abort              = QMessageBox::Abort,
        Retry              = QMessageBox::Retry,
        Ignore             = QMessageBox::Ignore,
        Close              = QMessageBox::Close,
        Cancel             = QMessageBox::Cancel,
        Discard            = QMessageBox::Discard,
        Help               = QMessageBox::Help,
        Apply              = QMessageBox::Apply,
        Reset              = QMessageBox::Reset,
        RestoreDefaults    = QMessageBox::RestoreDefaults,
        UserButton         = 0x10000000,   ///< Кнопка, определенная пользователем

        FirstButton        = Ok,                // internal
        LastButton         = RestoreDefaults,   // internal

        YesAll             = YesToAll,          // obsolete
        NoAll              = NoToAll,           // obsolete

        Default            = QMessageBox::Default,        // obsolete
        Escape             = QMessageBox::Escape,        // obsolete
        FlagMask           = QMessageBox::FlagMask,        // obsolete
        ButtonMask         = ~FlagMask          // obsolete
    };
    Q_DECLARE_FLAGS(StandardButtons, StandardButton)
    Q_FLAG(StandardButtons)


    static StandardButton information(QWidget *parent, const QString &title,
         const QString &text, StandardButtons buttons = Ok,
         StandardButton defaultButton = NoButton, const QString& textUserButton = "");
    static StandardButton question(QWidget *parent, const QString &title,
         const QString &text, StandardButtons buttons = StandardButtons(Yes | No),
         StandardButton defaultButton = NoButton, const QString& textUserButton = "");
    static StandardButton warning(QWidget *parent, const QString &title,
         const QString &text, StandardButtons buttons = Ok,
         StandardButton defaultButton = NoButton, const QString& textUserButton = "");
    static StandardButton critical(QWidget *parent, const QString &title,
         const QString &text, StandardButtons buttons = Ok,
         StandardButton defaultButton = NoButton, const QString& textUserButton = "");


    StandardButton result() {return m_result;}

private slots:
    void on_OKClicked();
    void on_NoClicked();
    void on_CancelClicked();
    void on_userClick();

private:
    Ui::AMessageBox *ui;

    static StandardButton message(KindMessage kind, QWidget *parent, const QString &title,
         const QString &text, StandardButtons buttons = Ok,
         StandardButton defaultButton = NoButton, const
                                  QString& textUserButton = "");


    void setTitle(const QString& title);
    void setText(const QString& text);
    void setTextUserButton(const QString& text);
    void setButtons(StandardButtons buttons);

    KindMessage m_kind;
    StandardButton m_result;
    StandardButtons m_buttons;
    QString m_textUserButton {""};
};

#endif // AMESSAGEBOX_H
