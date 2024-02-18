package render;

import "core:strings"
import "core:runtime"
import "core:fmt"
import "core:container/queue"
import "core:sync"
import "core:mem"
import "core:time"

import "vendor:glfw"
import gl "vendor:OpenGL"

import fs "vendor:fontstash"

Mouse_mode :: enum {
	locked = glfw.CURSOR_DISABLED,
	hidden = glfw.CURSOR_HIDDEN,
	normal = glfw.CURSOR_NORMAL,
}

key_callback : glfw.KeyProc : proc "c" (glfw_window : glfw.WindowHandle, key : i32, scancode : i32, action : i32, mods : i32) {	
	window : ^Window = cast(^Window)glfw.GetWindowUserPointer(glfw_window);

	sync.lock(&window.input_events_mutex);
	defer sync.unlock(&window.input_events_mutex);

	context = window.context;

	event : Key_input_event = {
		glfw_handle = glfw_window,
		key = auto_cast key,
		scancode = auto_cast scancode,
		action = auto_cast action,
		mods = transmute(Input_modifier) mods,
	}
	
	queue.append(&window.key_input_events, event);
}

button_callback : glfw.MouseButtonProc : proc "c" (glfw_window : glfw.WindowHandle, button, action, mods : i32) {
	window : ^Window = cast(^Window)glfw.GetWindowUserPointer(glfw_window);
	
	sync.lock(&window.input_events_mutex);
	defer sync.unlock(&window.input_events_mutex);
	
	context = window.context;

	event : Mouse_input_event = {
		glfw_handle = glfw_handle,
		button = auto_cast button,
		action = auto_cast action,
		mods = transmute(Input_modifier) mods,
	}
	
	queue.append(&window.button_input_events, event);
}

scroll_callback : glfw.ScrollProc : proc "c" (glfw_window : glfw.WindowHandle, xoffset, yoffset: f64) {
	window : ^Window = cast(^Window)glfw.GetWindowUserPointer(glfw_window);
	
	context = window.context;

	sync.lock(&window.input_events_mutex);
	defer sync.unlock(&window.input_events_mutex);

	queue.append(&window.scroll_input_event, [2]f32{auto_cast xoffset, auto_cast yoffset});
}

/*
input_callback : glfw.CharProc : proc "c" (window : glfw.WindowHandle, codepoint: rune) {
	sync.lock(&input_events_mutex);
	defer sync.unlock(&input_events_mutex);

	context = window_context;
	
	queue.append(&char_input_buffer, codepoint);
}
*/

input_callback : glfw.CharModsProc : proc "c" (glfw_window : glfw.WindowHandle, codepoint: rune, mods : i32) {
	window : ^Window = cast(^Window)glfw.GetWindowUserPointer(glfw_window);

	sync.lock(&window.input_events_mutex);
	defer sync.unlock(&window.input_events_mutex);

	context = window_context;
	
	queue.append(&window.char_input_buffer, codepoint);
}

Window :: struct {
	
	glfw_window : glfw.WindowHandle, //dont touch
	title : string, //dont change yourself, use set_title
	startup_timer : time.Stopwatch,
	frame_timer : time.Stopwatch,

	context : Context;

	//Current key state
	keys_down 		: #sparse [Key_code]bool,
	keys_released 	: #sparse [Key_code]bool,
	keys_pressed 	: #sparse [Key_code]bool,
	keys_triggered 	: #sparse [Key_code]bool,

	button_input_events : queue.Queue(Mouse_input_event),
	button_release_input_events : queue.Queue(Mouse_input_event),

	scroll_input_event : queue.Queue([2]f32),
	
	//Current key state
	button_down 	: [Mouse_code]bool,
	button_released : [Mouse_code]bool,
	button_pressed 	: [Mouse_code]bool,

	mouse_pos : [2]f32,
	mouse_delta : [2]f32,
	scroll_delta : [2]f32,

	//Locks
	input_events_mutex : sync.Mutex,
	key_input_events : queue.Queue(Key_input_event),
	key_release_input_events : queue.Queue(Key_input_event),
	char_input_buffer : queue.Queue(rune),
	char_input : queue.Queue(rune),
}

