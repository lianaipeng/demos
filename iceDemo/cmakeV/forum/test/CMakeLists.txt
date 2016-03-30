# 源文件
set(FORUM_CLIENT_SRC
    main.cpp ../interface/Forum.h ../interface/Forum.cpp
    )

add_executable(forum_client ${FORUM_CLIENT_SRC})

target_link_libraries(forum_client
        Glacier2 IceGrid Ice IceUtil
        )

