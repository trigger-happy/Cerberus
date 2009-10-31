#set(SRCS main.cpp mainwindow.cpp)
#set(MOC_HDRS mainwindow.h)
#QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})
#set(UIS mainwindow.ui)
#QT4_WRAP_UI(UIS_H ${UIS})
#link_libraries(${QT_LIBRARIES})
#add_executable(test ${SRCS} ${UIS_H} ${MOC_SRCS})

set(NET_SRCS contestant_net.cpp
		server_net.cpp
		temp_connection.cpp
		contestant_connection.cpp
		admin_connection.cpp
		projector_connection.cpp
		projector_net.cpp)
set(NET_MHDRS ${INCLUDE_DIR}/net/contestant_net.h
		${INCLUDE_DIR}/net/server_net.h
		${INCLUDE_DIR}/net/temp_connection.h
		${INCLUDE_DIR}/net/contestant_connection.h
		${INCLUDE_DIR}/net/admin_connection.h
		${INCLUDE_DIR}/net/projector_connection.h
		${INCLUDE_DIR}/net/projector_net.h)
QT4_WRAP_CPP(NET_MSRCS ${NET_MHDRS})
add_library(network ${NET_SRCS} ${NET_MSRCS})