cmake_minimum_required(VERSION 3.20)

set(CORE_PROJECT_NAME LabEngineCore)

set(CORE_PRIVATE_INCLUDES
	src/Window.hpp
	src/Rendering/OpenGL/ShaderProgram.hpp
	src/Rendering/OpenGL/VertexBuffer.hpp
	src/Rendering/OpenGL/VertexArray.hpp
	src/Rendering/OpenGL/IndexBuffer.hpp
)

set(CORE_PUBLIC_INCLUDES
	includes/Application.hpp	
	includes/Event.hpp
	includes/Log.hpp
)
set(CORE_PRIVATE_SOURCES
	src/Application.cpp
	src/Window.cpp
	src/Rendering/OpenGL/ShaderProgram.cpp
	src/Rendering/OpenGL/VertexBuffer.cpp
	src/Rendering/OpenGL/VertexArray.cpp
	src/Rendering/OpenGL/IndexBuffer.cpp
)

if(MSVC)
	 add_compile_options(/MP)
endif()

set(CORE_ALL_SOURCES
	${CORE_PUBLIC_INCLUDES}
	${CORE_PRIVATE_INCLUDES}
	${CORE_PRIVATE_SOURCES}	
)

add_library(${CORE_PROJECT_NAME} STATIC
	${CORE_PUBLIC_INCLUDES}
	${CORE_PRIVATE_INCLUDES}
	${CORE_PRIVATE_SOURCES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${CORE_ALL_SOURCES}
)

target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${CORE_PROJECT_NAME} PRIVATE src)
#glm
add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glm)

#glfw
add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glfw)

#glew
add_definitions(-DGLEW_STATIC)
option(glew-cmake_BUILD_SHARED off FORCE)
add_subdirectory(../external/glew ${CMAKE_CURRENT_BINARY_DIR}/glew)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE libglew_static)

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)
set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_demo.cpp
	../external/imgui/imgui_draw.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
)
add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)
target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE libglew_static glfw)

target_link_libraries(${CORE_PROJECT_NAME} PRIVATE ImGui)