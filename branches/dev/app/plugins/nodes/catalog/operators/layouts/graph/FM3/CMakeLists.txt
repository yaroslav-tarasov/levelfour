project(Fm3Layouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Fm3Layouter
set( Fm3Layouter_h ${Fm3Layouter_h}
			Fm3LayouterNode.h
			Fm3LayouterNodePlugin.h
			)

set( Fm3Layouter_moc ${Fm3Layouter_moc}	
			Fm3LayouterNode.h
			Fm3LayouterNodePlugin.h
			)

set( Fm3Layouter_src ${Fm3Layouter_src}	
			Fm3LayouterNode.cpp
			Fm3LayouterNodePlugin.cpp
			)

set( Fm3Layouter_res ${Fm3Layouter_res}	
			Fm3Layouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(Fm3Layouter_cxx ${Fm3Layouter_moc})

# Create source groups
source_group("Moc Files" FILES ${Fm3Layouter_cxx})
source_group("Header Files" FILES ${Fm3Layouter_h})
source_group("Resources" FILES ${Fm3Layouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Fm3Layouter_src ${Fm3Layouter_src} ${Fm3Layouter_h} ${Fm3Layouter_res})
ENDIF(WIN32)

# Create static library
add_library(Fm3Layouter SHARED ${Fm3Layouter_src} ${Fm3Layouter_cxx})

# Add library dependencies
target_link_libraries(Fm3Layouter
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			optimized vtkCommon debug vtkCommon
			)

# Install files
install( FILES ${Fm3Layouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Fm3Layouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
