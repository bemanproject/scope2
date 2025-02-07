# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(beman.scope)
add_library(beman::scope ALIAS beman.scope)

target_sources(beman.scope PRIVATE scope.cpp)

target_sources(
    beman.scope
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES ${PROJECT_SOURCE_DIR}/include/beman/scope/scope.hpp
)

set_target_properties(beman.scope PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

install(
    TARGETS beman.scope
    EXPORT beman.scope
    DESTINATION
    $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
