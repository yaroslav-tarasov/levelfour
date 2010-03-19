project(LevelFour)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include(${QT_USE_FILE})
include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			${CMAKE_CURRENT_BINARY_DIR}
			)

# frapperdemo
set( levelfour_h ${levelfour_h}
			Application.h
			#resource.h
		   )

set( levelfour_moc ${levelfour_moc}		
			Application.h
		   )

set( levelfour_ui ${levelfour_ui}

		   )

set( levelfour_src ${levelfour_src}
			Application.cpp
			main.cpp
		   )
		   
set ( levelfour_qrc ${levelfour_qrc}
			main.qrc
		   )		   
		   
# Create moc and ui files			   
qt4_wrap_cpp(levelfour_cxx ${levelfour_moc})
qt4_wrap_ui(levelfour_ui_h ${levelfour_ui})
qt4_add_resources(levelfour_rcc ${levelfour_qrc})

# Create source groups
source_group("Ui Files" FILES ${levelfour_ui})
source_group("Resource Files" FILES ${levelfour_qrc})
source_group("Generated Files" FILES ${levelfour_cxx} ${levelfour_ui_h} ${levelfour_rcc} ${levelfour_qrc})
source_group("Header Files" FILES ${levelfour_h})

# Add header files to sources to make headers visible in Visual Studio
set(levelfour_src ${levelfour_src} ${levelfour_h})

# Create static library
add_executable(levelfour WIN32 ${levelfour_src} ${levelfour_cxx} ${levelfour_ui_h} ${levelfour_rcc})

set_target_properties( levelfour
     PROPERTIES
     DEBUG_OUTPUT_NAME "levelfour_d"
     RELEASE_OUTPUT_NAME "levelfour"
)


IF (WIN32)
target_link_libraries(levelfour 
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
target_link_libraries(levelfour 
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
install( TARGETS levelfour RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
