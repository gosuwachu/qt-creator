/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
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

#include "cmakebuildconfiguration.h"

#include "cmakebuildinfo.h"
#include "cmakeopenprojectwizard.h"
#include "cmakeproject.h"
#include "cmakeprojectconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>

#include <utils/qtcassert.h>

#include <QInputDialog>

using namespace CMakeProjectManager;
using namespace Internal;

namespace {
const char USE_NINJA_KEY[] = "CMakeProjectManager.CMakeBuildConfiguration.UseNinja";
} // namespace

CMakeBuildConfiguration::CMakeBuildConfiguration(ProjectExplorer::Target *parent) :
    BuildConfiguration(parent, Core::Id(Constants::CMAKE_BC_ID)), m_useNinja(false)
{
    CMakeProject *project = static_cast<CMakeProject *>(parent->project());
    setBuildDirectory(Utils::FileName::fromString(project->shadowBuildDirectory(project->projectFilePath(),
                                                                                parent->kit(),
                                                                                displayName())));
}

CMakeBuildConfiguration::CMakeBuildConfiguration(ProjectExplorer::Target *parent,
                                                 CMakeBuildConfiguration *source) :
    BuildConfiguration(parent, source),
    m_msvcVersion(source->m_msvcVersion),
    m_useNinja(source->m_useNinja)
{
    Q_ASSERT(parent);
    cloneSteps(source);
}

QVariantMap CMakeBuildConfiguration::toMap() const
{
    QVariantMap map(ProjectExplorer::BuildConfiguration::toMap());
    map.insert(QLatin1String(USE_NINJA_KEY), m_useNinja);
    return map;
}

bool CMakeBuildConfiguration::fromMap(const QVariantMap &map)
{
    if (!BuildConfiguration::fromMap(map))
        return false;

    m_useNinja = map.value(QLatin1String(USE_NINJA_KEY), false).toBool();

    return true;
}

bool CMakeBuildConfiguration::useNinja() const
{
    return m_useNinja;
}

void CMakeBuildConfiguration::setUseNinja(bool useNninja)
{
    if (m_useNinja != useNninja) {
        m_useNinja = useNninja;
        emit useNinjaChanged(m_useNinja);
    }
}

CMakeBuildConfiguration::~CMakeBuildConfiguration()
{ }

ProjectExplorer::NamedWidget *CMakeBuildConfiguration::createConfigWidget()
{
    return new CMakeBuildSettingsWidget(this);
}

/*!
  \class CMakeBuildConfigurationFactory
*/

CMakeBuildConfigurationFactory::CMakeBuildConfigurationFactory(QObject *parent) :
    ProjectExplorer::IBuildConfigurationFactory(parent)
{
}

CMakeBuildConfigurationFactory::~CMakeBuildConfigurationFactory()
{
}

int CMakeBuildConfigurationFactory::priority(const ProjectExplorer::Target *parent) const
{
    return canHandle(parent) ? 0 : -1;
}

QList<ProjectExplorer::BuildInfo *> CMakeBuildConfigurationFactory::availableBuilds(const ProjectExplorer::Target *parent) const
{
    QList<ProjectExplorer::BuildInfo *> result;

    CMakeBuildInfo *info = createBuildInfo(parent->kit(),
                                           parent->project()->projectDirectory());
    result << info;
    return result;
}

int CMakeBuildConfigurationFactory::priority(const ProjectExplorer::Kit *k, const QString &projectPath) const
{
    return (k && Core::MimeDatabase::findByFile(QFileInfo(projectPath))
            .matchesType(QLatin1String(Constants::CMAKEMIMETYPE))) ? 0 : -1;
}

QList<ProjectExplorer::BuildInfo *> CMakeBuildConfigurationFactory::availableSetups(const ProjectExplorer::Kit *k,
                                                                                    const QString &projectPath) const
{
    QList<ProjectExplorer::BuildInfo *> result;
    CMakeBuildInfo *info = createBuildInfo(k, ProjectExplorer::Project::projectDirectory(projectPath));
    //: The name of the build configuration created by default for a cmake project.
    info->displayName = tr("Default");
    info->buildDirectory
            = Utils::FileName::fromString(CMakeProject::shadowBuildDirectory(projectPath, k,
                                                                             info->displayName));
    result << info;
    return result;
}

