#Copyright (c) 2019 Ultimaker B.V.
#CuraEngine is released under the terms of the AGPLv3 or higher.

project(CuraEngine)

cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CuraEngine/cmake)

if (MSVC)
    option(MSVC_STATIC_RUNTIME "Link the MSVC runtime statically" OFF)
endif()

#For reading image files.
find_package(Stb REQUIRED)
include_directories(${Stb_INCLUDE_DIRS})

# convert build type to upper case letters
if(CMAKE_BUILD_TYPE)
    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
endif()

if(CMAKE_BUILD_TYPE_UPPER MATCHES "DEBUG")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG_INIT})
else()
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE_INIT})
endif()

if(NOT ${CMAKE_VERSION} VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
elseif(not MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

if(NOT DEFINED LIB_SUFFIX)
    set(LIB_SUFFIX "")
endif()
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}")

set(CURA_ENGINE_VERSION "master" CACHE STRING "Version name of Cura")

# Add a compiler flag to check the output for insane values if we are in debug mode.
if(CMAKE_BUILD_TYPE_UPPER MATCHES "DEBUG" OR CMAKE_BUILD_TYPE_UPPER MATCHES "RELWITHDEBINFO")
    message(STATUS "Building debug release of CuraEngine.")
    if (NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -O0 -g -fno-omit-frame-pointer")
    endif()
    add_definitions(-DASSERT_INSANE_OUTPUT)
    add_definitions(-DUSE_CPU_TIME)
    add_definitions(-DDEBUG)
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /Zc:twoPhase- /EHsc /W3")
    if (MSVC_STATIC_RUNTIME)
        foreach(flag_var
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            )
            if(${flag_var} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            endif()
        endforeach()
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # Add warnings
endif()

option (ENABLE_MORE_COMPILER_OPTIMIZATION_FLAGS
    "Enable more optimization flags" ON)
if (ENABLE_MORE_COMPILER_OPTIMIZATION_FLAGS)
    message (STATUS "Compile with more optimization flags")
    if (MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /fp:fast")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Ofast -funroll-loops")
    endif()
endif ()

if(NOT APPLE AND NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
endif()

if (WIN32)
    add_definitions(-DNOMINMAX)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR} CuraEngine/libs CuraEngine/libs/clipper)
add_library(clipper STATIC CuraEngine/libs/clipper/clipper.cpp)

set(engine_SRCS # Except main.cpp.
    CuraEngine/src/Application.cpp
    CuraEngine/src/bridge.cpp
    CuraEngine/src/ConicalOverhang.cpp
    CuraEngine/src/ExtruderTrain.cpp
    CuraEngine/src/FffGcodeWriter.cpp
    CuraEngine/src/FffPolygonGenerator.cpp
    CuraEngine/src/FffProcessor.cpp
    CuraEngine/src/gcodeExport.cpp
    CuraEngine/src/GCodePathConfig.cpp
    CuraEngine/src/infill.cpp
    CuraEngine/src/InsetOrderOptimizer.cpp
    CuraEngine/src/layerPart.cpp
    CuraEngine/src/LayerPlan.cpp
    CuraEngine/src/LayerPlanBuffer.cpp
    CuraEngine/src/MergeInfillLines.cpp
    CuraEngine/src/mesh.cpp
    CuraEngine/src/MeshGroup.cpp
    CuraEngine/src/Mold.cpp
    CuraEngine/src/multiVolumes.cpp
    CuraEngine/src/pathOrderOptimizer.cpp
    CuraEngine/src/Preheat.cpp
    CuraEngine/src/PrimeTower.cpp
    CuraEngine/src/raft.cpp
    CuraEngine/src/Scene.cpp
    CuraEngine/src/skin.cpp
    CuraEngine/src/SkirtBrim.cpp
    CuraEngine/src/SupportInfillPart.cpp
    CuraEngine/src/Slice.cpp
    CuraEngine/src/sliceDataStorage.cpp
    CuraEngine/src/slicer.cpp
    CuraEngine/src/support.cpp
    CuraEngine/src/timeEstimate.cpp
    CuraEngine/src/TopSurface.cpp
    CuraEngine/src/TreeSupport.cpp
    CuraEngine/src/WallsComputation.cpp
    CuraEngine/src/wallOverlap.cpp
    CuraEngine/src/Weaver.cpp
    CuraEngine/src/Wireframe2gcode.cpp

    CuraEngine/src/communication/ArcusCommunication.cpp
    CuraEngine/src/communication/ArcusCommunicationPrivate.cpp
    CuraEngine/src/communication/CommandLine.cpp
    CuraEngine/src/communication/Listener.cpp

    CuraEngine/src/infill/ImageBasedDensityProvider.cpp
    CuraEngine/src/infill/NoZigZagConnectorProcessor.cpp
    CuraEngine/src/infill/ZigzagConnectorProcessor.cpp
    CuraEngine/src/infill/SierpinskiFill.cpp
    CuraEngine/src/infill/SierpinskiFillProvider.cpp
    CuraEngine/src/infill/SpaghettiInfill.cpp
    CuraEngine/src/infill/SpaghettiInfillPathGenerator.cpp
    CuraEngine/src/infill/SubDivCube.cpp
    CuraEngine/src/infill/GyroidInfill.cpp

    CuraEngine/src/pathPlanning/Comb.cpp
    CuraEngine/src/pathPlanning/GCodePath.cpp
    CuraEngine/src/pathPlanning/LinePolygonsCrossings.cpp
    CuraEngine/src/pathPlanning/NozzleTempInsert.cpp
    CuraEngine/src/pathPlanning/TimeMaterialEstimates.cpp

    CuraEngine/src/progress/Progress.cpp
    CuraEngine/src/progress/ProgressStageEstimator.cpp

    CuraEngine/src/settings/AdaptiveLayerHeights.cpp
    CuraEngine/src/settings/FlowTempGraph.cpp
    CuraEngine/src/settings/PathConfigStorage.cpp
    CuraEngine/src/settings/Settings.cpp

    CuraEngine/src/utils/AABB.cpp
    CuraEngine/src/utils/AABB3D.cpp
    CuraEngine/src/utils/Date.cpp
    CuraEngine/src/utils/gettime.cpp
    CuraEngine/src/utils/getpath.cpp
    CuraEngine/src/utils/LinearAlg2D.cpp
    CuraEngine/src/utils/ListPolyIt.cpp
    CuraEngine/src/utils/logoutput.cpp
    CuraEngine/src/utils/MinimumSpanningTree.cpp
    CuraEngine/src/utils/Point3.cpp
    CuraEngine/src/utils/PolygonConnector.cpp
    CuraEngine/src/utils/PolygonsPointIndex.cpp
    CuraEngine/src/utils/PolygonProximityLinker.cpp
    CuraEngine/src/utils/polygonUtils.cpp
    CuraEngine/src/utils/polygon.cpp
    CuraEngine/src/utils/ProximityPointLink.cpp
    CuraEngine/src/utils/SVG.cpp
    CuraEngine/src/utils/socket.cpp
)

# Compiling CuraEngine itself.
add_library(CuraEngine STATIC ${engine_SRCS} ${engine_PB_SRCS}) #First compile all of CuraEngine as library, allowing this to be re-used for tests.

if (CuraEngine_Download_Stb)
    add_dependencies(CuraEngine stb)
endif()

target_link_libraries(CuraEngine clipper)

set_target_properties(CuraEngine PROPERTIES COMPILE_DEFINITIONS "VERSION=\"${CURA_ENGINE_VERSION}\"")

if(WIN32)
  message(STATUS "Using windres")
  set(RES_FILES "CuraEngine.rc")
  ENABLE_LANGUAGE(RC)
  if(NOT MSVC)
    SET(CMAKE_RC_COMPILER_INIT windres)
    SET(CMAKE_RC_COMPILE_OBJECT
        "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>"
        )
 endif()
endif(WIN32)

if (UNIX)
    target_link_libraries(CuraEngine pthread)
endif()
