#set(SRCS main.cpp mainwindow.cpp)
#set(MOC_HDRS mainwindow.h)
#QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})
#set(UIS mainwindow.ui)
#QT4_WRAP_UI(UIS_H ${UIS})
#link_libraries(${QT_LIBRARIES})
#add_executable(test ${SRCS} ${UIS_H} ${MOC_SRCS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
link_libraries(${QT_LIBRARIES})

set(XML_UT_SRCS xml_test.cpp ../utils/xml_util.cpp)
set(XML_UT_MHDRS ../../include/tests/xml_test.h ../../include/xml_util.h)
qt4_wrap_cpp(XML_UT_MSRCS ${XML_UT_MHDRS})
add_executable(xml_test ${XML_UT_SRCS} ${XML_UT_MSRCS})

set(SQL_UT_SRCS sql_test.cpp ../utils/sql_util.cpp)
set(SQL_UT_MHDRS ../../include/tests/sql_test.h ../../include/sql_util.h)
qt4_wrap_cpp(SQL_UT_MSRCS ${SQL_UT_MHDRS})
add_executable(sql_test ${SQL_UT_SRCS} ${SQL_UT_MSRCS})

set(CCNET_UT_SRCS contestant_net_test.cpp 
	../net/contestant_net.cpp
	../net/server_net.cpp)
set(CCNET_UT_MHDRS ../../include/tests/contestant_net_test.h 
	../../include/net/contestant_net.h
	../../include/net/server_net.h)
qt4_wrap_cpp(CCNET_UT_MSRCS ${CCNET_UT_MHDRS})
add_executable(ccnet_test ${CCNET_UT_SRCS} ${CCNET_UT_MSRCS})
