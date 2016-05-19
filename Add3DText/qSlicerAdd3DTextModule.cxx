/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QtPlugin>

// Add3DText Logic includes
#include <vtkSlicerAdd3DTextLogic.h>

// Add3DText includes
#include "qSlicerAdd3DTextModule.h"
#include "qSlicerAdd3DTextModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerAdd3DTextModule, qSlicerAdd3DTextModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerAdd3DTextModulePrivate
{
public:
  qSlicerAdd3DTextModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerAdd3DTextModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerAdd3DTextModulePrivate::qSlicerAdd3DTextModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerAdd3DTextModule methods

//-----------------------------------------------------------------------------
qSlicerAdd3DTextModule::qSlicerAdd3DTextModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerAdd3DTextModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerAdd3DTextModule::~qSlicerAdd3DTextModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerAdd3DTextModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerAdd3DTextModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerAdd3DTextModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerAdd3DTextModule::icon() const
{
  return QIcon(":/Icons/Add3DText.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerAdd3DTextModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerAdd3DTextModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerAdd3DTextModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerAdd3DTextModule
::createWidgetRepresentation()
{
  return new qSlicerAdd3DTextModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerAdd3DTextModule::createLogic()
{
  return vtkSlicerAdd3DTextLogic::New();
}
