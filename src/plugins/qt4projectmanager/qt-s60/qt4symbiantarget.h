/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QT4SYMBIANTARGET_H
#define QT4SYMBIANTARGET_H

#include "qt4target.h"

#include <QPixmap>

namespace Qt4ProjectManager {
class Qt4Project;
class Qt4BuildConfigurationFactory;
namespace Internal {


class Qt4SymbianTarget : public Qt4BaseTarget
{
    friend class Qt4SymbianTargetFactory; // for from Map
    Q_OBJECT
public:
    Qt4SymbianTarget(Qt4Project *parent, const Core::Id id);
    ~Qt4SymbianTarget();

    ProjectExplorer::IBuildConfigurationFactory *buildConfigurationFactory() const;

    void createApplicationProFiles(bool reparse);
    virtual QList<ProjectExplorer::RunConfiguration *> runConfigurationsForNode(ProjectExplorer::Node *n);

    static QString defaultDisplayName(const Core::Id id);
    static QIcon iconForId(Core::Id id);

protected:
    ProjectExplorer::IDevice::ConstPtr currentDevice() const;

private:
    Qt4BuildConfigurationFactory *m_buildConfigurationFactory;
};
} // namespace Internal
} // namespace Qt4ProjectManager
#endif // QT4SYMBIANTARGET_H
