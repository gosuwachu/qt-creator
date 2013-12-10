/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef CPPFOLLOWVIRTUALSYMBOLS_H
#define CPPFOLLOWVIRTUALSYMBOLS_H

#include <texteditor/codeassist/iassistprovider.h>

#include <cplusplus/CppDocument.h>
#include <cplusplus/Symbols.h>
#include <cplusplus/TypeOfExpression.h>

#include <QSharedPointer>
#include <QTextCursor>

namespace CppEditor {
namespace Internal {

class VirtualFunctionAssistProvider : public TextEditor::IAssistProvider
{
public:
    VirtualFunctionAssistProvider();

    struct Parameters {
        Parameters() : function(0), staticClass(0), cursorPosition(-1), openInNextSplit(false) {}

        CPlusPlus::Function *function;
        CPlusPlus::Class *staticClass;
        QSharedPointer<CPlusPlus::TypeOfExpression> typeOfExpression; // Keeps instantiated symbols.
        CPlusPlus::Snapshot snapshot;
        int cursorPosition;
        bool openInNextSplit;
    };

    virtual bool configure(const Parameters &parameters);
    Parameters params() const { return m_params; }
    void clearParams() { m_params = Parameters(); }

    bool isAsynchronous() const;
    bool supportsEditor(const Core::Id &editorId) const;
    TextEditor::IAssistProcessor *createProcessor() const;

private:
    Parameters m_params;
};

class FunctionHelper
{
public:
    static bool isVirtualFunction(const CPlusPlus::Function *function,
                                  const CPlusPlus::LookupContext &context,
                                  const CPlusPlus::Function **firstVirtual = 0);

    static bool isPureVirtualFunction(const CPlusPlus::Function *function,
                                      const CPlusPlus::LookupContext &context,
                                      const CPlusPlus::Function **firstVirtual = 0);

    static QList<CPlusPlus::Symbol *> overrides(CPlusPlus::Function *function,
                                                CPlusPlus::Class *functionsClass,
                                                CPlusPlus::Class *staticClass,
                                                const CPlusPlus::Snapshot &snapshot);
};

} // namespace Internal
} // namespace CppEditor

#endif // CPPFOLLOWVIRTUALSYMBOLS_H
