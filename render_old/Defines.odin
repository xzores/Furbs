package render;

import glfw "vendor:glfw"
import gl "vendor:OpenGL"

import glsl "core:math/linalg/glsl"
import linalg "core:math/linalg"

import c "core:c/libc"

////////////////////////////////////////////////////////////////////

		// THESE DEFINES WHAT CANE BE IN A SHADER // 

//locations may overlap as long as there is only of the overlapping in use at a time.

//////////////////////////////////////////////////////////

Shader_program_id :: distinct u32;
Shader_vertex_id :: distinct u32;
Shader_fragment_id :: distinct u32;

Texture_id :: distinct u32;
Render_buffer_id :: distinct u32;
Frame_buffer_id :: distinct u32;

Attribute_id :: distinct i32;
Uniform_id :: distinct i32;

Vao_ID :: distinct i32; //TODO small ID
Vbo_ID :: distinct i32; //TODO

//Not an opengl thing
Texture_slot :: distinct i32;

Shader_type :: enum {
	vertex_shader,
	fragment_shader,
}

Uniform_type :: enum u32 {
	invalid 		= 0,
	float 			= gl.FLOAT,
	vec2 			= gl.FLOAT_VEC2,
	vec3 			= gl.FLOAT_VEC3,
	vec4 			= gl.FLOAT_VEC4,
	int 			= gl.INT,
	ivec2 			= gl.INT_VEC2,
	ivec3 			= gl.INT_VEC3,
	ivec4 			= gl.INT_VEC4,
	uint 			= gl.UNSIGNED_INT,
	uvec2 			= gl.UNSIGNED_INT_VEC2,
	uvec3 			= gl.UNSIGNED_INT_VEC3,
	uvec4 			= gl.UNSIGNED_INT_VEC4,
	bool 			= gl.BOOL,
	mat2 			= gl.FLOAT_MAT2,
	mat3 			= gl.FLOAT_MAT3,
	mat4 			= gl.FLOAT_MAT4,
	sampler_1d 		= gl.SAMPLER_1D,
	sampler_2d 		= gl.SAMPLER_2D,
	sampler_3d 		= gl.SAMPLER_3D,
	sampler_cube	= gl.SAMPLER_CUBE,
	isampler_1d 	= gl.INT_SAMPLER_1D,
	isampler_2d 	= gl.INT_SAMPLER_2D,
	isampler_3d 	= gl.INT_SAMPLER_3D,
	isampler_cube 	= gl.INT_SAMPLER_CUBE,
	isampler_buffer = gl.INT_SAMPLER_BUFFER,
	//TODO should we support more? : https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
}

Attribute_type :: enum u32 {
	invalid 		= 0,
	float 			= gl.FLOAT,
	vec2 			= gl.FLOAT_VEC2,
	vec3 			= gl.FLOAT_VEC3,
	vec4 			= gl.FLOAT_VEC4,
	int 			= gl.INT,
	ivec2 			= gl.INT_VEC2,
	ivec3 			= gl.INT_VEC3,
	ivec4 			= gl.INT_VEC4,
	uint 			= gl.UNSIGNED_INT,
	uvec2 			= gl.UNSIGNED_INT_VEC2,
	uvec3 			= gl.UNSIGNED_INT_VEC3,
	uvec4 			= gl.UNSIGNED_INT_VEC4,
	//mat2 			= gl.FLOAT_MAT2,
	//mat3 			= gl.FLOAT_MAT3,
	//mat4 			= gl.FLOAT_MAT4,
}

//return the "entries" or number of dimensions. numbers are between 0 and 4.
get_attribute_type_dimensions :: proc (at : Attribute_type) -> int {
	switch at {
		case .invalid:
			return 0;
		case .float, .int, .uint:
			return 1;
		case .vec2, .ivec2, .uvec2:
			return 2; 
		case .vec3, .ivec3, .uvec3:
			return 3;
		case .vec4, .ivec4, .uvec4:
			return 4;
	}

	return 0;
}

