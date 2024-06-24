package render;

import "base:runtime"

import "core:fmt"
import c "core:c/libc"
import "core:os"
import "core:bytes"
import "core:mem"
import "core:slice"
import "core:log"
import "core:thread"
import "core:math"
import "core:time"

import "core:image"
import "core:image/png"

import "gl"

import "../utils"

Pixel_format_internal 	:: gl.Pixel_format_internal;
Wrapmode 				:: gl.Wrapmode;
Filtermode 				:: gl.Filtermode;

Texture_desc :: struct {
	wrapmode : Wrapmode,
	filtermode : Filtermode,
	mipmaps : bool,						// Is mipmaps enabled?
	format	: Pixel_format_internal,	// Data format (PixelFormat type)
}

/////////////////////////////////// Texture 1D ///////////////////////////////////
Texture1D :: struct {
	id			: gl.Tex1d_id,            	// OpenGL texture id
	width		: i32,               		// Texture base width

	using desc : Texture_desc,
}

@(require_results)
texture1D_make :: proc(mipmaps : bool, wrapmode : Wrapmode, filtermode : Filtermode, internal_format : Pixel_format_internal,
							 width : i32, upload_format : gl.Pixel_format_upload, data : []u8, loc := #caller_location) -> Texture1D {

	desc : Texture_desc = {
		mipmaps 		= mipmaps,
		wrapmode 		= wrapmode,
		filtermode 		= filtermode,
		format 			= internal_format,
	};

	return texture1D_make_desc(desc, width, upload_format, data, loc);
}

@(require_results)
texture1D_make_desc :: proc(using desc : Texture_desc, width : i32, upload_format : gl.Pixel_format_upload, data : []u8, loc := #caller_location) -> Texture1D {

	//gl.PixelStorei(gl.UNPACK_ALIGNMENT, 1); //TODO

	id : gl.Tex1d_id = gl.gen_texture1D(loc);
	assert(id > 0, "TEXTURE: Failed to load texture", loc);

    gl.wrapmode_texture1D(id, desc.wrapmode);
	gl.filtermode_texture1D(id, desc.filtermode, mipmaps);

	size_per_component, channels : int;
	size_per_component = gl.upload_format_component_size(upload_format);
	channels = gl.upload_format_channel_cnt(upload_format);

	gl.setup_texure_1D(id, mipmaps, width, format);

	if len(data) == 0 {
		assert(raw_data(data) == nil, "Texture data is 0 len, but is not nil", loc);
	}
	else {
		assert(upload_format != .no_upload, "upload_format is no_upload, but there is data", loc);
		length := int(cast(int)width * channels);
		fmt.assertf(len(data) == length, "Data is not in the correct format, len is %i, while it should have been %i", len(data), length, loc = loc);
		gl.write_texure_data_1D(id, 0, 0, width, upload_format, data, loc);
	}

	if mipmaps && data != nil { //If there is no data, then it makes no sense to generate mipmaps
		gl.generate_mip_maps_1D(id);
	}

	tex : Texture1D = {
		id, 
		width,
		desc,
	}

    return tex;
}

texture1D_destroy :: proc(tex : ^Texture1D) {
	gl.delete_texture1D(tex.id);
	tex^ = {};
}

texture1D_upload_data :: proc(tex : ^Texture1D, #any_int pixel_offset : i32, #any_int pixel_cnt : i32, format : gl.Pixel_format_upload, data : []u8, loc := #caller_location) {
	
	gl.write_texure_data_1D(tex.id, 0, auto_cast pixel_offset, auto_cast pixel_cnt, format, data, loc);
	
	if (tex.mipmaps) {
		gl.generate_mip_maps_1D(tex.id);
	}
}

/////////////////////////////////// Texture 2D ///////////////////////////////////
Texture2D :: struct {
	id			: gl.Tex2d_id,            	// OpenGL texture id
	width		: i32,               		// Texture base width
	height		: i32,               		// Texture base height

	using desc : Texture_desc,
}

@(require_results)
texture2D_load_from_file :: proc(filename : string, desc : Texture_desc = {.clamp_to_edge, .linear, true, .RGBA8}, loc := #caller_location) -> Texture2D {
	
	data, ok := os.read_entire_file_from_filename(filename);
	defer delete(data);

	fmt.assertf(ok, "loading texture data for %v failed", filename, loc = loc);

    return texture2D_load_from_png_bytes(desc, data, filename, loc = loc);
}

