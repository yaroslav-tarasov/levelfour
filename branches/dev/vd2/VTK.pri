######################################################################
# VTK Project Include
######################################################################

win32{
VTK5_INCLUDE_DIR  = $${PROJECT_BASE}/VTK5/include/vtk-5.5
} else {
VTK5_INCLUDE_DIR  = $${PROJECT_BASE}/VTK5/include/
}
win32 {
VTK5_LIB_DIR      = $${PROJECT_BASE}/VTK5/lib/vtk-5.5
} else {
VTK5_LIB_DIR      = $${PROJECT_BASE}/VTK5/bin/
}

INCLUDEPATH       += $$VTK5_INCLUDE_DIR $${GCF_BASE}/Components 
unix:LIBS         += -L$$VTK5_LIB_DIR \
                    -lQVTK \
                    -lvtkalglib \
                    -lvtkCommon \
                    -lvtkDICOMParser \
                    -lvtkexoIIc \
                    -lvtkexpat \
                    -lvtkFiltering \
                    -lvtkfreetype \
                    -lvtkftgl \
                    -lvtkGenericFiltering \
                    -lvtkGeovis \
                    -lvtkGraphics \
                    -lvtkHybrid \
                    -lvtkImaging \
                    -lvtkInfovis \
                    -lvtkIO \
                    -lvtkjpeg \
                    -lvtklibxml2 \
                    -lvtkmetaio \
                    -lvtkNetCDF \
                    -lvtkpng \
                    -lvtkproj4 \
                    -lvtkQtChart \
                    -lvtkRendering \
                    -lvtksqlite \
                    -lvtksys \
                    -lvtktiff \
                    -lvtkverdict \
                    -lvtkViews \
                    -lvtkVolumeRendering \
                    -lvtkWidgets \
                    -lvtkzlib \
                    -lvtkInfovis

win32:LIBS +=   $$VTK5_LIB_DIR/vtkCommon.lib \ 
                $$VTK5_LIB_DIR/vtkGraphics.lib \ 
                $$VTK5_LIB_DIR/vtkFiltering.lib \ 
                $$VTK5_LIB_DIR/vtkGenericFiltering.lib \ 
                $$VTK5_LIB_DIR/vtkRendering.lib \ 
                $$VTK5_LIB_DIR/vtkImaging.lib \ 
                $$VTK5_LIB_DIR/vtkIO.lib \ 
                $$VTK5_LIB_DIR/QVTK.lib \ 
                $$VTK5_LIB_DIR/vtkWidgets.lib \ 
                $$VTK5_LIB_DIR/vtkHybrid.lib \ 
                $$VTK5_LIB_DIR/vtkzlib.lib \ 
                $$VTK5_LIB_DIR/vtkfreetype.lib \ 
                $$VTK5_LIB_DIR/vtkexpat.lib \ 
                $$VTK5_LIB_DIR/vtkexoIIc.lib \ 
                $$VTK5_LIB_DIR/vtkDICOMParser.lib \ 
                $$VTK5_LIB_DIR/vtksys.lib \ 
                $$VTK5_LIB_DIR/QVTK.lib \
                $$VTK5_LIB_DIR/vtkInfovis.lib

QT                += opengl


