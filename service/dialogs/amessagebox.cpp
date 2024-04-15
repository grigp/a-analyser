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

AMessageBox::StandardButton AMessageBox::information(QWidget *parent,
                                                     const QString &title,
                                                     const QString &text,
                                                     StandardButtons buttons,
                                                     StandardButton defaultButton)
{
    return message(Information, parent, title, text, buttons, defaultButton);
}

AMessageBox::StandardButton AMessageBox::question(QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  StandardButtons buttons,
                                                  StandardButton defaultButton)
{
    return message(Question, parent, title, text, buttons, defaultButton);
}

AMessageBox::StandardButton AMessageBox::warning(QWidget *parent,
                                                 const QString &title,
                                                 const QString &text,
                                                 StandardButtons buttons,
                                                 StandardButton defaultButton)
{
    return message(Warning, parent, title, text, buttons, defaultButton);
}

AMessageBox::StandardButton AMessageBox::critical(QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  StandardButtons buttons,
                                                  StandardButton defaultButton)
{
    return message(Critical, parent, title, text, buttons, defaultButton);
}

void AMessageBox::on_OKClicked()
{
    if ((m_buttons & Ok) != 0)
        m_result = Ok;
    else
    if ((m_buttons & Yes) != 0)
        m_result = Yes;
    else
        m_result = NoButton;

    accept();
}

void AMessageBox::on_NoClicked()
{
    if ((m_buttons & No) != 0)
        m_result = No;
    else
        m_result = NoButton;

    reject();
}

void AMessageBox::on_CancelClicked()
{
    if ((m_buttons & Cancel) != 0)
        m_result = Cancel;
    else
        m_result = NoButton;

    reject();
}

AMessageBox::StandardButton AMessageBox::message(AMessageBox::KindMessage kind,
                                                 QWidget *parent,
                                                 const QString &title,
                                                 const QString &text,
                                                 StandardButtons buttons,
                                                 StandardButton defaultButton)
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

void AMessageBox::setButtons(StandardButtons buttons)
{
    m_buttons = buttons;
    if ((buttons & Ok) != 0)
        ui->btnOK->setText("OK");
    else
    if ((buttons & Yes) != 0)
        ui->btnOK->setText("Да");

    if ((buttons & No) != 0)
        ui->btnNo->setText("Нет");

    if ((buttons & Cancel) != 0)
        ui->btnCancel->setText("Отмена");

    if (((buttons & Ok) == 0) &&
        ((buttons & Yes) == 0))
        ui->btnOK->setVisible(false);
    if ((buttons & No) == 0)
        ui->btnNo->setVisible(false);
    if ((buttons & Cancel) == 0)
        ui->btnCancel->setVisible(false);
}