//Load many textures threaded, good for many of the same types of textures.
//nil is returned if we failed to load. Allocator must be multithread safe if keep_allocator is true.
@(require_results)
texture2D_load_multi_from_file :: proc(paths : []string, desc : Texture_desc = {.clamp_to_edge, .linear, true, .RGBA8}, flipped := true, keep_allocator := false, loc := #caller_location) -> (textures : []Maybe(Texture2D)) {
	
	Load_png_info :: struct {
		//in
		filename : string,	//better name path
		flipped : bool,

		//out
		allocator : runtime.Allocator,
		img : ^image.Image,
		failed : bool,
	}
	
	run_load_from_disk :: proc (info : rawptr) {
		
		info : ^Load_png_info = auto_cast info;
		context.allocator = info.allocator;

		data, ok := os.read_entire_file_from_filename(info.filename);
		defer delete(data);
		if ok {
			using image;

			options := Options{
				.alpha_add_if_missing,
			};

			err: image.Error;
			info.img, err = png.load_from_bytes(data, options);

			if err != nil {
				info.failed = true;
				return;
			}

			switch info.img.depth {
				case 8:
					//do nothing
				case 16: //convert to 8 bit depth.
					current := bytes.buffer_to_bytes(&info.img.pixels);
					new := make([]u8, len(current) / 2);
					for &v, i in new {
						v = current[i*2];
					}
					bytes.buffer_destroy(&info.img.pixels);
					bytes.buffer_init(&info.img.pixels, new);
					info.img.depth = 8;
				case:
					panic("unimplemented");
			}

			fmt.assertf(info.img.depth == 8, "texture %v has depth %v, not 8", info.filename, info.img.depth);
			fmt.assertf(info.img.channels == 4, "texture %v does not have 4 channels", info.filename);

			if info.flipped {
				raw_data := bytes.buffer_to_bytes(&info.img.pixels);
				texture2D_flip(raw_data, info.img.width, info.img.height, info.img.channels);
			}
		}
		else {
			info.failed = true;
		}
	}

	textures = make([]Maybe(Texture2D), len(paths));

	data : []Load_png_info = make([]Load_png_info, len(paths));
	threads := make([]^thread.Thread, len(paths));
	defer delete(threads);
	defer delete(data);

	for to_load, i in paths {
		res : ^Load_png_info = &data[i];
		res.filename = to_load;
		res.flipped = flipped;
		res.allocator = context.allocator;
		threads[i] = thread.create_and_start_with_data(res, run_load_from_disk);
	}

	for t, i in threads {
		thread.join(t);
		
		info := data[i];

		if !info.failed {
			raw_data := bytes.buffer_to_bytes(&info.img.pixels);
			textures[i] = texture2D_make_desc(desc, info.img.width, info.img.height, .RGBA8, raw_data, loc);
			//cleanup
		}
		else {
			log.errorf("Could not load file : %v", info.filename);
			textures[i] = nil;
			//TODO is there any cleanup here?
		}
		
		image.destroy(info.img);
		thread.destroy(t);
	}

	return;
}

//Data is compressed bytes (ex png format)
@(require_results)
texture2D_load_from_png_bytes :: proc(desc : Texture_desc, data : []byte, texture_path := "", auto_convert_depth := true, flipped := true, loc := #caller_location) -> Texture2D {
	using image;

	options := Options{
		.alpha_add_if_missing,
	};

    img, err := png.load_from_bytes(data, options);
	defer image.destroy(img);
	
	if err != nil {
		fmt.panicf("Failed to load texture %s, err : %v", texture_path, err, loc = loc);
	}

	if auto_convert_depth {
		switch img.depth {
			case 8:
				//do nothing
			case 16: //convert to 8 bit depth.
				current := bytes.buffer_to_bytes(&img.pixels);
				new := make([]u8, len(current) / 2);
				for &v, i in new {
					v = current[i*2];
				}
				bytes.buffer_destroy(&img.pixels);
				bytes.buffer_init(&img.pixels, new);
				img.depth = 8;
			case:
				panic("unimplemented");
		}
	}

	fmt.assertf(img.depth == 8, "texture %v has depth %v, not 8", texture_path, img.depth, loc = loc);
	fmt.assertf(img.channels == 4, "texture %v does not have 4 channels", texture_path, loc = loc);
	
	raw_data := bytes.buffer_to_bytes(&img.pixels); //these are owned by the image and it will be deleted at the end of call.
	
	if flipped {
		texture2D_flip(raw_data, img.width, img.height, img.channels);
	}

	return texture2D_make_desc(desc, img.width, img.height, .RGBA8, raw_data, loc);
}