//TODO move culling to a render pass
//TODO this now return a pointer.
init_window :: proc(using s : ^Render_state($U,$A), width, height : i32, title : string, required_gl_verion : Maybe(GL_version) = nil, culling : bool = true, loc := #caller_location) -> (window : ^Window) {
	
	using window;
	
	assert(render_has_been_init == false, "init_render has already been called!", loc = loc);
	render_has_been_init = true;

	glfw.WindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE);

	if(!cast(bool)glfw.Init()){
		panic("Failed to init glfw");
	}
	
	shader_folder_location = strings.clone(shader_folder);
	
	glfw.SetErrorCallback(error_callback);

	assert(render_has_been_init == true, "You must call init_render", loc = loc)
	window.title = fmt.aprintf("%s",title);
	time.stopwatch_start(&window.startup_timer);

    // Create render window.
    window.glfw_window = glfw.CreateWindow(width, height, fmt.ctprintf("%s", window.title), nil, nil)
    assert(window.glfw_window != nil, "Window or OpenGL context creation failed");
	
	glfw.MakeContextCurrent(window.glfw_window);
	glfw.SetInputMode(window.glfw_window, glfw.STICKY_KEYS, 1);

	gl.load_up_to(3, 0, glfw.gl_set_proc_address);
	version := get_gl_version();
	//TODO, enum cannot be below 3.3 //assert(version >= .opengl_3_3, "This library only supports OpenGL 3.0 or higher")
	glfw.SetKeyCallback(window.glfw_window, key_callback);
	glfw.SetMouseButtonCallback(window.glfw_window, button_callback);
	glfw.SetScrollCallback(window.glfw_window, scroll_callback);
	glfw.SetCharModsCallback(window.glfw_window, input_callback);
	
	if required_verion, ok := required_gl_verion.?; ok {
		//load the specified
		assert(version >= required_verion, "OpenGL version is not new enough for the requied version");
		gl.load_up_to(get_gl_major(required_verion), get_gl_major(required_verion), glfw.gl_set_proc_address);
		opengl_version = required_verion;
	}
	else {
		//load the newest
		gl.load_up_to(get_gl_major(version), get_gl_major(version), glfw.gl_set_proc_address);
		opengl_version = version;
	}

	fmt.printf("Loaded opengl version : %v\n", opengl_version);

	assert(get_max_supported_active_textures() >= auto_cast len(texture_locations));
	init_shaders();
	glfw.MakeContextCurrent(nil);

	//TODO 1,1 for w and h is might not be the best idea, what should we do instead?
	fs.Init(&font_context, 1, 1, .BOTTOMLEFT); //TODO try TOPLEFT and BOTTOMLEFT

	glfw.SetWindowUserPointer :: proc(window.glfw_window, window) //TODO this window and then pointer

	return;
}

destroy_window :: proc(using s : ^Render_state($U,$A), window : ^Window, loc :=  #caller_location) {

	if v, ok := bound_window.?; ok {
		assert(v != window^, "The window must be unbound before it can be delelted", loc = loc);
	}
	
	fs.Destroy(&font_context);

	glfw.DestroyWindow(window.glfw_window);
	window.glfw_window = nil;
	//TODO //window_context = {};
	delete(window.title);
	glfw.Terminate();
}

bind_window :: proc(using s : ^Render_state($U,$A), window : Window, loc := #caller_location) {

	if bound_window != nil {
		panic("Another window is already bound", loc = loc);
	}

	glfw.MakeContextCurrent(window.glfw_window);
	bound_window = window;

	w, h := glfw.GetFramebufferSize(window.glfw_window)
	current_render_target_width = auto_cast w;
	current_render_target_height = auto_cast h;
	set_view();
}

unbind_window :: proc(using s : ^Render_state($U,$A), window : Window, loc := #caller_location) {
	
	if v, ok := bound_window.?; ok {
		assert(v == window, "You are not unbinding the currently bound window", loc = loc);
	}
	else {
		panic("Ther is currently no bound window", loc = loc);
	}

	glfw.MakeContextCurrent(nil);
	bound_window = nil;
}

