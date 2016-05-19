cmake_minimum_required(VERSION 2.8.9)

project(Add3DText)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "http://slicer.org/slicerWiki/index.php/Documentation/Nightly/Extensions/Add3DText")
set(EXTENSION_CATEGORY "Mesh Generation")
set(EXTENSION_CONTRIBUTORS "Fenghe TANG, Dr. Xiaojun CHEN (SJTU)")
set(EXTENSION_DESCRIPTION "This is an extension module to add 3D text to a model.")
set(EXTENSION_ICONURL "http://wiki.slicer.org/slicerWiki/index.php/File:Add3DText.png")
set(EXTENSION_SCREENSHOTURLS "http://wiki.slicer.org/slicerWiki/index.php/File:Add3DText_ScreenShot.png")

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(Add3DText)
## NEXT_MODULE

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_CPACK})
