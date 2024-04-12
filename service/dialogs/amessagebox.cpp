#include "amessagebox.h"
#include "ui_amessagebox.h"

#include "aanalyserapplication.h"

#include <QDebug>

AMessageBox::AMessageBox(KindMessage kind, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AMessageBox)
  , m_kind(kind)
{
    ui->setupUi(this);

    if (m_kind == Information)
        ui->lblLogotip->setPixmap(QPixmap(":/images/info32.png"));
    else
    if (m_kind == Question)
        ui->lblLogotip->setPixmap(QPixmap(":/images/Question32.png"));
    else
    if (m_kind == Warning)
        ui->lblLogotip->setPixmap(QPixmap(":/images/Warning32.png"));
    else
    if (m_kind == Critical)
        ui->lblLogotip->setPixmap(QPixmap(":/images/Critical32.png"));

    setStyleSheet(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet());
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowFullscreenButtonHint);// | Qt::MSWindowsFixedSizeDialogHint);
}

AMessageBox::~AMessageBox()
{
    delete ui;
}

QMessageBox::StandardButton AMessageBox::information(QWidget *parent,
                                                     const QString &title,
                                                     const QString &text,
                                                     QMessageBox::StandardButtons buttons,
                                                     QMessageBox::StandardButton defaultButton)
{
    return message(Information, parent, title, text, buttons, defaultButton);
}

QMessageBox::StandardButton AMessageBox::question(QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton)
{
    return message(Question, parent, title, text, buttons, defaultButton);
}

QMessageBox::StandardButton AMessageBox::warning(QWidget *parent,
                                                 const QString &title,
                                                 const QString &text,
                                                 QMessageBox::StandardButtons buttons,
                                                 QMessageBox::StandardButton defaultButton)
{
    return message(Warning, parent, title, text, buttons, defaultButton);
}

QMessageBox::StandardButton AMessageBox::critical(QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton)
{
    return message(Critical, parent, title, text, buttons, defaultButton);
}

void AMessageBox::on_OKClicked()
{
    if ((m_buttons & QMessageBox::Ok) != 0)
        m_result = QMessageBox::Ok;
    else
    if ((m_buttons & QMessageBox::Yes) != 0)
        m_result = QMessageBox::Yes;
    else
        m_result = QMessageBox::NoButton;

    accept();
}

void AMessageBox::on_NoClicked()
{
    if ((m_buttons & QMessageBox::No) != 0)
        m_result = QMessageBox::No;
    else
        m_result = QMessageBox::NoButton;

    reject();
}

void AMessageBox::on_CancelClicked()
{
    if ((m_buttons & QMessageBox::Cancel) != 0)
        m_result = QMessageBox::Cancel;
    else
        m_result = QMessageBox::NoButton;

    reject();
}

QMessageBox::StandardButton AMessageBox::message(AMessageBox::KindMessage kind,
                                                 QWidget *parent,
                                                 const QString &title,
                                                 const QString &text,
                                                 QMessageBox::StandardButtons buttons,
                                                 QMessageBox::StandardButton defaultButton)
{
    Q_UNUSED(defaultButton);

    AMessageBox dlg(kind, parent);
    dlg.setTitle(title);
    dlg.setText(text);
    dlg.setButtons(buttons);
    dlg.exec();
    return dlg.result();
}

void AMessageBox::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void AMessageBox::setText(const QString &text)
{
    ui->lblComment->setText(text);
}

void AMessageBox::setButtons(QMessageBox::StandardButtons buttons)
{
    m_buttons = buttons;
    if ((buttons & QMessageBox::Ok) != 0)
        ui->btnOK->setText("OK");
    else
    if ((buttons & QMessageBox::Yes) != 0)
        ui->btnOK->setText("Да");

    if ((buttons & QMessageBox::No) != 0)
        ui->btnNo->setText("Нет");

    if ((buttons & QMessageBox::Cancel) != 0)
        ui->btnCancel->setText("Отмена");

    if (((buttons & QMessageBox::Ok) == 0) &&
        ((buttons & QMessageBox::Yes) == 0))
        ui->btnOK->setVisible(false);
    if ((buttons & QMessageBox::No) == 0)
        ui->btnNo->setVisible(false);
    if ((buttons & QMessageBox::Cancel) == 0)
        ui->btnCancel->setVisible(false);
}


//enum StandardButton {
//    // keep this in sync with QDialogButtonBox::StandardButton and QPlatformDialogHelper::StandardButton
//    NoButton           = 0x00000000,
//    Ok                 = 0x00000400,
//    Save               = 0x00000800,
//    SaveAll            = 0x00001000,
//    Open               = 0x00002000,
//    Yes                = 0x00004000,
//    YesToAll           = 0x00008000,
//    No                 = 0x00010000,
//    NoToAll            = 0x00020000,
//    Abort              = 0x00040000,
//    Retry              = 0x00080000,
//    Ignore             = 0x00100000,
//    Close              = 0x00200000,
//    Cancel             = 0x00400000,
//    Discard            = 0x00800000,
//    Help               = 0x01000000,
//    Apply              = 0x02000000,
//    Reset              = 0x04000000,
//    RestoreDefaults    = 0x08000000,

//    FirstButton        = Ok,                // internal
//    LastButton         = RestoreDefaults,   // internal

//    YesAll             = YesToAll,          // obsolete
//    NoAll              = NoToAll,           // obsolete

//    Default            = 0x00000100,        // obsolete
//    Escape             = 0x00000200,        // obsolete
//    FlagMask           = 0x00000300,        // obsolete
//    ButtonMask         = ~FlagMask          // obsolete
//};
