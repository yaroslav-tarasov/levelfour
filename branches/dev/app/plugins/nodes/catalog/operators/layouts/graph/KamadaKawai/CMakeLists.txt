project(KamadaKawaiLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# KamadaKawaiLayouter
set( KamadaKawaiLayouter_h ${KamadaKawaiLayouter_h}
			KamadaKawaiLayouterNode.h
			KamadaKawaiLayouterNodePlugin.h
			)

set( KamadaKawaiLayouter_moc ${KamadaKawaiLayouter_moc}	
			KamadaKawaiLayouterNode.h
			KamadaKawaiLayouterNodePlugin.h
			)

set( KamadaKawaiLayouter_src ${KamadaKawaiLayouter_src}	
			KamadaKawaiLayouterNode.cpp
			KamadaKawaiLayouterNodePlugin.cpp
			)

set( KamadaKawaiLayouter_res ${KamadaKawaiLayouter_res}	
			KamadaKawaiLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(KamadaKawaiLayouter_cxx ${KamadaKawaiLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${KamadaKawaiLayouter_cxx})
source_group("Header Files" FILES ${KamadaKawaiLayouter_h})
source_group("Resources" FILES ${KamadaKawaiLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(KamadaKawaiLayouter_src ${KamadaKawaiLayouter_src} ${KamadaKawaiLayouter_h} ${KamadaKawaiLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(KamadaKawaiLayouter SHARED ${KamadaKawaiLayouter_src} ${KamadaKawaiLayouter_cxx})

# Add library dependencies
target_link_libraries(KamadaKawaiLayouter
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
install( FILES ${KamadaKawaiLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS KamadaKawaiLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
