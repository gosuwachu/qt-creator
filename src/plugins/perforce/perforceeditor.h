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

#ifndef PERFORCEEDITOR_H
#define PERFORCEEDITOR_H

#include <vcsbase/vcsbaseeditor.h>

#include <QtCore/QRegExp>

namespace Perforce {
namespace Internal {

class PerforcePlugin;

class PerforceEditor : public VcsBase::VcsBaseEditorWidget
{
    Q_OBJECT

public:
    explicit PerforceEditor(const VcsBase::VcsBaseEditorParameters *type,
                            QWidget *parent);

private:
    virtual QSet<QString> annotationChanges() const;
    virtual QString changeUnderCursor(const QTextCursor &) const;
    virtual VcsBase::DiffHighlighter *createDiffHighlighter() const;
    virtual VcsBase::BaseAnnotationHighlighter *createAnnotationHighlighter(const QSet<QString> &changes) const;
    virtual QString fileNameFromDiffSpecification(const QTextBlock &diffFileName) const;
    virtual QStringList annotationPreviousVersions(const QString &v) const;

    const QRegExp m_changeNumberPattern;
    PerforcePlugin *m_plugin;
};

} // namespace Perforce
} // namespace Internal

#endif // PERFORCEEDITOR_H