begin_frame :: proc(using s : ^Render_state($U,$A), window : Window, clear_color : [4]f32 = {0,0,0,1}, loc := #caller_location) {

	time.stopwatch_start(&frame_timer);

	w, h := glfw.GetWindowSize(window.glfw_window)
	current_render_target_width = auto_cast w;
	current_render_target_height = auto_cast h;

	set_view();

	glfw.PollEvents();
	clear_color_depth(clear_color);

	begin_inputs();
} 

end_frame :: proc(using s : ^Render_state($U,$A), window : Window, loc := #caller_location) {
	
	end_inputs();
	
	if v, ok := bound_window.?; ok {
		assert(window == v, "the window you are updating is not the bound window", loc = loc);
	}
	else {
		panic("There must be a bound window", loc = loc);
	}

	glfw.SwapBuffers(window.glfw_window);
	
	time.stopwatch_stop(&frame_timer);
	ms := time.duration_seconds(time.stopwatch_duration(frame_timer));
	glfw.SetWindowTitle(window.glfw_window, fmt.ctprintf("%s (%iFPS)", window.title, i32(1.0/(ms))));
	time.stopwatch_reset(&frame_timer);
}

set_view :: proc(using s : ^Render_state($U,$A)) {
	set_viewport(0, 0, auto_cast current_render_target_width, auto_cast current_render_target_height);
}

should_close :: proc(using s : ^Render_state($U,$A), window : Window) -> bool {
	return auto_cast glfw.WindowShouldClose(window.glfw_window);
}

enable_vsync :: proc(using s : ^Render_state($U,$A), enable : bool) {
	glfw.SwapInterval(auto_cast enable);
}

get_screen_width :: proc(using s : ^Render_state($U,$A), loc := #caller_location) -> i32{
	
	if window, ok := bound_window.?; ok {
		w, h := glfw.GetFramebufferSize(window.glfw_window);
		return w;
	}
	else {
		panic("No window is bound", loc = loc);
	}
}

get_screen_height :: proc(using s : ^Render_state($U,$A), loc := #caller_location) -> i32 {
	if window, ok := bound_window.?; ok {
		w, h := glfw.GetFramebufferSize(window.glfw_window);
		return h;
	}
	else {
		panic("No window is bound", loc = loc);
	}
}

mouse_mode :: proc(using s : ^Render_state($U,$A), mouse_mode : Mouse_mode, loc := #caller_location) {
	
	if v, ok := bound_window.?; ok {
		glfw.SetInputMode(v.glfw_window, glfw.CURSOR, auto_cast mouse_mode);
	}
	else {
		panic("Ther is currently no bound window", loc = loc);
	}
}

//The image data is 32-bit, little-endian, non-premultiplied RGBA, i.e. eight bits per channel. The pixels are arranged canonically as sequential rows, starting from the top-left corner.
set_cursor :: proc(using s : ^Render_state($U,$A), cursor : []u8, size : i32, loc := #caller_location) {
	
	fmt.assertf(len(cursor) == auto_cast(size * size * 4), "Size does not match array data. Data length : %v, expected : %v\n", len(cursor), size * size * 4, loc = loc)

	if window, ok := bound_window.?; ok {
		
		image : glfw.Image;
		image.width = size;
		image.height = size;
		image.pixels = raw_data(cursor);
		
		cursor : glfw.CursorHandle = glfw.CreateCursor(&image, 0, 0); //TODO this is leaked, i belive.
		glfw.SetCursor(window.glfw_window, cursor);
		//glfw.DestroyCursor(cursor);
	}
}

delta_time :: proc(using s : ^Render_state($U,$A)) -> f32 {
	return 1.0/120;
}

time_since_window_creation :: proc(using s : ^Render_state($U,$A)) -> f64 {
	
	if v, ok := bound_window.?; ok {
		return time.duration_seconds(time.stopwatch_duration(v.startup_timer));
	}

	panic("time_since_window_creation requires a boudn window");
}