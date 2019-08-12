#ifndef EXITCODES_H
#define EXITCODES_H

#include <QMap>

enum eExitCodes: int
{
    EC_OK = 0,
    EC_ERROR = -1,
    EC_START_ERROR = -2,
    EC_START_UNKNOWN_ERROR = -3,
    EC_MAINLOOP_ERROR = -4,
    EC_MAINLOOP_UNKNOWN_ERROR = -5
};

static QMap<eExitCodes, QString> EXIT_COMMENTS = {
      {EC_OK, "Программа корректно завершила свою работу."}
    , {EC_START_ERROR, "Ошибка при запуске программы: %1. Программа будет завершена."}
    , {EC_START_UNKNOWN_ERROR, "Неизвестная ошибка при запуске программы. Программа будет завершена."}
    , {EC_MAINLOOP_ERROR, "Программа аварийно завершила свою работу с сообщением: %1. Источник %2"}
    , {EC_MAINLOOP_UNKNOWN_ERROR, "Программа аварийно завершила свою работу по неизвестной причине. Источник %1"}
};


#endif // EXITCODES_H
