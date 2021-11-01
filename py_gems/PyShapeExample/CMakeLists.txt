
set(o3de_gem_path ${CMAKE_CURRENT_LIST_DIR})
set(o3de_gem_json ${o3de_gem_path}/gem.json)
o3de_read_json_key(o3de_gem_name ${o3de_gem_json} "gem_name")
o3de_restricted_path(${o3de_gem_json} o3de_gem_restricted_path)

add_subdirectory(Code)
