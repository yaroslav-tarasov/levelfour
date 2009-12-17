######################################################################
# Generic Component Framework Library from VCreate Logic
# 
# Common configuration settings for most components
######################################################################

include(../GCF/config.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . source $${PROJECT_BASE}/Interfaces $${PROJECT_BASE}/Common
INCLUDEPATH    += . source $${PROJECT_BASE}/Interfaces $${PROJECT_BASE}/Common

# Interfaces to other standard components
INCLUDEPATH += $${PROJECT_BASE}/Components/PropertyEditor/interfaces
INCLUDEPATH += $${PROJECT_BASE}/Components/Utils/interfaces
INCLUDEPATH += $${PROJECT_BASE}/Components/ScriptEditor/interfaces
INCLUDEPATH += $${PROJECT_BASE}/Components/ProjectManager/interfaces
INCLUDEPATH += $${PROJECT_BASE}/Components/MainWindow/source

