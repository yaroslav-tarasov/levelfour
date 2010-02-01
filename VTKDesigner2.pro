######################################################################
# VTK Designer 2 Project from VCreate Logic (P) Ltd Bangalore
######################################################################

# Check if 
isEmpty(PROJECT_BASE) {

    win32 {
        error("PROJECT_BASE variable not specified. Use qmake -tp vc -recursive \"PROJECT_BASE=/path/to/vtkdesigner2\"");
    }
    
    unix {
        error("PROJECT_BASE variable not specified. Use qmake -recursive \"PROJECT_BASE=/path/to/vtkdesigner2\"");
    }

}

exists( $$PROJECT_BASE/VTKDesigner2.pro ) {

    TEMPLATE       = subdirs
    CONFIG         += ordered
    TARGET          = VTKDesigner2
    DEFINES        += GCF_APP_VERSION="VTK Designer 2.5"
    SUBDIRS         = GCF \
			    Components

    !isEmpty(COMMERCIAL_BUILD) {

        !exists( $${PROJECT_BASE}/CommercialComponents/CommercialComponents.pro ) {
            error( "Cannot find " $${PROJECT_BASE}"/CommercialComponents/CommercialComponents.pro");
        }
        
        SUBDIRS += CommercialComponents
        message("Building GNU/GPL and Commercial components")
        
    }

} else {

    error("Cannot find " $$GCF_BASE "/VTKDesigner2.pro")
    
}
