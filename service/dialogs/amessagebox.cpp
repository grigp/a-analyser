#include "amessagebox.h"
#include "ui_amessagebox.h"

#include "aanalyserapplication.h"

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
    Q_UNUSED(defaultButton);

    AMessageBox dlg(Information, parent);
    dlg.setTitle(title);
    dlg.setText(text);
    auto mr = dlg.exec();
    if (mr == QDialog::Accepted)
    {
        if ((buttons & QMessageBox::Ok) != 0)
            return QMessageBox::Ok;
        else
        if ((buttons & QMessageBox::Yes) != 0)
            return QMessageBox::Yes;
    }
    if (mr == QDialog::Rejected)
    {
        if ((buttons & QMessageBox::Cancel) != 0)
            return QMessageBox::Cancel;
        else
        if ((buttons & QMessageBox::No) != 0)
            return QMessageBox::No;
    }
    return QMessageBox::NoButton;
}

QMessageBox::StandardButton AMessageBox::question(QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton)
{
}

QMessageBox::StandardButton AMessageBox::warning(QWidget *parent,
                                                 const QString &title,
                                                 const QString &text,
                                                 QMessageBox::StandardButtons buttons,
                                                 QMessageBox::StandardButton defaultButton)
{

}

QMessageBox::StandardButton AMessageBox::critical(QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton)
{

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
    if ((buttons & QMessageBox::Ok) != 0)
        ui->btnOK->setText("OK");
    else
    if ((buttons & QMessageBox::Yes) != 0)
        ui->btnOK->setText("Да");

    if ((buttons & QMessageBox::Cancel) != 0)
        ui->btnCancel->setText("Отмена");
    else
    if ((buttons & QMessageBox::No) != 0)
        ui->btnCancel->setText("Нет");

    if (((buttons & QMessageBox::Cancel) == 0) &&
        ((buttons & QMessageBox::No) == 0))
    {
        ui->btnCancel->setVisible(false);
    }
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