@(require_results)
texture2D_make :: proc(mipmaps : bool, wrapmode : Wrapmode, filtermode : Filtermode, internal_format : Pixel_format_internal,
							#any_int width, height : i32, upload_format : gl.Pixel_format_upload, data : []u8, loc := #caller_location) -> Texture2D {

	desc : Texture_desc = {
		mipmaps 		= mipmaps,
		wrapmode 		= wrapmode,
		filtermode 		= filtermode,
		format 			= internal_format,
	};

	return texture2D_make_desc(desc, width, height, upload_format, data, loc);
}

@(require_results)
texture2D_make_desc :: proc(using desc : Texture_desc, #any_int width, height : i32, upload_format : gl.Pixel_format_upload, data : []u8, loc := #caller_location) -> Texture2D {

	/*
	assert(wrapmode != nil, "wrapmode is nil", loc);
	assert(filtermode != nil, "filtermode is nil", loc);
	assert(format != nil, "format is nil", loc);
	*/
	
	//gl.PixelStorei(gl.UNPACK_ALIGNMENT, 1); //TODO

	id : gl.Tex2d_id = gl.gen_texture2D(loc);
	assert(id > 0, "TEXTURE: Failed to load texture", loc);

    gl.wrapmode_texture2D(id, desc.wrapmode);
	gl.filtermode_texture2D(id, desc.filtermode, mipmaps);

	size_per_component, channels : int;
	size_per_component = gl.upload_format_component_size(upload_format);
	channels = gl.upload_format_channel_cnt(upload_format);

	gl.setup_texure_2D(id, mipmaps, width, height, format);

	if len(data) == 0 {
		assert(raw_data(data) == nil, "Texture data is 0 len, but is not nil", loc);
	}
	else {
		assert(upload_format != .no_upload, "upload_format is no_upload, but there is data", loc);
		length := int(cast(int)width * cast(int)height * channels);
		fmt.assertf(len(data) == length, "Data is not in the correct format, len is %i, while it should have been %i", len(data), length, loc = loc);
		gl.write_texure_data_2D(id, 0, 0, 0, width, height, upload_format, data, loc);
	}
	
	if mipmaps && data != nil { //If there is no data, then it makes no sense to generate mipmaps
		gl.generate_mip_maps_2D(id);
	}

	tex : Texture2D = {
		id, 
		width,
		height,
		desc,
	}

    return tex;
}

texture2D_upload_data :: proc(tex : ^Texture2D, upload_format : gl.Pixel_format_upload, pixel_offset : [2]i32, pixel_cnt : [2]i32, data : []$T, loc := #caller_location) {
	
	gl.write_texure_data_2D(tex.id, 0, pixel_offset.x, pixel_offset.y, pixel_cnt.x, pixel_cnt.y, upload_format, data, loc);
	
	if (tex.mipmaps) {
		gl.generate_mip_maps_2D(tex.id);
	}
}

//TODO should this require the pointer?
texture2D_destroy :: proc(tex : Texture2D) {
	gl.delete_texture2D(tex.id);
}

texture2D_flip :: proc(data : []byte, width, height, channels : int) {
	
	line_size := width * channels;
	line_mem, err := mem.alloc_bytes(line_size, allocator = context.temp_allocator);

	assert(err == nil, "Failed to allocate");

	for h in 0..<height/2 {
		row_1 : = h;
		row_2 := (height-h-1);
		mem.copy(raw_data(line_mem), 				&data[h * line_size], 				line_size);	
		mem.copy(&data[h * line_size], 				&data[(height-h-1) * line_size], 	line_size);
		mem.copy(&data[(height-h-1) * line_size], 	raw_data(line_mem), 				line_size);
	}
}

