qt5_add_resources(QT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/icons.qrc
                  ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/canvas.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/canvas_config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cached_resource.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/draw_history.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/draw_history.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/canvas.cpp)

add_executable(
  ${CMAKE_PROJECT_NAME}
  ${QT_RESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/qtquickcontrols2.conf
  ${SOURCE_FILES})

target_link_libraries(
  ${CMAKE_PROJECT_NAME} PRIVATE project_options project_warnings Qt5::Widgets
                                Qt5::Qml Qt5::Quick)
