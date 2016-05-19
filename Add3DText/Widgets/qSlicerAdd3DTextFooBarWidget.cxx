/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerAdd3DTextFooBarWidget.h"
#include "ui_qSlicerAdd3DTextFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_Add3DText
class qSlicerAdd3DTextFooBarWidgetPrivate
  : public Ui_qSlicerAdd3DTextFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerAdd3DTextFooBarWidget);
protected:
  qSlicerAdd3DTextFooBarWidget* const q_ptr;

public:
  qSlicerAdd3DTextFooBarWidgetPrivate(
    qSlicerAdd3DTextFooBarWidget& object);
  virtual void setupUi(qSlicerAdd3DTextFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerAdd3DTextFooBarWidgetPrivate
::qSlicerAdd3DTextFooBarWidgetPrivate(
  qSlicerAdd3DTextFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerAdd3DTextFooBarWidgetPrivate
::setupUi(qSlicerAdd3DTextFooBarWidget* widget)
{
  this->Ui_qSlicerAdd3DTextFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerAdd3DTextFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerAdd3DTextFooBarWidget
::qSlicerAdd3DTextFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerAdd3DTextFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerAdd3DTextFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerAdd3DTextFooBarWidget
::~qSlicerAdd3DTextFooBarWidget()
{
}
