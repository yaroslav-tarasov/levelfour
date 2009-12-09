# Directory containing class headers.
SET(VTK_CHARTS_HEADER_DIR "${VTK_INSTALL_PREFIX}/include/vtk-5.5")

# Classes in vtkCharts.
SET(VTK_CHARTS_CLASSES
  "vtkAxis"
  "vtkBlockItem"
  "vtkBrush"
  "vtkChart"
  "vtkChartXY"
  "vtkContext2D"
  "vtkContextActor"
  "vtkContextDevice2D"
  "vtkContextItem"
  "vtkContextMapper2D"
  "vtkContextScene"
  "vtkContextView"
  "vtkImageItem"
  "vtkOpenGLContextDevice2D"
  "vtkPen"
  "vtkPlot"
  "vtkPlotGrid"
  "vtkPlotLine"
  "vtkPlotPoints")

# Abstract classes in vtkCharts.
SET(VTK_CHARTS_CLASSES_ABSTRACT
  "vtkChart"
  "vtkContextDevice2D"
  "vtkContextItem"
  "vtkContextMapper2D"
  "vtkPlot")

# Wrap-exclude classes in vtkCharts.
SET(VTK_CHARTS_CLASSES_WRAP_EXCLUDE)

# Set convenient variables to test each class.
FOREACH(class ${VTK_CHARTS_CLASSES})
  SET(VTK_CLASS_EXISTS_${class} 1)
ENDFOREACH(class)
FOREACH(class ${VTK_CHARTS_CLASSES_ABSTRACT})
  SET(VTK_CLASS_ABSTRACT_${class} 1)
ENDFOREACH(class)
FOREACH(class ${VTK_CHARTS_CLASSES_WRAP_EXCLUDE})
  SET(VTK_CLASS_WRAP_EXCLUDE_${class} 1)
ENDFOREACH(class)
