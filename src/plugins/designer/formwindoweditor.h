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

#ifndef FORMWINDOWEDITOR_H
#define FORMWINDOWEDITOR_H

#include "designer_export.h"
#include <coreplugin/editormanager/ieditor.h>

QT_BEGIN_NAMESPACE
class QDesignerFormWindowInterface;
QT_END_NAMESPACE

namespace TextEditor {
    class BaseTextDocument;
    class PlainTextEditor;
}

namespace Designer {

namespace Internal {
    class DesignerXmlEditor;
}
struct FormWindowEditorPrivate;

// The actual Core::IEditor belonging to Qt Designer. Uses FormWindowFile
// as the Core::IDocument to do the isModified() handling,
// which needs to be done by Qt Designer.
// However, to make the read-only XML text editor work,
// a TextEditor::PlainTextEditorEditable (IEditor) is also required.
// It is aggregated and some functions are delegated to it.

class DESIGNER_EXPORT FormWindowEditor : public Core::IEditor
{
    Q_PROPERTY(QString contents READ contents)
    Q_OBJECT
public:
    explicit FormWindowEditor(Internal::DesignerXmlEditor *editor,
                              QDesignerFormWindowInterface *form,
                              QObject *parent = 0);
    virtual ~FormWindowEditor();

    // IEditor
    virtual bool createNew(const QString &contents = QString());
    virtual bool open(QString *errorString, const QString &fileName, const QString &realFileName);
    virtual Core::IDocument *document();
    virtual Core::Id id() const;
    virtual QString displayName() const;
    virtual void setDisplayName(const QString &title);

    virtual bool duplicateSupported() const;
    virtual IEditor *duplicate(QWidget *parent);

    virtual QByteArray saveState() const;
    virtual bool restoreState(const QByteArray &state);

    virtual bool isTemporary() const;

    virtual QWidget *toolBar();

    virtual Core::Id preferredModeType() const;

    // For uic code model support
    QString contents() const;

    TextEditor::BaseTextDocument *textDocument();
    TextEditor::PlainTextEditor *textEditor();

public slots:
    void syncXmlEditor();

private slots:
    void slotOpen(QString *errorString, const QString &fileName);

private:
    void syncXmlEditor(const QString &contents);

    FormWindowEditorPrivate *d;
};

} // namespace Designer

#endif // FORMWINDOWEDITOR_H
