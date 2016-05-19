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

#ifndef __qSlicerAdd3DTextModuleWidget_h
#define __qSlicerAdd3DTextModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"
#include <qSlicerApplication.h>
#include "qSlicerAdd3DTextModuleExport.h"

//	Qt includes
#include <QMessageBox>

//	vtk includes
#include<vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkSphereSource.h>
#include <vtkVectorText.h>
#include <vtkProperty.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkReverseSense.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkWorldPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkTextActor.h>
#include <vtkTextRepresentation.h>
#include <vtkBorderRepresentation.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>
#include <vtkTextWidget.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkThreshold.h>
#include <vtkDistancePolyDataFilter.h>
#include <vtkIntersectionPolyDataFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>

class qSlicerAdd3DTextModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_ADD3DTEXT_EXPORT qSlicerAdd3DTextModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerAdd3DTextModuleWidget(QWidget *parent=0);
  virtual ~qSlicerAdd3DTextModuleWidget();

public slots:


protected:
  QScopedPointer<qSlicerAdd3DTextModuleWidgetPrivate> d_ptr;

  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerAdd3DTextModuleWidget);
  Q_DISABLE_COPY(qSlicerAdd3DTextModuleWidget);

//Below is extension code
protected:
	vtkRenderWindowInteractor* iren;
	vtkRenderWindow* renderWindow;
	vtkRenderer* renderer;
	qSlicerApplication *app;

private slots:
	void buttonSetTextClicked();
	void buttonLocateText();
	void pointPickEvent();
	void pointPickCancel();
	void buttonTextOKClicked();
	void buttonTextCancelClicked();
	void sliderTextSizeChanged();

private:
	void CameraFaceToPoint(double posX, double posY, double posZ, double normX, double normY, double normZ);
	void TextWidToPoint(double posX, double posY, double posZ);
	double* ComputeCrossMultiply(double Vector1[3], double Vector2[3]);
	vtkSmartPointer<vtkTransformPolyDataFilter>TransformText(double Vector1[3], double Vector2[3], double Coord[3], double TextScale);
	vtkSmartPointer<vtkPolyData>ProjectText(vtkPolyData* PolyText);
	vtkSmartPointer<vtkPolyData>CombinePolyData(vtkPolyData* input1, vtkPolyData* input2);

private:
	vtkSmartPointer<vtkPolyData>polydataMain;
	vtkSmartPointer<vtkPolyData>polydataResult;
	vtkSmartPointer<vtkPolyData>polydataText;
	vtkSmartPointer<vtkEventQtSlotConnect>m_vtkConnection;
	vtkSmartPointer<vtkVectorText>textSource;
	vtkSmartPointer<vtkActor>mActor;
	vtkSmartPointer<vtkActor>textActor;
	vtkSmartPointer<vtkTransformPolyDataFilter> transformText;
	vtkSmartPointer<vtkCellPicker>CellPicker;

	vtkSmartPointer<vtkTextWidget>textWidget;

private:
	bool isTextSet;
	double coordWorld1[4];
	double coordWorld2[4];
	double coordWorldC[4];
	double ViewPlaneNormal[3];
	double targetVector[3];


};

#endif
