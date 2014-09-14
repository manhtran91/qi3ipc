# To keep the file list clean
set(hdrs_dir ${${PROJECT_NAME}_include_dir})
set(srcs_dir ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir})
set(tests_dir ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/tests)

# Headers
set(hdrs ${hdrs_dir}/qi3socket.h)

# Sources
set(srcs ${srcs_dir}/qi3socket.cpp)
