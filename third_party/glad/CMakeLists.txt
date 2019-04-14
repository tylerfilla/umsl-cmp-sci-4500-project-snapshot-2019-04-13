#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

cmake_minimum_required(VERSION 3.4)
project(glad)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)