texture2D_get_white :: proc() -> Texture2D {
	
	if state.white_texture.id == 0 {
		
		desc := Texture_desc{
			.repeat,
			.nearest,
			false,
			.RGBA8,
		};

		state.white_texture = texture2D_make_desc(desc, 1, 1, .RGBA8, {255, 255, 255, 255});
	}

	return state.white_texture;
}

texture2D_get_black :: proc () -> Texture2D {
	
	if state.black_texture.id == 0 {
		
		desc := Texture_desc{
			.repeat,
			.nearest,
			false,
			.RGBA8,
		};

		state.black_texture = texture2D_make_desc(desc, 1, 1, .RGBA8, {0, 0, 0, 255});
	}

	return state.black_texture;
}

/////////////////////////////////// Texture 3D ///////////////////////////////////
Texture3D :: struct {
	id			: gl.Tex3d_id,            	// OpenGL texture id
	width		: i32,               		// Texture base width
	height		: i32,               		// Texture base height
	depth 		: i32, 						// Texture base depth

	using desc : Texture_desc,
}

@(require_results)
texture3D_make :: proc(mipmaps : bool, wrapmode : Wrapmode, filtermode : Filtermode, internal_format : Pixel_format_internal,
							 width, height, depth : i32, upload_format : gl.Pixel_format_upload, data : []u8, loc := #caller_location) -> Texture3D {

	desc : Texture_desc = {
		mipmaps 		= mipmaps,
		wrapmode 		= wrapmode,
		filtermode 		= filtermode,
		format 			= internal_format,
	};

	return texture3D_make_desc(desc, width, height, depth, upload_format, data, loc);
}

@(require_results)
texture3D_make_desc :: proc(using desc : Texture_desc, width, height, depth : i32, upload_format : gl.Pixel_format_upload, data : []u8, loc := #caller_location) -> Texture3D {

	//gl.PixelStorei(gl.UNPACK_ALIGNMENT, 1); //TODO

	id : gl.Tex3d_id = gl.gen_texture3D(loc);
	assert(id > 0, "TEXTURE: Failed to load texture", loc);

    gl.wrapmode_texture3D(id, desc.wrapmode);
	gl.filtermode_texture3D(id, desc.filtermode, mipmaps);
	
	size_per_component, channels : int;
	size_per_component = gl.upload_format_component_size(upload_format);
	channels = gl.upload_format_channel_cnt(upload_format);

	gl.setup_texure_3D(id, mipmaps, width, height, depth, format);

	if len(data) == 0 {
		assert(raw_data(data) == nil, "Texture data is 0 len, but is not nil", loc);
	}
	else {
		assert(upload_format != .no_upload, "upload_format is no_upload, but there is data", loc);
		length := int(cast(int)width * channels);
		fmt.assertf(len(data) == length, "Data is not in the correct format, len is %i, while it should have been %i", len(data), length, loc = loc);
		gl.write_texure_data_3D(id, 0, 0, 0, 0, width, height, depth, upload_format, data, loc);
	}

	if mipmaps && data != nil { //If there is no data, then it makes no sense to generate mipmaps
		gl.generate_mip_maps_3D(id);
	}

	tex : Texture3D = {
		id, 
		width,
		height,
		depth,
		desc,
	}

    return tex;
}

texture3D_destroy :: proc(tex : ^Texture3D) {
	gl.delete_texture3D(tex.id);
	tex^ = {};
}

texture3D_upload_data :: proc(tex : ^Texture3D, pixel_offset : [3]i32, pixel_cnt : [3]i32, format : gl.Pixel_format_upload, data : []u8, loc := #caller_location) {
	
	gl.write_texure_data_3D(tex.id, 0, pixel_offset.x, pixel_offset.y, pixel_offset.z, pixel_cnt.x, pixel_cnt.y, pixel_cnt.z, format, data, loc);
	
	if (tex.mipmaps) {
		gl.generate_mip_maps_3D(tex.id);
	}
}

/////////////////////////////////// Texture 2D Atlas ///////////////////////////////////

//Refers to a quad in the atlas, there are returned from texture2D_atlas_upload
Atlas_handle :: utils.Atlas_handle;

@(private="file")
Texutre2D_atlas_data :: struct {
	backing : Texture2D,
	upload_format : gl.Pixel_format_upload,
	pixels : []u8, //TODO dont store pixels on the CPU, just do GPU (requires texture copy)
}

