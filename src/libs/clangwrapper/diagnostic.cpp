/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "diagnostic.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

using namespace Clang;

Diagnostic::Diagnostic()
    : m_severity(Unknown)
    , m_length(0)
{}

Diagnostic::Diagnostic(Severity severity, const SourceLocation &location, unsigned length, const QString &spelling)
    : m_severity(severity)
    , m_loc(location)
    , m_length(length)
    , m_spelling(spelling)
{}

const QString Diagnostic::severityAsString() const
{
    if (m_severity == Unknown)
        return QString();

    static QStringList strs = QStringList()
            << QCoreApplication::translate("Diagnostic", "ignored")
            << QCoreApplication::translate("Diagnostic", "note")
            << QCoreApplication::translate("Diagnostic", "warning")
            << QCoreApplication::translate("Diagnostic", "error")
            << QCoreApplication::translate("Diagnostic", "fatal")
               ;

    return strs.at(m_severity);
}