ProjectExplorer::BuildConfiguration *CMakeBuildConfigurationFactory::create(ProjectExplorer::Target *parent,
                                                                            const ProjectExplorer::BuildInfo *info) const
{
    QTC_ASSERT(info->factory() == this, return 0);
    QTC_ASSERT(info->kitId == parent->kit()->id(), return 0);
    QTC_ASSERT(!info->displayName.isEmpty(), return 0);

    CMakeBuildInfo copy(*static_cast<const CMakeBuildInfo *>(info));
    CMakeProject *project = static_cast<CMakeProject *>(parent->project());

    if (copy.buildDirectory.isEmpty())
        copy.buildDirectory
                = Utils::FileName::fromString(project->shadowBuildDirectory(project->projectFilePath(),
                                                                            parent->kit(),
                                                                            copy.displayName));

    CMakeOpenProjectWizard copw(project->projectManager(), CMakeOpenProjectWizard::ChangeDirectory, &copy);
    if (copw.exec() != QDialog::Accepted)
        return 0;

    CMakeBuildConfiguration *bc = new CMakeBuildConfiguration(parent);
    bc->setDisplayName(copy.displayName);
    bc->setDefaultDisplayName(copy.displayName);

    ProjectExplorer::BuildStepList *buildSteps = bc->stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    ProjectExplorer::BuildStepList *cleanSteps = bc->stepList(ProjectExplorer::Constants::BUILDSTEPS_CLEAN);

    MakeStep *makeStep = new MakeStep(buildSteps);
    buildSteps->insertStep(0, makeStep);

    MakeStep *cleanMakeStep = new MakeStep(cleanSteps);
    cleanSteps->insertStep(0, cleanMakeStep);
    cleanMakeStep->setAdditionalArguments(QLatin1String("clean"));
    cleanMakeStep->setClean(true);

    bc->setBuildDirectory(Utils::FileName::fromString(copw.buildDirectory()));
    bc->setUseNinja(copw.useNinja());

    // Default to all
    if (project->hasBuildTarget(QLatin1String("all")))
        makeStep->setBuildTarget(QLatin1String("all"), true);

    return bc;
}

bool CMakeBuildConfigurationFactory::canClone(const ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *source) const
{
    if (!canHandle(parent))
        return false;
    return source->id() == Constants::CMAKE_BC_ID;
}

CMakeBuildConfiguration *CMakeBuildConfigurationFactory::clone(ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *source)
{
    if (!canClone(parent, source))
        return 0;
    CMakeBuildConfiguration *old = static_cast<CMakeBuildConfiguration *>(source);
    return new CMakeBuildConfiguration(parent, old);
}

bool CMakeBuildConfigurationFactory::canRestore(const ProjectExplorer::Target *parent, const QVariantMap &map) const
{
    if (!canHandle(parent))
        return false;
    return ProjectExplorer::idFromMap(map) == Constants::CMAKE_BC_ID;
}

CMakeBuildConfiguration *CMakeBuildConfigurationFactory::restore(ProjectExplorer::Target *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    CMakeBuildConfiguration *bc = new CMakeBuildConfiguration(parent);
    if (bc->fromMap(map))
        return bc;
    delete bc;
    return 0;
}

bool CMakeBuildConfigurationFactory::canHandle(const ProjectExplorer::Target *t) const
{
    QTC_ASSERT(t, return false);
    if (!t->project()->supportsKit(t->kit()))
        return false;
    return qobject_cast<CMakeProject *>(t->project());
}

CMakeBuildInfo *CMakeBuildConfigurationFactory::createBuildInfo(const ProjectExplorer::Kit *k,
                                                                const QString &sourceDir) const
{
    CMakeBuildInfo *info = new CMakeBuildInfo(this);
    info->typeName = tr("Build");
    info->kitId = k->id();
    info->environment = Utils::Environment::systemEnvironment();
    k->addToEnvironment(info->environment);
    info->useNinja = false;
    info->sourceDirectory = sourceDir;
    info->supportsShadowBuild = true;

    return info;
}

ProjectExplorer::BuildConfiguration::BuildType CMakeBuildConfiguration::buildType() const
{
    QString cmakeBuildType;
    QFile cmakeCache(buildDirectory().toString() + QLatin1String("/CMakeCache.txt"));
    if (cmakeCache.open(QIODevice::ReadOnly)) {
        while (!cmakeCache.atEnd()) {
            QByteArray line = cmakeCache.readLine();
            if (line.startsWith("CMAKE_BUILD_TYPE")) {
                if (int pos = line.indexOf('='))
                    cmakeBuildType = QString::fromLocal8Bit(line.mid(pos + 1).trimmed());
                break;
            }
        }
        cmakeCache.close();
    }

    // Cover all common CMake build types
    if (cmakeBuildType.compare(QLatin1String("Release"), Qt::CaseInsensitive) == 0
        || cmakeBuildType.compare(QLatin1String("MinSizeRel"), Qt::CaseInsensitive) == 0)
    {
        return Release;
    } else if (cmakeBuildType.compare(QLatin1String("Debug"), Qt::CaseInsensitive) == 0
               || cmakeBuildType.compare(QLatin1String("DebugFull"), Qt::CaseInsensitive) == 0
               || cmakeBuildType.compare(QLatin1String("RelWithDebInfo"), Qt::CaseInsensitive) == 0)
    {
        return Debug;
    }

    return Unknown;
}

