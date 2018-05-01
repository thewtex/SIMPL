/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice, this
* list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "FilterGroup10.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/SIMPLibVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterGroup10::FilterGroup10()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterGroup10::~FilterGroup10() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterGroup10::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterGroup10::setupFilterParameters()
{
  FilterParameterVector parameters;

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterGroup10::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterGroup10::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck();                       // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted();          // We are done preflighting this filter
  setInPreflight(false);             // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterGroup10::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  if(getCancel())
  {
    return;
  }

  if(getErrorCondition() < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer FilterGroup10::newFilterInstance(bool copyFilterParameters) const
{
  FilterGroup10::Pointer filter = FilterGroup10::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FilterGroup10::getCompiledLibraryName() const
{
  return Core::CoreBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FilterGroup10::getBrandingString() const
{
  return "SIMPLib Core Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FilterGroup10::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << SIMPLib::Version::Major() << "." << SIMPLib::Version::Minor() << "." << SIMPLib::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FilterGroup10::getGroupName() const
{
  return SIMPL::FilterGroups::Utilities;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid FilterGroup10::getUuid()
{
  return QUuid("{f259fade-f69b-5da7-916e-3826fcef15a6}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FilterGroup10::getSubGroupName() const
{
  return "Test";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FilterGroup10::getHumanLabel() const
{
  return "Utilities Test Filter";
}
