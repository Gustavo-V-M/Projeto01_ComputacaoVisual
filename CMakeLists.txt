cmake_minimum_required(VERSION 3.16)
project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Diretórios de saída organizados
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")


set(SDLTTF_VENDORED ON)

# Adiciona SDL3 e SDL3_image (submodules clonados em lib/)
add_subdirectory(lib/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(lib/SDL3_image EXCLUDE_FROM_ALL)
add_subdirectory(lib/SDL3_ttf EXCLUDE_FROM_ALL)

# Fontes do seu projeto
file(GLOB SRC "src/*.cpp")
add_executable(main ${SRC})

# Headers locais
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/includes)

# Linka bibliotecas SDL
target_link_libraries(main PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)

# Copiar DLLs no Windows para a pasta bin/
if (WIN32)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:main>
    )
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_image::SDL3_image>
            $<TARGET_FILE_DIR:main>
    )
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
            $<TARGET_FILE_DIR:main>
    )
endif()

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:main>"
        "${CMAKE_SOURCE_DIR}/"
    COMMENT "Copiando executável para a raiz do projeto"
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/assets/font-roboto.ttf"
        "${CMAKE_SOURCE_DIR}/"
    COMMENT "Copiando font-roboto.ttf para a raiz do projeto"
)

if (WIN32)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            "${CMAKE_SOURCE_DIR}/"
    )
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_image::SDL3_image>
            "${CMAKE_SOURCE_DIR}/"
    )
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
            "${CMAKE_SOURCE_DIR}/"
    )
endif()
