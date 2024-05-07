#ifndef REHABREPORTDEFINES_H
#define REHABREPORTDEFINES_H

#include <QString>

namespace RehabReportDefines
{

static const QString TrenEasyTestKindUid = "{848935A7-394E-42F9-9320-41ABD2ED5E5B}";
static const QString TrenHardTestKindUid = "{E7829C54-4F99-4870-A91C-404238B43E50}";


static const QString RombergTestUid = "{02D4EC93-C0C2-42BD-979C-D9F472A3CCEE}";
static const QString CrossTestUid = "{60164BDD-78E1-4A0D-8B9E-F1485D268227}";

/*!
 * \brief Роли в таблицах показателей The TableFactorsRoles enum
 */
enum TableFactorsRoles
{
      FactorUidRole = Qt::UserRole + 1   ///< uid показателя. QString. В столбцах с показателями, начиная с первого
    , FactorValueRole                    ///< Значение показателя. double. В столбцах с показателями, начиная с первого
};

}

#endif // REHABREPORTDEFINES_H
