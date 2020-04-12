add_executable(
  SkribbleTests ${CMAKE_CURRENT_SOURCE_DIR}/cached_resource_test.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/format_test.cpp)
target_include_directories(
  SkribbleTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/helper/
                        ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
target_link_libraries(SkribbleTests PRIVATE project_options project_warnings
                                            Qt5::Core)

add_test(SkribbleTests SkribbleTests)
