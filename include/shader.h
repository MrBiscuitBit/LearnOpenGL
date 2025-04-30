#pragma once

#include "common.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

u32 load_shader_program(const char *vertex_shader_source, const char *fragment_shader_source);