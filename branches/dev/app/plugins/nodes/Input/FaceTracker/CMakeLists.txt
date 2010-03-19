project(facetracker)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/opencv/cv/include
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/opencv/cvaux/include
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/opencv/cxcore/include
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/opencv/otherlibs/highgui
			)

link_directories(
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/opencv/lib
			)

# facetracker
set( facetracker_h ${facetracker_h}
			Face.h
			FaceTrackerNode.h
			FaceTrackerNodePlugin.h
			Helper.h
			)

set( facetracker_moc ${facetracker_moc}	
			FaceTrackerNode.h
			FaceTrackerNodePlugin.h
			)

set( facetracker_src ${facetracker_src}
			Face.cpp
			FaceTrackerNode.cpp
			FaceTrackerNodePlugin.cpp
			)

set( facetracker_res ${facetracker_res}	
			facetracker.xml
			)

# Create moc files		   
qt4_wrap_cpp(facetracker_cxx ${facetracker_moc})

# Create source groups
source_group("Moc Files" FILES ${facetracker_cxx})
source_group("Header Files" FILES ${facetracker_h})
source_group("Resources" FILES ${facetracker_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(facetracker_src ${facetracker_src} ${facetracker_h} ${facetracker_res})
ENDIF(WIN32)

# Create static library
add_library(facetracker SHARED ${facetracker_src} ${facetracker_cxx})

# Add library dependencies
target_link_libraries(facetracker
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized cv debug cv
			optimized cxcore debug cxcore
			optimized highgui debug highgui
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${facetracker_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS facetracker RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
