#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

cmake_minimum_required(VERSION 3.4)
project(cozmonaut)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GetGitRevisionDescription)

get_git_head_revision(GIT_REFSPEC GIT_HASH)

set(cozmo_SRC_FILES
        src/global.c
        src/main.c
        )

add_executable(cozmo ${cozmo_SRC_FILES})
set_target_properties(cozmo PROPERTIES C_STANDARD 99)

# Git-related definitions
target_compile_definitions(cozmo PRIVATE
        -D__git_hash=${GIT_HASH}
        -D__git_refspec=${GIT_REFSPEC}
        )
