include(GNUInstallDirs)

set(MONAD_CXX_SOURCES_ROOT
        include/MonadCXX.h)

set(MONAD_CXX_SOURCES_SUB
        include/MonadCXX/MaybeMonad.h
        include/MonadCXX/MonadHelper.h
        include/MonadCXX/StateMonad.h)

set(MONAD_CXX_SOURCES ${MONAD_CXX_SOURCES_ROOT} ${MONAD_CXX_SOURCES_SUB})

add_library(MonadCXX INTERFACE)

target_include_directories(
        MonadCXX
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS MonadCXX
        EXPORT MonadCXX_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("MonadCXXConfigVersion.cmake"
        VERSION 0.1.0
        COMPATIBILITY SameMajorVersion)
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/MonadCXXConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/MonadCXXConfig.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/MonadCXX/cmake)

install(EXPORT MonadCXX_Targets
        FILE MonadCXXTargets.cmake
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/MonadCXX/cmake)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/MonadCXXConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/MonadCXXConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/MonadCXX/cmake)

install(FILES ${MONAD_CXX_SOURCES_ROOT} DESTINATION include)
install(FILES ${MONAD_CXX_SOURCES_SUB} DESTINATION include/MonadCXX)
