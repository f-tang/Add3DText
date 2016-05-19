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

#ifndef __qSlicerAdd3DTextFooBarWidget_h
#define __qSlicerAdd3DTextFooBarWidget_h

// Qt includes
#include <QWidget>

// FooBar Widgets includes
#include "qSlicerAdd3DTextModuleWidgetsExport.h"

class qSlicerAdd3DTextFooBarWidgetPrivate;

/// \ingroup Slicer_QtModules_Add3DText
class Q_SLICER_MODULE_ADD3DTEXT_WIDGETS_EXPORT qSlicerAdd3DTextFooBarWidget
  : public QWidget
{
  Q_OBJECT
public:
  typedef QWidget Superclass;
  qSlicerAdd3DTextFooBarWidget(QWidget *parent=0);
  virtual ~qSlicerAdd3DTextFooBarWidget();

protected slots:

protected:
  QScopedPointer<qSlicerAdd3DTextFooBarWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerAdd3DTextFooBarWidget);
  Q_DISABLE_COPY(qSlicerAdd3DTextFooBarWidget);
};

#endif
