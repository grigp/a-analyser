#ifndef SIGNALEXPORTDEFINES_H
#define SIGNALEXPORTDEFINES_H

namespace SignalExportDefines
{

/*!
 * \brief Режим экспортирования The Mode enum
 */
enum Mode
{
      mdUndefined = 0
    , mdOnce       ///< Одиночный
    , mdBatch      ///< Пакетный
};


/*!
 * \brief Режим записи подканалов в файлы The Files enum
 */
enum FilesMode
{
      fUndefined = 0
    , fSingle        ///< Все подканалы в один файл
    , fDifferent     ///< Подканалы в разные файлы
};

}

#endif // SIGNALEXPORTDEFINES_H
