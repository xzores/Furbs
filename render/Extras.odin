package render;

import "core:fmt"
import "core:math"
import "core:math/linalg"
import "core:math/linalg/glsl"
import "core:time"

//TODO move to state
arrow_init 		: bool;

shapes_pipeline 	: Pipeline;
overlay_pipeline 	: Pipeline;
arrow_fbo			: Frame_buffer;

//Used internally
get_coordinate_overlay_texture :: proc(camera : Camera3D, texture_size : [2]i32 = {256,256}, loc := #caller_location) -> Texture2D {

	camera := camera;
	
	if arrow_init == false {
		//s, ok := load_shader_from_path("my_shader.glsl"); assert(ok == nil);
		shapes_pipeline = make_pipeline(get_default_shader(), .no_blend, true, true, .fill, .no_cull);
		overlay_pipeline = make_pipeline(get_default_shader(), .blend, true, true, .fill, .no_cull);
		init_frame_buffer_textures(&arrow_fbo, 1, texture_size.x, texture_size.y, .RGBA8, .depth_component16, false, .linear);
		arrow_init = true;
	}
	
	f := camera_forward(camera);
	r := camera_right(camera);
	u := camera.up;

	overlay_camera : Camera3D = {
		position		= -f,            	// Camera position
		target			= {0,0,0},       // Camera target it looks-at
		up				= {0,1,0},            			// Camera up vector (rotation over its axis)
		fovy			= 45,                			// Camera field-of-view apperture in Y (degrees) in perspective
		ortho_height 	= 2,							// Camera ortho_height when using orthographic projection
		projection		= .orthographic, 				// Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
		
		near 			= -1,
		far 			= 2,
	}

	begin_target(&arrow_fbo, [4]f32{0,0,0,0});
		begin_pipeline(shapes_pipeline, overlay_camera);
			set_texture(.texture_diffuse, get_white_texture());
			draw_arrow({0,0,0}, {1,0,0},  [4]f32{0.8, 0.1, 0.1, 1});
			draw_arrow({0,0,0}, {0,1,0},  [4]f32{0.1, 0.8, 0.1, 1});
			draw_arrow({0,0,0}, {0,0,1},  [4]f32{0.1, 0.1, 0.8, 1});
		end_pipeline();
	end_target();

	return arrow_fbo.color_attachments[0].(Texture2D);
	//return arrow_fbo.depth_attachment.(Texture2D);
}

//offset is in "screen coordinates" from top right corner.
draw_coordinate_overlay :: proc (target : Render_target, camera : Camera3D, offset : [2]f32 = {0.05, 0.05}, scale : f32 = 0.25, loc := #caller_location) {
	
	assert(state.current_target == nil, "There must not be a target, call end_target", loc);
	
	tex := get_coordinate_overlay_texture(camera);

	cam : Camera2D = {
		position 		= {0,0},		// Camera position
		target_relative = {0,0},		// 
		rotation 		= 0,			// in degrees
		zoom 			= 1,            //
		near 			= -10,
		far 			= 10,
	};

	begin_target(target, nil);
		aspect : f32 = state.target_pixel_width / state.target_pixel_height;
		begin_pipeline(overlay_pipeline, cam);
			set_texture(.texture_diffuse, tex);
			draw_quad(linalg.matrix4_from_trs_f32([3]f32{(aspect) - offset.x - scale/2, 1.0 - offset.y - scale/2, 0}, 0, {scale,scale,1}));
		end_pipeline();
	end_target();
}

//offset is in "screen coordinates" from top left corner.
draw_fps_overlay :: proc (target : Render_target, offset : [2]f32 = {0,0}, scale : f32 = 1) {
	
	//A low pass filter XD
	smoothing : f32 = 0.92; // larger=more smoothing
	state.fps_measurement = (state.fps_measurement * smoothing) + (state.delta_time * (1.0-smoothing))
	
	t := fmt.aprintf("fps : %i", cast(int)(1.0 / state.fps_measurement));
	defer delete(t);

	color : [4]f32 = {1,1,1,1};
	
	size := scale * 50;
	text_dim := get_text_dimensions(t, size, 0);

	begin_target(target, nil);
		draw_text_simple(t, {offset.x, state.target_pixel_height - text_dim.y - offset.y}, size, 0, color);
	end_target();
}