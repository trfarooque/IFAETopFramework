atlas_subdir ( IFAETopFramework )


atlas_depends_on_subdirs( PUBLIC )

find_package( ROOT REQUIRED COMPONENTS Core Tree RIO Physics Hist )

atlas_add_root_dictionary( IFAETopFramework _cintDictSource
   ROOT_HEADERS IFAETopFramework/*.h Root/LinkDef.h
   EXTERNAL_PACKAGES ROOT )

atlas_add_library( IFAETopFramework Root/*.cxx Root/*.h 
		   IFAETopFramework/*h ${_cintDictSource}
		   PUBLIC_HEADERS IFAETopFramework
		   LINK_LIBRARIES ${ROOT_LIBRARIES}
		   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} )

atlas_install_data( data/* )
atlas_install_scripts( scripts/*py )
# Install the Python modules of the package:
atlas_install_python_modules( python/*.py )
