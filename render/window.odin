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

	context = window.window_context;
	
	sync.lock(&window.input_events_mutex);
	defer sync.unlock(&window.input_events_mutex);

	assert(window != nil, "window is nil");

	event : Key_input_event = {
		glfw_handle = window.glfw_window,
		key = auto_cast key,
		scancode = auto_cast scancode,
		action = auto_cast action,
		mods = transmute(Input_modifier) mods,
	}
	
	queue.append(&window.key_input_events, event);
}

button_callback : glfw.MouseButtonProc : proc "c" (glfw_window : glfw.WindowHandle, button, action, mods : i32) {
	window : ^Window = cast(^Window)glfw.GetWindowUserPointer(glfw_window);
		
	context = window.window_context;
	
	sync.lock(&window.input_events_mutex);
	defer sync.unlock(&window.input_events_mutex);
	
	assert(window != nil, "window is nil");

	event : Mouse_input_event = {
		glfw_handle = window.glfw_window,
		button = auto_cast button,
		action = auto_cast action,
		mods = transmute(Input_modifier) mods,
	}
	
	queue.append(&window.button_input_events, event);
}

scroll_callback : glfw.ScrollProc : proc "c" (glfw_window : glfw.WindowHandle, xoffset, yoffset: f64) {
	window : ^Window = cast(^Window)glfw.GetWindowUserPointer(glfw_window);
	
	context = window.window_context;

	sync.lock(&window.input_events_mutex);
	defer sync.unlock(&window.input_events_mutex);

	assert(window != nil, "window is nil");

	queue.append(&window.scroll_input_event, [2]f32{auto_cast xoffset, auto_cast yoffset});
}

error_callback : glfw.ErrorProc : proc "c" (error: i32, description: cstring) {
	context = runtime.default_context();
	fmt.panicf("Recvied GLFW error : %v, text : %s", error, description);
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

	context = window.window_context;

	sync.lock(&window.input_events_mutex);
	defer sync.unlock(&window.input_events_mutex);

	assert(window != nil, "window is nil");

	queue.append(&window.char_input_buffer, codepoint);
}

bound_window : Maybe(^Window) = nil;

Window :: struct {
	
	glfw_window : glfw.WindowHandle, //dont touch
	startup_timer : time.Stopwatch,
	frame_timer : time.Stopwatch,

	window_context : runtime.Context,

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

destroy_window :: proc(using s : ^Render_state($U,$A), loc :=  #caller_location) {
	
	unbind_window(s, window);

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

bind_window :: proc(using s : ^Render_state($U,$A), loc := #caller_location) {
	
	if bound_window != nil {
		panic("Another window is already bound", loc = loc);
	}

	glfw.MakeContextCurrent(window.glfw_window);
	bound_window = &s.window;

	w, h := glfw.GetFramebufferSize(window.glfw_window)
	current_render_target_width = auto_cast w;
	current_render_target_height = auto_cast h;
	set_view(s);
}

unbind_window :: proc(using s : ^Render_state($U,$A), loc := #caller_location) {
	
	if v, ok := bound_window.?; ok {
		assert(v == &s.window, "You are not unbinding the currently bound window", loc = loc);
	}
	else {
		panic("Ther is currently no bound window", loc = loc);
	}

	glfw.MakeContextCurrent(nil);
	bound_window = nil;
}

begin_frame :: proc(using s : ^Render_state($U,$A), clear_color : [4]f32 = {0,0,0,1}, loc := #caller_location) {

	when ODIN_DEBUG {
		assert(s.is_begin_render == false, "begin render has already been called this frame.", loc = loc);
		s.is_begin_render = true;
	}

	time.stopwatch_start(&s.window.frame_timer);

	w, h := glfw.GetWindowSize(window.glfw_window)
	current_render_target_width = auto_cast w;
	current_render_target_height = auto_cast h;

	set_view(s);
	
	glfw.PollEvents();
	clear_color_depth(s, clear_color);

	begin_inputs(s);
} 

end_frame :: proc(using s : ^Render_state($U,$A), loc := #caller_location) {
	
	when ODIN_DEBUG {
		assert(s.is_begin_render == true, "begin render has not been called this frame.", loc = loc);
		s.is_begin_render = false;
	}

	end_inputs(s);
	
	if v, ok := bound_window.?; ok {
		assert(&s.window == v, "the window you are updating is not the bound window", loc = loc);
	}
	else {
		panic("There must be a bound window", loc = loc);
	}

	glfw.SwapBuffers(window.glfw_window);
	
	time.stopwatch_stop(&s.window.frame_timer);
	ms := time.duration_seconds(time.stopwatch_duration(s.window.frame_timer));
	glfw.SetWindowTitle(window.glfw_window, fmt.ctprintf("%s (%iFPS)", "my window : ", i32(1.0/(ms))));
	time.stopwatch_reset(&s.window.frame_timer);
}

set_view :: proc(using s : ^Render_state($U,$A)) {
	set_viewport(s, 0, 0, auto_cast current_render_target_width, auto_cast current_render_target_height);
}

should_close :: proc(using s : ^Render_state($U,$A)) -> bool {
	return auto_cast glfw.WindowShouldClose(window.glfw_window);
}

enable_vsync :: proc(using s : ^Render_state($U,$A), enable : bool) {
	glfw.SwapInterval(auto_cast enable);
}

get_screen_width :: proc(using s : ^Render_state($U,$A), loc := #caller_location) -> i32{
	
	w, h := glfw.GetFramebufferSize(window.glfw_window);
	return w;
}

get_screen_height :: proc(using s : ^Render_state($U,$A), loc := #caller_location) -> i32 {
	
	w, h := glfw.GetFramebufferSize(window.glfw_window);
	return h;
}

mouse_mode :: proc(using s : ^Render_state($U,$A), mouse_mode : Mouse_mode, loc := #caller_location) {
	
	glfw.SetInputMode(v.glfw_window, glfw.CURSOR, auto_cast mouse_mode);
}

//The image data is 32-bit, little-endian, non-premultiplied RGBA, i.e. eight bits per channel. The pixels are arranged canonically as sequential rows, starting from the top-left corner.
set_cursor :: proc(using s : ^Render_state($U,$A), cursor : []u8, size : i32, loc := #caller_location) {
	
	fmt.assertf(len(cursor) == auto_cast(size * size * 4), "Size does not match array data. Data length : %v, expected : %v\n", len(cursor), size * size * 4, loc = loc)

	image : glfw.Image;
	image.width = size;
	image.height = size;
	image.pixels = raw_data(cursor);
	
	cursor : glfw.CursorHandle = glfw.CreateCursor(&image, 0, 0); //TODO this is leaked, i belive.
	glfw.SetCursor(window.glfw_window, cursor);
	//glfw.DestroyCursor(cursor);
}

delta_time :: proc(using s : ^Render_state($U,$A)) -> f32 {
	return 1.0/120;
}

time_since_window_creation :: proc(using s : ^Render_state($U,$A)) -> f64 {
	
	return time.duration_seconds(time.stopwatch_duration(v.startup_timer));
}