//Uses a modified strip packing, it is quite fast as it gets and the packing ratio is good for similar sized rectangles. 
//It might not work well for very large differences in quad sizes.
//Resize might not be fast.
Texture2D_atlas :: struct {
	impl : utils.Atlas,
	using data : ^Texutre2D_atlas_data,
}

texture2D_atlas_make :: proc (upload_format : gl.Pixel_format_upload, desc : Texture_desc = {.clamp_to_edge, .linear, false, .RGBA8},
								#any_int margin : i32 = 0, #any_int init_size : i32 = 128, loc := #caller_location) -> (atlas : Texture2D_atlas) {
	
	assert(upload_format != nil, "upload_format may not be nil", loc);
	//TODO remove assert(gl.upload_format_channel_cnt(upload_format) == 4, "upload_format channel count must be 4", loc);
	
	make_proc : utils.Atlas_make_from_proc : proc(atlas_src : rawptr, new_size : i32) -> rawptr {
		
		tex1 := cast(^Texutre2D_atlas_data)atlas_src;
		tex2 := new(Texutre2D_atlas_data);
		
		//We make a new texture from the descrition of the old.
		tex2.backing = texture2D_make_desc(tex1.backing.desc, new_size, new_size, tex1.upload_format, nil);
		tex2.pixels = make([]u8, new_size * new_size * cast(i32)gl.upload_format_channel_cnt(tex1.upload_format));
		tex2.upload_format = tex1.upload_format;
		
		return tex2;
	}
	
	swap_proc : utils.Atlas_swap_proc : proc(atlas_src : rawptr, atlas_dst : rawptr) {
		tex1 := cast(^Texutre2D_atlas_data)atlas_src;
		tex2 := cast(^Texutre2D_atlas_data)atlas_dst;
		tex1^, tex2^ = tex2^, tex1^;
	}
	
	upload_proc : utils.Atlas_upload_proc : proc(atlas : rawptr, quad : [4]i32, user_data : any) {
		
		tex := cast(^Texutre2D_atlas_data)atlas;
		
		if v, ok := user_data.([]u8); ok {
			fmt.assertf(tex.backing.width >= quad.x + quad.z, "Tex out of bounds, width : %v, quad x pos: %v, quad width : %v", tex.backing.width, quad.x, quad.z);
			fmt.assertf(tex.backing.height >= quad.y + quad.w, "Tex out of bounds, height : %v, quad y pos: %v, quad height : %v", tex.backing.height, quad.y, quad.w);
			texture2D_upload_data(&tex.backing, tex.upload_format, quad.xy, quad.zw, v);
			utils.copy_pixels(gl.upload_format_channel_cnt(tex.upload_format), quad.z, quad.w, 0, 0, v, tex.backing.width, tex.backing.height, quad.x, quad.y, tex.pixels, quad.z, quad.w);
		}
		else if user_data == nil {
			texture2D_upload_data(&tex.backing, tex.upload_format, quad.xy, quad.zw, tex.pixels);
		}
		else {
			fmt.panicf("Unhandled : %v", user_data);
		}
	}

	copy_proc : utils.Atlas_copy_proc : proc(atlas_src, atlas_dst : rawptr, src, dst, size : [2]i32) {
		 
		tex1 := cast(^Texutre2D_atlas_data)atlas_src;
		tex2 := cast(^Texutre2D_atlas_data)atlas_dst;

		assert(tex1.upload_format == tex2.upload_format);
		
		utils.copy_pixels(gl.upload_format_channel_cnt(tex1.upload_format), tex1.backing.width, tex1.backing.height, src.x, src.y, tex1.pixels,
							tex2.backing.width, tex2.backing.height, dst.x, dst.y, tex2.pixels, size.x, size.y);
		
		//TODO here we upload the entire texture, and that is slow.
		//texture2D_upload_data(&tex2.backing, tex2.upload_format, dst, size, tex2.pixels);
	}

	delete_proc : utils.Atlas_delete_proc : proc(atlas : rawptr) {
		assert(atlas != nil);
		tex := cast(^Texutre2D_atlas_data)atlas;
		texture2D_destroy(tex.backing);
		delete(tex.pixels);
		free(tex);
	}

	erase_proc : utils.Atlas_erase_proc : proc(atlas : rawptr, dst, size : [2]i32) {
		tex := cast(^Texutre2D_atlas_data)atlas;
		
		erase_data : []u8 = make([]u8, size.x * size.y * cast(i32)gl.upload_format_channel_cnt(tex.upload_format));
		defer delete(erase_data);

		for d, i in erase_data {
			erase_data[i] = 0;
		}
		
		texture2D_upload_data(&tex.backing, tex.upload_format, dst, size, erase_data);
	}

	data := new(Texutre2D_atlas_data);

	data.backing = texture2D_make_desc(desc, init_size, init_size, upload_format, nil, loc);
	data.upload_format = upload_format;
	data.pixels = make([]u8, init_size * init_size * cast(i32)gl.upload_format_channel_cnt(upload_format));
	
	atlas = Texture2D_atlas{
		data = data,
		impl = utils.atlas_make(10000, margin, init_size, data, make_proc, swap_proc, upload_proc, copy_proc, delete_proc, erase_proc, loc),
	}

	return;
}

