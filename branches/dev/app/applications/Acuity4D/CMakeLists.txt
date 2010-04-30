project(ACUITY4D)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include(${QT_USE_FILE})
include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			${CMAKE_CURRENT_BINARY_DIR}
			)

# frapperdemo
set( ACUITY4D_h ${ACUITY4D_h}
			Application.h
			#resource.h
		   )

set( ACUITY4D_moc ${ACUITY4D_moc}		
			Application.h
		   )

set( ACUITY4D_ui ${ACUITY4D_ui}

		   )

set( ACUITY4D_src ${ACUITY4D_src}
			Application.cpp
			main.cpp
		   )
		   
set ( ACUITY4D_qrc ${ACUITY4D_qrc}
			main.qrc
		   )		   
		   
# Create moc and ui files			   
qt4_wrap_cpp(ACUITY4D_cxx ${ACUITY4D_moc})
qt4_wrap_ui(ACUITY4D_ui_h ${ACUITY4D_ui})
qt4_add_resources(ACUITY4D_rcc ${ACUITY4D_qrc})

# Create source groups
source_group("Ui Files" FILES ${ACUITY4D_ui})
source_group("Resource Files" FILES ${ACUITY4D_qrc})
source_group("Generated Files" FILES ${ACUITY4D_cxx} ${ACUITY4D_ui_h} ${ACUITY4D_rcc} ${ACUITY4D_qrc})
source_group("Header Files" FILES ${ACUITY4D_h})

# Add header files to sources to make headers visible in Visual Studio
set(ACUITY4D_src ${ACUITY4D_src} ${ACUITY4D_h})

# Create static library
add_executable(ACUITY4D WIN32 ${ACUITY4D_src} ${ACUITY4D_cxx} ${ACUITY4D_ui_h} ${ACUITY4D_rcc})

set_target_properties( ACUITY4D
     PROPERTIES
     DEBUG_OUTPUT_NAME "ACUITY4D_d"
     RELEASE_OUTPUT_NAME "ACUITY4D"
)


IF (WIN32)
target_link_libraries(ACUITY4D 
			#${QT_LIBRARIES}
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized libcollada15dom21 debug libcollada15dom21-d
			optimized OgreMain debug OgreMain_d
			optimized qtmain.lib debug qtmaind.lib
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)
ENDIF (WIN32)
IF (UNIX)
target_link_libraries(ACUITY4D 
			#${QT_LIBRARIES}
			optimized QtCore debug QtCored
			optimized QtGui debug QtGuid
			optimized QtXml debug QtXmld
			optimized minizip debug minizip
			optimized tools debug tools_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			optimized boost_filesystem debug boost_filesystem
			optimized collada14dom debug collada14dom
			optimized OgreMain debug OgreMain_d
			)
ENDIF (UNIX)

# Install files
install( TARGETS ACUITY4D RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
