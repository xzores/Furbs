/*
 *  Copyright (c) 2005-2009 Jaroslav Gresula
 *
 *  Distributed under the MIT license (See accompanying file
 *  LICENSE.txt or copy at http://jagpdf.org/LICENSE.txt)
 */
#ifndef C_API_HEADER_H_JAGPDF__
#define C_API_HEADER_H_JAGPDF__
#include <jagpdf/detail/c_prologue.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* ==== enums ==== */
typedef enum {
  jag_CS_DEVICE_RGB=1,
  jag_CS_DEVICE_CMYK=2,
  jag_CS_DEVICE_GRAY=4,
  jag_CS_CIELAB=16,
  jag_CS_CALGRAY=32,
  jag_CS_CALRGB=64,
  jag_CS_INDEXED=128,
  jag_CS_ICCBASED=256
} jag_ColorSpaceType;

typedef enum {
  jag_IMAGE_FORMAT_AUTO=0,
  jag_IMAGE_FORMAT_NATIVE=1,
  jag_IMAGE_FORMAT_PNG=2,
  jag_IMAGE_FORMAT_JPEG=3
} jag_ImageFormat;

typedef enum {
  jag_LINE_CAP_BUTT=0,
  jag_LINE_CAP_ROUND=1,
  jag_LINE_CAP_SQUARE=2
} jag_LineCapStyle;

typedef enum {
  jag_LINE_JOIN_MITER=0,
  jag_LINE_JOIN_ROUND=1,
  jag_LINE_JOIN_BEVEL=2
} jag_LineJoinStyle;

typedef enum {
  jag_RI_ABSOLUTE_COLORIMETRIC=0,
  jag_RI_RELATIVE_COLORIMETRIC=1,
  jag_RI_SATURATION=2,
  jag_RI_PERCEPTUAL=3
} jag_RenderingIntentType;


/* ==== handles ==== */
JAG_GEN_UNIQUE_HANDLE(jag_Canvas);
JAG_GEN_UNIQUE_HANDLE(jag_Document);
JAG_GEN_UNIQUE_HANDLE(jag_DocumentOutline);
JAG_GEN_UNIQUE_HANDLE(jag_Font);
JAG_GEN_UNIQUE_HANDLE(jag_Image);
JAG_GEN_UNIQUE_HANDLE(jag_ImageDef);
JAG_GEN_UNIQUE_HANDLE(jag_ImageMask);
JAG_GEN_UNIQUE_HANDLE(jag_Page);
JAG_GEN_UNIQUE_HANDLE(jag_Profile);

/* ==== free functions ==== */
JAG_EXPORT jag_Document JAG_CALLSPEC jag_create_file(jag_Char const* file_path, jag_Profile profile);
JAG_EXPORT jag_Document JAG_CALLSPEC jag_create_stream(jag_streamout const* stream, jag_Profile profile);
JAG_EXPORT jag_Profile JAG_CALLSPEC jag_create_profile();
JAG_EXPORT jag_Profile JAG_CALLSPEC jag_create_profile_from_file(jag_Char const* fname);
JAG_EXPORT jag_Profile JAG_CALLSPEC jag_create_profile_from_string(jag_Char const* str);
JAG_EXPORT jag_UInt JAG_CALLSPEC jag_version();

/* ==== methods ===== */
JAG_EXPORT jag_Canvas JAG_CALLSPEC jag_Document_canvas_create(jag_Document hobj);
JAG_EXPORT jag_Canvas JAG_CALLSPEC jag_Page_canvas(jag_Page hobj);
JAG_EXPORT jag_Char const* JAG_CALLSPEC jag_Font_family_name(jag_Font hobj);
JAG_EXPORT jag_ColorSpace JAG_CALLSPEC jag_Document_color_space_load(jag_Document hobj, jag_Char const* spec);
JAG_EXPORT jag_Destination JAG_CALLSPEC jag_Document_destination_define(jag_Document hobj, jag_Char const* dest);
JAG_EXPORT jag_Destination JAG_CALLSPEC jag_Document_destination_reserve(jag_Document hobj);
JAG_EXPORT jag_DocumentOutline JAG_CALLSPEC jag_Document_outline(jag_Document hobj);

JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_advance(jag_Font hobj, jag_Char const* txt_u);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_ascender(jag_Font hobj);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_bbox_xmax(jag_Font hobj);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_bbox_xmin(jag_Font hobj);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_bbox_ymax(jag_Font hobj);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_bbox_ymin(jag_Font hobj);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_descender(jag_Font hobj);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_glyph_width(jag_Font hobj, jag_UInt16 glyph_index);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_height(jag_Font hobj);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Font_size(jag_Font hobj);

JAG_EXPORT jag_Double JAG_CALLSPEC jag_Image_dpi_x(jag_Image hobj);
JAG_EXPORT jag_Double JAG_CALLSPEC jag_Image_dpi_y(jag_Image hobj);

JAG_EXPORT jag_Font JAG_CALLSPEC jag_Document_font_load(jag_Document hobj, jag_Char const* fspec);
JAG_EXPORT jag_Function JAG_CALLSPEC jag_Document_function_2_load(jag_Document hobj, jag_Char const* fun);
JAG_EXPORT jag_Function JAG_CALLSPEC jag_Document_function_3_load(jag_Document hobj, jag_Char const* fun, jag_Function const* array_in, jag_UInt length);
JAG_EXPORT jag_Function JAG_CALLSPEC jag_Document_function_4_load(jag_Document hobj, jag_Char const* fun);
JAG_EXPORT jag_Image JAG_CALLSPEC jag_Document_image_load(jag_Document hobj, jag_ImageDef image);
JAG_EXPORT jag_Image JAG_CALLSPEC jag_Document_image_load_file(jag_Document hobj, jag_Char const* image_file_path, jag_ImageFormat image_format);
JAG_EXPORT jag_ImageDef JAG_CALLSPEC jag_Document_image_definition(jag_Document hobj);
JAG_EXPORT jag_ImageMask JAG_CALLSPEC jag_Document_define_image_mask(jag_Document hobj);
JAG_EXPORT jag_ImageMaskID JAG_CALLSPEC jag_Document_register_image_mask(jag_Document hobj, jag_ImageMask image_mask);
JAG_EXPORT jag_Int JAG_CALLSPEC jag_Document_page_number(jag_Document hobj);
JAG_EXPORT jag_Int JAG_CALLSPEC jag_Document_version(jag_Document hobj);
JAG_EXPORT jag_Int JAG_CALLSPEC jag_Font_is_bold(jag_Font hobj);
JAG_EXPORT jag_Int JAG_CALLSPEC jag_Font_is_italic(jag_Font hobj);
JAG_EXPORT jag_Page JAG_CALLSPEC jag_Document_page(jag_Document hobj);
JAG_EXPORT jag_Pattern JAG_CALLSPEC jag_Document_shading_pattern_load(jag_Document hobj, jag_Char const* pattern, jag_ColorSpace color_space, jag_Function func);
JAG_EXPORT jag_Pattern JAG_CALLSPEC jag_Document_shading_pattern_load_n(jag_Document hobj, jag_Char const* pattern, jag_ColorSpace cs, jag_Function const* array_in, jag_UInt length);
JAG_EXPORT jag_Pattern JAG_CALLSPEC jag_Document_tiling_pattern_load(jag_Document hobj, jag_Char const* pattern, jag_Canvas canvas);

JAG_EXPORT jag_UInt JAG_CALLSPEC jag_Image_bits_per_component(jag_Image hobj);
JAG_EXPORT jag_UInt JAG_CALLSPEC jag_Image_height(jag_Image hobj);
JAG_EXPORT jag_UInt JAG_CALLSPEC jag_Image_width(jag_Image hobj);

JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_alpha(jag_Canvas hobj, jag_Char const* op, jag_Double alpha);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_alpha_is_shape(jag_Canvas hobj, jag_Int bool_val);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_arc(jag_Canvas hobj, jag_Double cx, jag_Double cy, jag_Double rx, jag_Double ry, jag_Double start_angle, jag_Double sweep_angle);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_arc_to(jag_Canvas hobj, jag_Double x, jag_Double y, jag_Double rx, jag_Double ry, jag_Double angle, jag_Int large_arc_flag, jag_Int sweep_flag);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_bezier_to(jag_Canvas hobj, jag_Double x1, jag_Double y1, jag_Double x2, jag_Double y2, jag_Double x3, jag_Double y3);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_bezier_to_1st_ctrlpt(jag_Canvas hobj, jag_Double x1, jag_Double y1, jag_Double x3, jag_Double y3);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_bezier_to_2nd_ctrlpt(jag_Canvas hobj, jag_Double x2, jag_Double y2, jag_Double x3, jag_Double y3);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_circle(jag_Canvas hobj, jag_Double x, jag_Double y, jag_Double radius);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_color1(jag_Canvas hobj, jag_Char const* op, jag_Double ch1);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_color3(jag_Canvas hobj, jag_Char const* op, jag_Double ch1, jag_Double ch2, jag_Double ch3);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_color4(jag_Canvas hobj, jag_Char const* op, jag_Double ch1, jag_Double ch2, jag_Double ch3, jag_Double ch4);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_color_space(jag_Canvas hobj, jag_Char const* op, jag_ColorSpace cs);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_color_space_pattern(jag_Canvas hobj, jag_Char const* op);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_color_space_pattern_uncolored(jag_Canvas hobj, jag_Char const* op, jag_ColorSpace cs);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_image(jag_Canvas hobj, jag_Image img, jag_Double x, jag_Double y);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_line_cap(jag_Canvas hobj, jag_LineCapStyle style);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_line_dash(jag_Canvas hobj, jag_UInt const* array_in, jag_UInt length, jag_UInt phase);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_line_join(jag_Canvas hobj, jag_LineJoinStyle style);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_line_miter_limit(jag_Canvas hobj, jag_Double limit);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_line_to(jag_Canvas hobj, jag_Double x, jag_Double y);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_line_width(jag_Canvas hobj, jag_Double width);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_move_to(jag_Canvas hobj, jag_Double x, jag_Double y);

JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_path_close(jag_Canvas hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_path_paint(jag_Canvas hobj, jag_Char const* cmd);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_pattern(jag_Canvas hobj, jag_Char const* op, jag_Pattern patt);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_pattern1(jag_Canvas hobj, jag_Char const* op, jag_Pattern patt, jag_Double ch1);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_pattern3(jag_Canvas hobj, jag_Char const* op, jag_Pattern patt, jag_Double ch1, jag_Double ch2, jag_Double ch3);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_pattern4(jag_Canvas hobj, jag_Char const* op, jag_Pattern patt, jag_Double ch1, jag_Double ch2, jag_Double ch3, jag_Double ch4);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_rectangle(jag_Canvas hobj, jag_Double x, jag_Double y, jag_Double width, jag_Double height);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_rotate(jag_Canvas hobj, jag_Double alpha);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_scale(jag_Canvas hobj, jag_Double sx, jag_Double sy);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_scaled_image(jag_Canvas hobj, jag_Image image, jag_Double x, jag_Double y, jag_Double sx, jag_Double sy);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_shading_apply(jag_Canvas hobj, jag_Pattern pattern);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_skew(jag_Canvas hobj, jag_Double alpha, jag_Double beta);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_state_restore(jag_Canvas hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_state_save(jag_Canvas hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text(jag_Canvas hobj, jag_Char const* txt_u);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_character_spacing(jag_Canvas hobj, jag_Double spacing);

JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_end(jag_Canvas hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_font(jag_Canvas hobj, jag_Font font);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_glyphs(jag_Canvas hobj, jag_Double x, jag_Double y, jag_UInt16 const* array_in, jag_UInt length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_glyphs_o(jag_Canvas hobj, jag_Double x, jag_Double y, jag_UInt16 const* array_in, jag_UInt length, jag_Double const* offsets, jag_UInt offsets_length, jag_Int const* positions, jag_UInt positions_length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_horizontal_scaling(jag_Canvas hobj, jag_Double scaling);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_o(jag_Canvas hobj, jag_Char const* txt_u, jag_Double const* offsets, jag_UInt offsets_length, jag_Int const* positions, jag_UInt positions_length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_r(jag_Canvas hobj, jag_Char const* start, jag_Char const* end);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_rendering_mode(jag_Canvas hobj, jag_Char const* mode);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_rise(jag_Canvas hobj, jag_Double rise);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_ro(jag_Canvas hobj, jag_Char const* start, jag_Char const* end, jag_Double const* offsets, jag_UInt offsets_length, jag_Int const* positions, jag_UInt positions_length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_simple(jag_Canvas hobj, jag_Double x, jag_Double y, jag_Char const* txt_u);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_simple_o(jag_Canvas hobj, jag_Double x, jag_Double y, jag_Char const* txt_u, jag_Double const* offsets, jag_UInt offsets_length, jag_Int const* positions, jag_UInt positions_length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_simple_r(jag_Canvas hobj, jag_Double x, jag_Double y, jag_Char const* start, jag_Char const* end);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_simple_ro(jag_Canvas hobj, jag_Double x, jag_Double y, jag_Char const* start, jag_Char const* end, jag_Double const* offsets, jag_UInt offsets_length, jag_Int const* positions, jag_UInt positions_length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_start(jag_Canvas hobj, jag_Double x, jag_Double y);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_text_translate_line(jag_Canvas hobj, jag_Double tx, jag_Double ty);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_transform(jag_Canvas hobj, jag_Double a, jag_Double b, jag_Double c, jag_Double d, jag_Double e, jag_Double f);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Canvas_translate(jag_Canvas hobj, jag_Double tx, jag_Double ty);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_color(jag_DocumentOutline hobj, jag_Double red, jag_Double green, jag_Double blue);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_item(jag_DocumentOutline hobj, jag_Char const* title);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_item_destination(jag_DocumentOutline hobj, jag_Char const* title, jag_Char const* dest);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_item_destination_obj(jag_DocumentOutline hobj, jag_Char const* title, jag_Destination dest);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_level_down(jag_DocumentOutline hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_level_up(jag_DocumentOutline hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_state_restore(jag_DocumentOutline hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_state_save(jag_DocumentOutline hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_DocumentOutline_style(jag_DocumentOutline hobj, jag_Int val);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Document_destination_define_reserved(jag_Document hobj, jag_Destination id, jag_Char const* dest);

JAG_EXPORT jag_error JAG_CALLSPEC jag_Document_finalize(jag_Document hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Document_page_end(jag_Document hobj);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Document_page_start(jag_Document hobj, jag_Double width, jag_Double height);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_alternate_for_printing(jag_ImageDef hobj, jag_Image image);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_bits_per_component(jag_ImageDef hobj, jag_UInt bpc);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_color_key_mask(jag_ImageDef hobj, jag_UInt const* array_in, jag_UInt length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_color_space(jag_ImageDef hobj, jag_ColorSpace cs);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_data(jag_ImageDef hobj, jag_Byte const* array_in, jag_UInt length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_decode(jag_ImageDef hobj, jag_Double const* array_in, jag_UInt length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_dimensions(jag_ImageDef hobj, jag_UInt width, jag_UInt height);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_dpi(jag_ImageDef hobj, jag_Double xdpi, jag_Double ydpi);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_file_name(jag_ImageDef hobj, jag_Char const* file_name);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_format(jag_ImageDef hobj, jag_ImageFormat format);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_gamma(jag_ImageDef hobj, jag_Double val);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_image_mask(jag_ImageDef hobj, jag_ImageMaskID image_mask);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_interpolate(jag_ImageDef hobj, jag_Int flag);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageDef_rendering_intent(jag_ImageDef hobj, jag_RenderingIntentType intent);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageMask_bit_depth(jag_ImageMask hobj, jag_UInt bps);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageMask_data(jag_ImageMask hobj, jag_Byte const* array_in, jag_UInt length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageMask_decode(jag_ImageMask hobj, jag_Double lbound, jag_Double ubound);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageMask_dimensions(jag_ImageMask hobj, jag_UInt width, jag_UInt height);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageMask_file_name(jag_ImageMask hobj, jag_Char const* file_name);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageMask_interpolate(jag_ImageMask hobj, jag_Int val);
JAG_EXPORT jag_error JAG_CALLSPEC jag_ImageMask_matte(jag_ImageMask hobj, jag_Double const* array_in, jag_UInt length);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Page_annotation_goto(jag_Page hobj, jag_Double x, jag_Double y, jag_Double width, jag_Double height, jag_Char const* dest, jag_Char const* style);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Page_annotation_goto_obj(jag_Page hobj, jag_Double x, jag_Double y, jag_Double width, jag_Double height, jag_Destination dest, jag_Char const* style);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Page_annotation_uri(jag_Page hobj, jag_Double x, jag_Double y, jag_Double width, jag_Double height, jag_Char const* uri, jag_Char const* style);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Profile_save_to_file(jag_Profile hobj, jag_Char const* fname);
JAG_EXPORT jag_error JAG_CALLSPEC jag_Profile_set(jag_Profile hobj, jag_Char const* option, jag_Char const* value);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
