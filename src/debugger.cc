/*
  Freebox QtCreator plugin for QML application development.

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not see
  http://www.gnu.org/licenses/lgpl-2.1.html.

  Copyright (c) 2014, Freebox SAS, See AUTHORS for details.
*/

#include "debugger.hh"
#include "project.hh"

#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/target.h>

#include <debugger/debuggerstartparameters.h>
#include <debugger/debuggerstartparameters.h>

namespace Freebox {

DebuggerRunControl::DebuggerRunControl(Debugger::DebuggerRunControl *runControl) :
    RunControl(runControl->runConfiguration(), runControl->runMode()),
    mRunControl(runControl)
{
    mDebug = true;

    connect(mRunControl, SIGNAL(finished()), SLOT(remoteStop()));
    connect(mRunControl, SIGNAL(started()), SLOT(remoteStart()));
    connect(this,
            SIGNAL(remoteStarted(quint16)),
            SLOT(remoteSetupDone(quint16)));
    connect(this, SIGNAL(appendMessage(ProjectExplorer::RunControl *,
                                       const QString &,
                                       Utils::OutputFormat)),
            SLOT(redirectMessage(ProjectExplorer::RunControl *,
                                 const QString &,
                                 Utils::OutputFormat)));
}

void DebuggerRunControl::redirectMessage(ProjectExplorer::RunControl *rc,
                                         const QString &msg,
                                         Utils::OutputFormat format)
{
    Q_UNUSED(rc);
    Q_UNUSED(format);
    mRunControl->showMessage(msg, Debugger::AppOutput);
}

void DebuggerRunControl::remoteStart()
{
    start();
}

void DebuggerRunControl::remoteStop()
{
    stop();
}

void DebuggerRunControl::remoteSetupDone(quint16 port)
{
    Debugger::RemoteSetupResult result;
    result.success = true;
    result.gdbServerPort = -1;
    result.qmlServerPort = port;
    mRunControl->notifyEngineRemoteSetupFinished(result);
}

} // namespace Freebox
