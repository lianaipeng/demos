
message(" src ###### ${PROJECT_SOURCE_DIR}/interface/")
# 源文件
set(FORUM_SERVER_SRC
    main.cpp ../interface/Forum.h ../interface/Forum.cpp ForumI.h ForumI.cpp
    )

#set(DUMMY_SERVER_SRC
#    src/follow/dummy_server.cpp
#    src/follow/followI.cpp src/follow/followI.h
#    src/follow/Follow.cpp src/follow/Follow.h
#    src/follow/follow_dao.cpp src/follow/follow_dao.h
#    src/common/redis_util.h
#    src/common/log.cpp src/common/log.h
#    src/common/zlib_util.h src/common/timer.h
#    src/common/mysql_pool.cpp src/common/mysql_pool.h
#        src/common/globals.cpp src/common/globals.h
#    src/common/statsd_client.cpp src/common/statsd_client.h)

#add_executable(dummy_server ${DUMMY_SERVER_SRC})
add_executable(forum_server ${FORUM_SERVER_SRC})
#add_executable(follow_client ${FOLLOW_CLIENT_SRC})

#target_link_libraries(dummy_server
#        Glacier2 IceGrid Ice IceUtil
#        boost_log_setup boost_thread boost_iostreams boost_log boost_system
#        _acl_cpp _protocol _acl mysqlcppconn perconaserverclient
#        pthread z
#        )

target_link_libraries(forum_server
        Glacier2 IceGrid Ice IceUtil
        )

#target_link_libraries(follow_client
#    Glacier2 IceGrid Ice IceUtil
#    pthread
#)
