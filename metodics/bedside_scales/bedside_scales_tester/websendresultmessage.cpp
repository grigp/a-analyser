#include "websendresultmessage.h"
#include "ui_websendresultmessage.h"

#include <QTextStream>
#include <QFileDialog>

#include "aanalyserapplication.h"

namespace
{
static QMap<WebSendResultMessage::MessageType, QString> ResultComment {
    std::pair<WebSendResultMessage::MessageType, QString> (WebSendResultMessage::ResultSucces, "Данные переданы успешно")
  , std::pair<WebSendResultMessage::MessageType, QString> (WebSendResultMessage::ResultError, "Ошибка передачи данных")
};

}

WebSendResultMessage::WebSendResultMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebSendResultMessage)
{
    ui->setupUi(this);

    ui->frDetail->setVisible(false);
    setStyleSheet(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet());
    adjustSize();
    setFixedSize(size());
}

WebSendResultMessage::~WebSendResultMessage()
{
    delete ui;
}

void WebSendResultMessage::addCommentString(const QString &string)
{
    ui->teDetail->append(string);
}

int WebSendResultMessage::exec()
{
    Q_ASSERT(false);
    return false;
}

int WebSendResultMessage::exec(const WebSendResultMessage::MessageType mt)
{
    ui->lblComment->setText(ResultComment.value(mt));
    ui->lblImageOK->setVisible(mt == WebSendResultMessage::ResultSucces);
    ui->lblImageError->setVisible(mt == WebSendResultMessage::ResultError);
    return QDialog::exec();
}

void WebSendResultMessage::on_showDetail()
{
    ui->frDetail->setVisible(!ui->frDetail->isVisible());
    adjustSize();
    if (!ui->frDetail->isVisible())
        setFixedSize(size());
    else
        setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}

void WebSendResultMessage::on_writeToFile()
{
    auto fn = QFileDialog::getSaveFileName(this,
                                           tr("Выберите файл для сохранения"),
                                           DataDefines::aanalyserDocumentsPath(),
                                           tr("Текстовые файлы (*.txt)"));
    if (fn != "")
    {
        QFile file(fn);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                 return;
        QTextStream out(&file);
        out << ui->teDetail->toPlainText();
        file.close();
    }
}
