## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
##  * Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
##  * Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
##  * Neither the name of NVIDIA CORPORATION nor the names of its
##    contributors may be used to endorse or promote products derived
##    from this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
## EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
## PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
## CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
## EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
## PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
## OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
## OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
## Copyright (c) 2008-2023 NVIDIA Corporation. All rights reserved.

#
# Build Snippetvehicle2 common template
#

# Include here after the directories are defined so that the platform specific file can use the variables.
INCLUDE(${PHYSX_ROOT_DIR}/snippets/${PROJECT_CMAKE_FILES_DIR}/${TARGET_BUILD_PLATFORM}/SnippetVehicle2Template.cmake)

IF(NOT PM_RAPIDJSON_PATH_INTERNAL)
	SET(PM_RAPIDJSON_PATH_INTERNAL $ENV{PM_rapidjson_PATH} CACHE INTERNAL "rapidjson package path")
ENDIF()
SET(PM_RAPIDJSON_INCLUDE_PATH ${PM_RAPIDJSON_PATH_INTERNAL}/include)

STRING(TOLOWER ${SNIPPET_NAME} SNIPPET_NAME_LOWER)

SET(TEMP_SOURCES "")
SET(SnippetVehicle2Sources "")
FILE(GLOB TEMP_SOURCES ${PHYSX_ROOT_DIR}/snippets/snippet${SNIPPET_NAME_LOWER}/*.cpp ${PHYSX_ROOT_DIR}/snippets/snippet${SNIPPET_NAME_LOWER}/*.h)
LIST(APPEND SnippetVehicle2Sources ${TEMP_SOURCES})

SET(COMMON_DIRECTORIES base directdrivetrain enginedrivetrain physxintegration serialization)	
FOREACH(COMMON_DIRECTORY ${COMMON_DIRECTORIES})
SET(TEMP_SOURCES "")
FILE(GLOB_RECURSE TEMP_SOURCES ${PHYSX_ROOT_DIR}/snippets/snippetvehicle2common/${COMMON_DIRECTORY}/*.cpp ${PHYSX_ROOT_DIR}/snippets/snippetvehicle2common/${COMMON_DIRECTORY}/*.h ${PHYSX_ROOT_DIR}/snippets/snippetvehicle2common/${COMMON_DIRECTORY}/*.txt)
SOURCE_GROUP(common\\${COMMON_DIRECTORY} FILES ${TEMP_SOURCES})
LIST(APPEND SnippetVehicle2Sources ${TEMP_SOURCES})
ENDFOREACH()

FILE(GLOB_RECURSE RAPIDJSON_FILES ${PM_RAPIDJSON_INCLUDE_PATH}/rapidjson/*.h)
SOURCE_GROUP(common\\rapidjson FILES ${RAPIDJSON_FILES})
LIST(APPEND SnippetVehicle2Sources ${RAPIDJSON_FILES})

SET(TEMP_SOURCES "")
FILE(GLOB TEMP_SOURCES ${PHYSX_ROOT_DIR}/snippets/snippetvehicle2common/*.cpp ${PHYSX_ROOT_DIR}/snippets/snippetvehicle2common/*.h)
SOURCE_GROUP(common FILES ${TEMP_SOURCES})
LIST(APPEND SnippetVehicle2Sources ${TEMP_SOURCES})

ADD_EXECUTABLE(Snippet${SNIPPET_NAME} ${SNIPPET_BUNDLE}
	${SNIPPET_PLATFORM_SOURCES}
	${SnippetVehicle2Sources}
)

TARGET_INCLUDE_DIRECTORIES(Snippet${SNIPPET_NAME}
	PRIVATE ${SNIPPET_PLATFORM_INCLUDES}
    PRIVATE ${PM_RAPIDJSON_INCLUDE_PATH}
	
	PRIVATE ${PHYSX_ROOT_DIR}/include/
)

TARGET_COMPILE_DEFINITIONS(Snippet${SNIPPET_NAME}
	PRIVATE ${SNIPPET_COMPILE_DEFS}
)

SET_TARGET_PROPERTIES(Snippet${SNIPPET_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PX_EXE_OUTPUT_DIRECTORY_DEBUG}${EXE_PLATFORM_DIR}
    RUNTIME_OUTPUT_DIRECTORY_PROFILE ${PX_EXE_OUTPUT_DIRECTORY_PROFILE}${EXE_PLATFORM_DIR}
    RUNTIME_OUTPUT_DIRECTORY_CHECKED ${PX_EXE_OUTPUT_DIRECTORY_CHECKED}${EXE_PLATFORM_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PX_EXE_OUTPUT_DIRECTORY_RELEASE}${EXE_PLATFORM_DIR}

    OUTPUT_NAME Snippet${SNIPPET_NAME}${EXE_SUFFIX}
)

TARGET_LINK_LIBRARIES(Snippet${SNIPPET_NAME} 
	PUBLIC PhysXExtensions PhysXPvdSDK PhysX PhysXVehicle2 PhysXCharacterKinematic PhysXCooking PhysXCommon PhysXFoundation SnippetUtils 
	PUBLIC ${SNIPPET_PLATFORM_LINKED_LIBS})

IF(CUSTOM_SNIPPET_TARGET_PROPERTIES)
	SET_TARGET_PROPERTIES(Snippet${SNIPPET_NAME} PROPERTIES 
	   ${CUSTOM_SNIPPET_TARGET_PROPERTIES}
	)
ENDIF()

IF(PX_GENERATE_SOURCE_DISTRO)	
	LIST(APPEND SOURCE_DISTRO_FILE_LIST ${SNIPPET_PLATFORM_SOURCES})
	LIST(APPEND SOURCE_DISTRO_FILE_LIST ${SnippetVehicle2Sources})
ENDIF()