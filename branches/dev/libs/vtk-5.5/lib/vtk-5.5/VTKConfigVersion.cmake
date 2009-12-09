# The full VTK version number.
SET(PACKAGE_VERSION "5.5.20091209")

# This version is compatible only with matching major.minor versions.
IF("5.5" VERSION_EQUAL "${PACKAGE_FIND_VERSION_MAJOR}.${PACKAGE_FIND_VERSION_MINOR}")
  # This version is compatible with equal or lesser patch versions.
  IF(NOT "20091209" VERSION_LESS "${PACKAGE_FIND_VERSION_PATCH}")
    SET(PACKAGE_VERSION_COMPATIBLE 1)
    IF("20091209" VERSION_EQUAL "${PACKAGE_FIND_VERSION_PATCH}")
      SET(PACKAGE_VERSION_EXACT 1)
    ENDIF()
  ENDIF()
ENDIF()
