cmake_minimum_required(VERSION 3.10)

# Establecer políticas
cmake_policy(SET CMP0081 NEW)

# Nombre del proyecto
project(MS_Camera LANGUAGES CXX)

# Configuración de las opciones de compilación para Debug y Release
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Variables de ruta
set(ORBITER_ROOT "C:/Users/Matias_Saibene/Desktop/OpenOrbiter2024")
set(ORBITER_SDK_INCLUDE "${ORBITER_ROOT}/Orbitersdk/include")
set(ORBITER_SDK_LIB_RELEASE "${ORBITER_ROOT}/Orbitersdk/lib/Orbitersdk.lib")
set(ORBITER_SDK_LIB_DEBUG "${ORBITER_ROOT}/Orbitersdk/lib_debug")
set(ORBITER_MODULES "${ORBITER_ROOT}/Modules")
set(ORBITER_SOURCE "${ORBITER_ROOT}/sources/Multistage2015")

file(TO_CMAKE_PATH "${ORBITER_SDK_LIB_DEBUG}" ORBITER_SDK_LIB_DEBUG)

file(GLOB ORBITER_LIBS_DEBUG "${ORBITER_SDK_LIB_DEBUG}/*.lib")

# Archivos fuente y encabezados
set(SOURCES
    MS_Camera.cpp
)
set(HEADERS
    MS_Camera.h
)

# Crear el objetivo como una librería compartida (DLL)
add_library(${PROJECT_NAME} SHARED 
    ${SOURCES}
    ${HEADERS}
)


# Rutas de inclusión
target_include_directories(${PROJECT_NAME} PRIVATE
    "${ORBITER_SDK_INCLUDE}"
    "${ORBITER_SOURCE}"
)

# Enlace a librerías: versiones específicas por configuración
target_link_libraries(${PROJECT_NAME}
    debug ${ORBITER_LIBS_DEBUG}
    optimized ${ORBITER_SDK_LIB_RELEASE}
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MD>
    )
endif()

# Salida en la carpeta Modules para Debug y Release
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ORBITER_MODULES}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ORBITER_MODULES}"
)