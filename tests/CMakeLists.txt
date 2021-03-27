find_package(GTest REQUIRED)

function(AddMonadCXXTest NAME)
    add_executable(${NAME}
            ${ARGN})
    target_link_libraries(${NAME}
            MonadCXX
            GTest::Main)
    set_target_properties(${NAME}
            PROPERTIES
            LINKER_LANGUAGE CXX)
    gtest_discover_tests(${NAME} XML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

AddMonadCXXTest(TestMaybeMonad TestMaybeMonad.cpp)
AddMonadCXXTest(TestStateMonad TestStateMonad.cpp)