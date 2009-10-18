#set(SRCS main.cpp mainwindow.cpp)
#set(MOC_HDRS mainwindow.h)
#QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})
#set(UIS mainwindow.ui)
#QT4_WRAP_UI(UIS_H ${UIS})
#link_libraries(${QT_LIBRARIES})
#add_executable(test ${SRCS} ${UIS_H} ${MOC_SRCS})

set(UTIL_SRCS sql_util.cpp
		xml_util.cpp)
add_library(util ${UTIL_SRCS})