get_attribute_primary_type :: proc (at : Attribute_type) -> Attribute_primary_type {
	switch at {
		case .invalid:
			return .invalid;
		case .float, .vec2, .vec3, .vec4: 
			return .float;
		case .int, .ivec2, .ivec3, .ivec4:
			return .int; 
		case .uint, .uvec2, .uvec3, .uvec4:
			return .uint;
	}

	return .invalid:
}

Attribute_primary_type :: enum u32 {
	invalid 		= 0,
	float 			= gl.FLOAT,
	int 			= gl.INT,
	uint 			= gl.UNSIGNED_INT,
}

get_attribute_primary_byte_len :: proc (at : Attribute_primary_type) -> int {
	switch at {
		case .invalid:
			return 0;
		case .float: 
			return size_of(f32);
		case .int:
			return size_of(i32); 
		case .uint:
			return size_of(u32); 
	}
}

Uniform_info :: struct {
	location : Uniform_id,
	uniform_type : Uniform_type,
	array_size : i32,
}

Attribute_info :: struct {
	location : Attribute_id,
	attribute_type : Attribute_type,
}

// Shader attribute data types
Shader_attribute_data_type :: enum c.int {
	float = 0,         // Shader attribute type: float
	vector2,              // Shader attribute type: vec2 (2 float)
	vector3,              // Shader attribute type: vec3 (3 float)
	vector4,              // Shader attribute type: vec4 (4 float)
}

Depth_format :: enum {
	bits_auto = gl.DEPTH_COMPONENT,
	bits_16 = gl.DEPTH_COMPONENT16,
	bits_24 = gl.DEPTH_COMPONENT24,
	bits_32 = gl.DEPTH_COMPONENT32,
}

format_info :: proc(format : Pixel_format) -> (gl_name : c.uint, channels : c.int) {
	
	if format == .uncompressed_R8 {
		return gl.RED, 1;
	}
	else if format == .uncompressed_RG8 {
		return gl.RG, 2;
	}
	else if format == .uncompressed_RGB8 {
		return gl.RGB, 3;
	}
	else if format == .uncompressed_RGBA8 {
		return gl.RGBA, 4;
	}
	else {
		panic("Unsupported pixel format");
	}
}

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
Pixel_format :: enum c.int {
	invalid = 0,
	uncompressed_R8 = gl.R8,
	uncompressed_RG8 = gl.RG8,
	uncompressed_RGB8 = gl.RGB8,
	uncompressed_RGBA8 = gl.RGBA8,
}

GL_version :: enum {
	invalid = 0,
	opengl_3_0,
	opengl_3_1,
	opengl_3_2,
	opengl_3_3,
	opengl_4_0,
	opengl_4_1,
	opengl_4_2,
	opengl_4_3,
	opengl_4_4,
	opengl_4_5,
	opengl_4_6,
}

get_gl_major :: proc(version : GL_version) -> int {
	
	if version >= .opengl_4_0 {
		return 4;
	}
	
	if version >= .opengl_3_0 {
		return 3;
	}

	return 0;
} 

get_gl_minor :: proc(version : GL_version) -> int {
	
	switch version {
		case .opengl_3_0, .opengl_4_0:
			return 0;
		case .opengl_3_1, .opengl_4_1:
			return 1;
		case .opengl_3_2, .opengl_4_2:
			return 2;
		case .opengl_3_3, .opengl_4_3:
			return 3;
		case .opengl_4_4:
			return 4;
		case .opengl_4_5:
			return 5;
		case .opengl_4_6:
			return 6;
		case .invalid:
			return 0;
		case:
			return 0;
	}

	return 0;
}

//////////////////
Anchor_point :: enum {
    top_left,
    top_center,
    top_right,
    center_left,
    center_center,
    center_right,
    bottom_left,
    bottom_center,
    bottom_right,
}
