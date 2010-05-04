project(ParentCenteredLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ParentCenteredLayouter
set( ParentCenteredLayouter_h ${ParentCenteredLayouter_h}
			ParentCenteredLayouterNode.h
			ParentCenteredLayouterNodePlugin.h
			)

set( ParentCenteredLayouter_moc ${ParentCenteredLayouter_moc}	
			ParentCenteredLayouterNode.h
			ParentCenteredLayouterNodePlugin.h
			)

set( ParentCenteredLayouter_src ${ParentCenteredLayouter_src}	
			ParentCenteredLayouterNode.cpp
			ParentCenteredLayouterNodePlugin.cpp
			)

set( ParentCenteredLayouter_res ${ParentCenteredLayouter_res}	
			ParentCenteredLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(ParentCenteredLayouter_cxx ${ParentCenteredLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${ParentCenteredLayouter_cxx})
source_group("Header Files" FILES ${ParentCenteredLayouter_h})
source_group("Resources" FILES ${ParentCenteredLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ParentCenteredLayouter_src ${ParentCenteredLayouter_src} ${ParentCenteredLayouter_h} ${ParentCenteredLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(ParentCenteredLayouter SHARED ${ParentCenteredLayouter_src} ${ParentCenteredLayouter_cxx})

# Add library dependencies
target_link_libraries(ParentCenteredLayouter
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
install( FILES ${ParentCenteredLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ParentCenteredLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
