add_subdirectory(ctemplate)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${ctemplate_BINARY_DIR}/include)

unset(QT_LIBRARIES)
set(QT_USE_QTWEBKIT 1)
include(${QT_USE_FILE})

set(PROJECTOR_SRC main.cpp ProjectorWindow.cpp TemplateManager.cpp DefaultTemplates.cpp)
set(PROJECTOR_HDR TemplateManager.h DefaultTemplates.h)
set(PROJECTOR_MHDR ProjectorWindow.h)
qt4_wrap_cpp(PROJECTOR_MSRC ${PROJECTOR_MHDR})

set(PROJECTOR_UIS ProjectorWindow.ui)
qt4_wrap_ui(PROJECTOR_UIS_H ${PROJECTOR_UIS})

add_executable(projector ${PROJECTOR_SRC} ${PROJECTOR_MSRC} ${PROJECTOR_UIS_H} ${PROJECTOR_HDR})

target_link_libraries(projector ${QT_LIBRARIES} ctemplate util network)
