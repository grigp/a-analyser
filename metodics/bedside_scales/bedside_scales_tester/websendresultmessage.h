#ifndef WEBSENDRESULTMESSAGE_H
#define WEBSENDRESULTMESSAGE_H

#include <QDialog>

namespace Ui {
class WebSendResultMessage;
}

class WebSendResultMessage : public QDialog
{
    Q_OBJECT

public:
    explicit WebSendResultMessage(QWidget *parent = nullptr);
    ~WebSendResultMessage() override;

    /*!
     * \brief Результаты The MessageType enum
     */
    enum MessageType
    {
          ResultSucces = 0  ///< Успех передачи данных
        , ResultError       ///< Ошибка передачи данных
    };

    /*!
     * \brief Добавляет строку в комментарий
     */
    void addCommentString(const QString &string);

public slots:
    int exec() override;
    int exec(const MessageType mt);

private slots:
    void on_showDetail();
    void on_writeToFile();

private:
    Ui::WebSendResultMessage *ui;
};

#endif // WEBSENDRESULTMESSAGE_H
