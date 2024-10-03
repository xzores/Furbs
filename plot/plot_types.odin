package plot;

import "../render"
import gui "../regui"
import "core:fmt"
import "core:math"
import "core:unicode/utf8"
import "core:slice"

Callout_line :: struct {
	length : f32,
	thickness : f32,
	//color : [4]f32,
	
	placement : f64,
	
	value : f64,
	display_value : bool,
}

plot_inner :: proc (plot_type : ^Plot_type, width_i, height_i : i32, allow_state_change : bool) -> (pv_pos, pv_size : [2]f32, x_view, y_view : [2]f64, x_callout, y_callout : []Callout_line) {
	
	//Calculate normalized space coordinates.
	width_f, height_f := cast(f32)width_i, cast(f32)height_i;
	aspect_ratio := width_f / height_f;
	width, height : f32 = aspect_ratio, 1.0;
	
	//Draw the polt to the plot_texture
	switch &p in plot_type {
		case Plot_xy:
			
			//handle export variables
			{
				//plot view
				pv_pos = {0.15, 0.10};
				size : [2]f32 = {0.78, 0.85};
				pv_size = {width - (1.0 - size.x), height - (1.0 - size.y)};
				
				//inner view
				x_view = p.x_view;
				y_view = p.y_view;
				
				//Callouts
				{
					x_callout_dyn := make([dynamic]Callout_line);
					y_callout_dyn := make([dynamic]Callout_line);
					
					grid_cnt := [2]f64{4.0 * cast(f64)width, 12 * cast(f64)height};
					
					//X-axis
					{
						x_total := p.x_view[1] - p.x_view[0];
						
						append(&x_callout_dyn, Callout_line{0.03, 0.003, 0, p.x_view[0], true});
						r := 0.1;
						base : f64 = nice_round(x_total/grid_cnt.x, r);
						for base == 0 && x_total != 0 {
							r *= 10;
							base = nice_round(x_total/grid_cnt.x, r);
						}
						x_cur : f64 = math.round(p.x_view[0] / base) * base - base;
						
						for i in -1..<grid_cnt.x+2 {
							x_cur += base;
							
							//Handle cases where 0 displays as 0.0001p because of floating point erros.
							if math.abs(x_cur) < 1.0 / (x_total * 1000) || x_cur == -0 {
								//x_cur = 0;
							}
							
							if x_cur < (p.x_view[0] + base / 2) {
								continue;
							}
							if x_cur > (p.x_view[1] - base / 2) {
								continue;
							}
							placement := (x_cur - p.x_view[0]) / x_total;
							append(&x_callout_dyn, Callout_line{0.03, 0.003, placement, x_cur, true});
						}
						append(&x_callout_dyn, Callout_line{0.03, 0.003, 1, p.x_view[1], true});
					}
					//Y-axis
					{
						y_total := p.y_view[1] - p.y_view[0];
						
						append(&y_callout_dyn, Callout_line{0.03, 0.003, 0, p.y_view[0], true});
						r := 0.1;
						base : f64 = nice_round(y_total/grid_cnt.y, r);
						for base == 0 && y_total != 0 {
							r *= 10;
							base = nice_round(y_total/grid_cnt.y, r);
						}
						y_cur : f64 = math.round(p.y_view[0] / base) * base - base;
						for i in -1..<grid_cnt.y+2 {
							y_cur += base;
							
							//Handle cases where 0 displays as 0.0001p because of floating point erros.
							if math.abs(y_cur) < 1.0 / (y_total * 1000) || y_cur == -0 {
								y_cur = 0;
							}
							
							if y_cur < (p.y_view[0] + base / 2) {
								continue;
							}
							if y_cur > (p.y_view[1] - base / 2) {
								continue;
							}
							placement := (y_cur - p.y_view[0]) / y_total;
							append(&y_callout_dyn, Callout_line{0.03, 0.003, placement, y_cur, true});
						}
						append(&y_callout_dyn, Callout_line{0.03, 0.003, 1, p.y_view[1], true});
					}
					
					x_callout = x_callout_dyn[:];
					y_callout = y_callout_dyn[:];
				}
				
				//TODO find a better spacing, max 6(+1) (so text can fit)
				//There is a max of 7 chars
			}
			
			//handle input and change state
			if allow_state_change {
				
				if render.button_down(.middel) {
					md := render.mouse_delta();
					
					total_x : f32 = cast(f32)(p.x_view[1] - p.x_view[0]);
					total_y : f32 = cast(f32)(p.y_view[1] - p.y_view[0]);
					p.x_view -= cast(f64)((total_x / pv_size.x) * (md.x / height_f));
					p.y_view += cast(f64)((total_y / pv_size.y) * (md.y / height_f));
				}
				
				{
					scroll_delta := render.scroll_delta();
					
					total_x : f64 = cast(f64)(p.x_view[1] - p.x_view[0]);
					total_y : f64 = cast(f64)(p.y_view[1] - p.y_view[0]);
					
					d : f64 = -0.02 * cast(f64)scroll_delta.y;
					
					if !render.is_key_down(.shift_left) {
						p.x_view = p.x_view + d * [2]f64{-total_x, total_x};
					}
					if !render.is_key_down(.control_left) {
						p.y_view = p.y_view + d * [2]f64{-total_y, total_y};
					}
				}
				
				if render.is_key_down(.r) {
					xlow, xhigh, ylow, yhigh : f64 = max(f64), min(f64), max(f64), min(f64);
					for trace in p.traces {
						xl, xh := get_extremes(trace.abscissa);
						yl, yh := get_extremes(trace.ordinate);
						xlow = 	math.min(xlow, xl);
						xhigh = math.max(xhigh, xh);
						ylow = 	math.min(ylow, yl);
						yhigh = math.max(yhigh, yh);
					}
					
					total_y := yhigh - ylow;
					
					p.x_view = {xlow, xhigh};
					p.y_view = {ylow - 0.1 * total_y, yhigh + 0.1 * total_y};
				}
			}
			
			//Plot the inner plot
			{
				render.set_texture(.texture_diffuse, render.texture2D_get_white());
				
				grid_line_width := p.grid_desc.line_width * min(width, height);
				
				for call in x_callout {
					x : f32 = cast(f32)call.placement * width;						
					render.draw_line_2D({x,0}, {x,height}, grid_line_width, 0, p.grid_desc.color);
				}
				for call in y_callout {
					y : f32 = cast(f32)call.placement * height;						
					render.draw_line_2D({0,y}, {width,y}, grid_line_width, 0, p.grid_desc.color);
				}
				
				line_width := 0.005 * min(width, height);
				
				for trace, it in p.traces {
					using trace;
					
					//TODO draw_quad_instanced();
					assert(len(abscissa) == len(ordinate), "The x and y does not have same length");
					for e, i in ordinate[:len(ordinate)-1] {
						x1 : f32 = cast(f32)((abscissa[i] - x_view[0]) 			/ (x_view[1] - x_view[0]));
						x2 : f32 = cast(f32)((abscissa[i+1] - x_view[0]) 			/ (x_view[1] - x_view[0]));
						y1 : f32 = cast(f32)((cast(f64)e - y_view[0]) 				/ (y_view[1] - y_view[0]));
						y2 : f32 = cast(f32)((cast(f64)ordinate[i+1] - y_view[0]) / (y_view[1] - y_view[0]));
						
						//x1, x2, y1, y2 = math.clamp(x1, 0, 1), math.clamp(x2, 0, 1), math.clamp(y1, 0, 1), math.clamp(y2, 0, 1);
						
						color, marker_style := get_trace_info(it);
						render.draw_line_2D({x1 * width, y1 * height}, {x2 * width, y2 * height}, line_width, 0, color);
						//fmt.printf("a : %v, b : %v\n", [2]f32{x1, y1}, [2]f32{x2, y2});
					}
				}
			}
			
			return;
	}
	
	unreachable();
}



