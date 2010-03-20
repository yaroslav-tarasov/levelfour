project(testNode)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# test
set( test_h ${test_h}
			testNode.h
			testNodePlugin.h
			)

set( test_moc ${test_moc}	
			testNode.h
			testNodePlugin.h
			)

set( test_src ${test_src}	
			testNode.cpp
			testNodePlugin.cpp
			)

set( test_res ${test_res}	
			test.xml
			)

# Create moc files		   
qt4_wrap_cpp(test_cxx ${test_moc})

# Create source groups
source_group("Moc Files" FILES ${test_cxx})
source_group("Header Files" FILES ${test_h})
source_group("Resources" FILES ${test_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(test_src ${test_src} ${test_h} ${test_res})
ENDIF(WIN32)

# Create static library
add_library(test SHARED ${test_src} ${test_cxx})

# Add library dependencies
target_link_libraries(test
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${test_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS test RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