//Uploads a texture into the atlas and returns a handle.
//Success may return false if the GPU texture size limit is reached.
//TODO this has a "bug", where it does not add a free_quad if the row is grown. This is because we do not remember the "source/refence" of the row and column.
@(require_results)
texture2D_atlas_upload :: proc (atlas : ^Texture2D_atlas, pixel_cnt : [2]i32, data : []u8, loc := #caller_location) -> (handle : Atlas_handle, success : bool) {
	fmt.assertf(cast(i32)len(data) == cast(i32)gl.upload_format_channel_cnt(atlas.upload_format) * pixel_cnt.x * pixel_cnt.y, "upload size must match, data len : %v, but size resulted in %v", len(data), pixel_cnt.x * pixel_cnt.y * 4, loc = loc);
	handle, success = utils.atlas_upload(&atlas.impl, pixel_cnt, data, loc);
	
	if !success { //if we fail, we prune and try again.
		utils.atlas_prune(&atlas.impl);
		handle, success = utils.atlas_upload(&atlas.impl, pixel_cnt, data, loc);
	}
	
	return handle, success;
}

//Returns the texture coordinates in (0,0) -> (1,1) coordinates. 
//The coordinates until the atlas is resized or destroy.
//resized refers to texture2D_atlas_shirnk, texture2D_atlas_grow and texture2D_atlas_upload.
@(require_results)
texture2D_atlas_get_coords :: proc (atlas : Texture2D_atlas, handle : Atlas_handle) -> [4]i32 {
	return utils.atlas_get_coords(atlas.impl, handle);
}

texture2D_atlas_remove :: proc(atlas : ^Texture2D_atlas, handle : Atlas_handle) {
	//TODO add a list of deleted quads and then try those before increasing the row widht/height.
	utils.atlas_remove(&atlas.impl, handle);
}

//Will double the size (in each dimension) of the atlas, the old rects will be repacked in a smart way to increase the packing ratio.
//Retruns false if the GPU texture size limit is reached.
texture2D_atlas_grow :: proc (atlas : ^Texture2D_atlas, loc := #caller_location) -> (success : bool) {
	return utils.atlas_grow(&atlas.impl);
}

//Will try and shrink the atlas to half the size, returns true if success, returns false if it could not shrink.
//To shrink as much as possiable do "for texture2D_atlas_shirnk(atlas) {};"
texture2D_atlas_shirnk :: proc (atlas : ^Texture2D_atlas) -> (success : bool) {
	return utils.atlas_shirnk(&atlas.impl);
}

texture2D_atlas_destroy :: proc (using atlas : Texture2D_atlas) {
	utils.atlas_destroy(atlas.impl);
	texture2D_destroy(atlas.backing);
}

/////////////////////////////////// Texture 2D Atlas Array ///////////////////////////////////

//Might do in the future

/////////////////////////////////// Texture 3D Atlas ///////////////////////////////////



/////////////////////////////////// Texture 2D multisampled ///////////////////////////////////



/////////////////////////////////// Texture arrays 2D ///////////////////////////////////



/////////////////////////////////// Texture cubemap 2D ///////////////////////////////////
//TODO use glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS)

