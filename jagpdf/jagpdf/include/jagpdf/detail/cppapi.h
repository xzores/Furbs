/*
 *  Copyright (c) 2005-2009 Jaroslav Gresula
 *
 *  Distributed under the MIT license (See accompanying file
 *  LICENSE.txt or copy at http://jagpdf.org/LICENSE.txt)
 */

#ifndef CPP_API_HEADER_JAGPDF_H__
#define CPP_API_HEADER_JAGPDF_H__
#include <jagpdf/detail/cpp_prologue.h>

namespace jag {
namespace pdf {

/* ==== enums ==== */
typedef enum {
  CS_DEVICE_RGB=1,
  CS_DEVICE_CMYK=2,
  CS_DEVICE_GRAY=4,
  CS_CIELAB=16,
  CS_CALGRAY=32,
  CS_CALRGB=64,
  CS_INDEXED=128,
  CS_ICCBASED=256
} ColorSpaceType;

typedef enum {
  IMAGE_FORMAT_AUTO=0,
  IMAGE_FORMAT_NATIVE=1,
  IMAGE_FORMAT_PNG=2,
  IMAGE_FORMAT_JPEG=3
} ImageFormat;

typedef enum {
  LINE_CAP_BUTT=0,
  LINE_CAP_ROUND=1,
  LINE_CAP_SQUARE=2
} LineCapStyle;

typedef enum {
  LINE_JOIN_MITER=0,
  LINE_JOIN_ROUND=1,
  LINE_JOIN_BEVEL=2
} LineJoinStyle;

typedef enum {
  RI_ABSOLUTE_COLORIMETRIC=0,
  RI_RELATIVE_COLORIMETRIC=1,
  RI_SATURATION=2,
  RI_PERCEPTUAL=3
} RenderingIntentType;



/* ==== classes ==== */

class Font
{
    jag_Font m_obj;

public:
    Char const* family_name() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Char const* result_ = jag_Font_family_name(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_family_name(m_obj);
#endif
    }

    Double advance(Char const* txt_u) const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_advance(m_obj, txt_u);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_advance(m_obj, txt_u);
#endif
    }

    Double ascender() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_ascender(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_ascender(m_obj);
#endif
    }

    Double bbox_xmax() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_bbox_xmax(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_bbox_xmax(m_obj);
#endif
    }

    Double bbox_xmin() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_bbox_xmin(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_bbox_xmin(m_obj);
#endif
    }

    Double bbox_ymax() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_bbox_ymax(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_bbox_ymax(m_obj);
#endif
    }

    Double bbox_ymin() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_bbox_ymin(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_bbox_ymin(m_obj);
#endif
    }

    Double descender() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_descender(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_descender(m_obj);
#endif
    }

    Double glyph_width(UInt16 glyph_index) const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_glyph_width(m_obj, glyph_index);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_glyph_width(m_obj, glyph_index);
#endif
    }

    Double height() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_height(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_height(m_obj);
#endif
    }

    Double size() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Font_size(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_size(m_obj);
#endif
    }

    Int is_bold() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Int result_ = jag_Font_is_bold(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_is_bold(m_obj);
#endif
    }

    Int is_italic() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Int result_ = jag_Font_is_italic(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Font_is_italic(m_obj);
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(Font***)
    {
    }

    typedef void (*unspecified_bool_type)(Font***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_Font Font::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &Font::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~Font() {
    }

    Font(Font const& other) {
        m_obj = other.m_obj;
    }

    Font& operator=(Font const& other) {
        m_obj = other.m_obj;
        return *this;
    }



    Font()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit Font(jag_Font obj)
        : m_obj(obj)
    {}

    jag_Font handle_() {
        return m_obj;
    }
};

class Image
{
    jag_Image m_obj;

public:
    Double dpi_x() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Image_dpi_x(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Image_dpi_x(m_obj);
#endif
    }

    Double dpi_y() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Double result_ = jag_Image_dpi_y(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Image_dpi_y(m_obj);
#endif
    }

    UInt bits_per_component() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        UInt result_ = jag_Image_bits_per_component(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Image_bits_per_component(m_obj);
#endif
    }

    UInt height() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        UInt result_ = jag_Image_height(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Image_height(m_obj);
#endif
    }

    UInt width() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        UInt result_ = jag_Image_width(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Image_width(m_obj);
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(Image***)
    {
    }

    typedef void (*unspecified_bool_type)(Image***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_Image Image::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &Image::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~Image() {
    }

    Image(Image const& other) {
        m_obj = other.m_obj;
    }

    Image& operator=(Image const& other) {
        m_obj = other.m_obj;
        return *this;
    }



    Image()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit Image(jag_Image obj)
        : m_obj(obj)
    {}

    jag_Image handle_() {
        return m_obj;
    }
};

class Canvas
{
    jag_Canvas m_obj;

public:
    Result alpha(Char const* op, Double alpha)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_alpha(m_obj, op, alpha))
            throw Exception();
#else
        return jag_Canvas_alpha(m_obj, op, alpha);
#endif
    }

    Result alpha_is_shape(Int bool_val)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_alpha_is_shape(m_obj, bool_val))
            throw Exception();
#else
        return jag_Canvas_alpha_is_shape(m_obj, bool_val);
#endif
    }

    Result arc(Double cx, Double cy, Double rx, Double ry, Double start_angle, Double sweep_angle)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_arc(m_obj, cx, cy, rx, ry, start_angle, sweep_angle))
            throw Exception();
#else
        return jag_Canvas_arc(m_obj, cx, cy, rx, ry, start_angle, sweep_angle);
#endif
    }

    Result arc_to(Double x, Double y, Double rx, Double ry, Double angle, Int large_arc_flag, Int sweep_flag)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_arc_to(m_obj, x, y, rx, ry, angle, large_arc_flag, sweep_flag))
            throw Exception();
#else
        return jag_Canvas_arc_to(m_obj, x, y, rx, ry, angle, large_arc_flag, sweep_flag);
#endif
    }

    Result bezier_to(Double x1, Double y1, Double x2, Double y2, Double x3, Double y3)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_bezier_to(m_obj, x1, y1, x2, y2, x3, y3))
            throw Exception();
#else
        return jag_Canvas_bezier_to(m_obj, x1, y1, x2, y2, x3, y3);
#endif
    }

    Result bezier_to_1st_ctrlpt(Double x1, Double y1, Double x3, Double y3)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_bezier_to_1st_ctrlpt(m_obj, x1, y1, x3, y3))
            throw Exception();
#else
        return jag_Canvas_bezier_to_1st_ctrlpt(m_obj, x1, y1, x3, y3);
#endif
    }

    Result bezier_to_2nd_ctrlpt(Double x2, Double y2, Double x3, Double y3)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_bezier_to_2nd_ctrlpt(m_obj, x2, y2, x3, y3))
            throw Exception();
#else
        return jag_Canvas_bezier_to_2nd_ctrlpt(m_obj, x2, y2, x3, y3);
#endif
    }

    Result circle(Double x, Double y, Double radius)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_circle(m_obj, x, y, radius))
            throw Exception();
#else
        return jag_Canvas_circle(m_obj, x, y, radius);
#endif
    }

    Result color(Char const* op, Double ch1)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_color1(m_obj, op, ch1))
            throw Exception();
#else
        return jag_Canvas_color1(m_obj, op, ch1);
#endif
    }

    Result color(Char const* op, Double ch1, Double ch2, Double ch3)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_color3(m_obj, op, ch1, ch2, ch3))
            throw Exception();
#else
        return jag_Canvas_color3(m_obj, op, ch1, ch2, ch3);
#endif
    }

    Result color(Char const* op, Double ch1, Double ch2, Double ch3, Double ch4)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_color4(m_obj, op, ch1, ch2, ch3, ch4))
            throw Exception();
#else
        return jag_Canvas_color4(m_obj, op, ch1, ch2, ch3, ch4);
#endif
    }

    Result color_space(Char const* op, ColorSpace cs)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_color_space(m_obj, op, cs))
            throw Exception();
#else
        return jag_Canvas_color_space(m_obj, op, cs);
#endif
    }

    Result color_space_pattern(Char const* op)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_color_space_pattern(m_obj, op))
            throw Exception();
#else
        return jag_Canvas_color_space_pattern(m_obj, op);
#endif
    }

    Result color_space_pattern_uncolored(Char const* op, ColorSpace cs)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_color_space_pattern_uncolored(m_obj, op, cs))
            throw Exception();
#else
        return jag_Canvas_color_space_pattern_uncolored(m_obj, op, cs);
#endif
    }

    Result image(Image img, Double x, Double y)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_image(m_obj, img.handle_(), x, y))
            throw Exception();
#else
        return jag_Canvas_image(m_obj, img.handle_(), x, y);
#endif
    }

    Result line_cap(LineCapStyle style)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_line_cap(m_obj, static_cast<jag_LineCapStyle>(style)))
            throw Exception();
#else
        return jag_Canvas_line_cap(m_obj, static_cast<jag_LineCapStyle>(style));
#endif
    }

    Result line_dash(UInt const* array_in, UInt length, UInt phase)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_line_dash(m_obj, array_in, length, phase))
            throw Exception();
#else
        return jag_Canvas_line_dash(m_obj, array_in, length, phase);
#endif
    }

    Result line_join(LineJoinStyle style)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_line_join(m_obj, static_cast<jag_LineJoinStyle>(style)))
            throw Exception();
#else
        return jag_Canvas_line_join(m_obj, static_cast<jag_LineJoinStyle>(style));
#endif
    }

    Result line_miter_limit(Double limit)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_line_miter_limit(m_obj, limit))
            throw Exception();
#else
        return jag_Canvas_line_miter_limit(m_obj, limit);
#endif
    }

    Result line_to(Double x, Double y)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_line_to(m_obj, x, y))
            throw Exception();
#else
        return jag_Canvas_line_to(m_obj, x, y);
#endif
    }

    Result line_width(Double width)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_line_width(m_obj, width))
            throw Exception();
#else
        return jag_Canvas_line_width(m_obj, width);
#endif
    }

    Result move_to(Double x, Double y)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_move_to(m_obj, x, y))
            throw Exception();
#else
        return jag_Canvas_move_to(m_obj, x, y);
#endif
    }

    Result path_close()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_path_close(m_obj))
            throw Exception();
#else
        return jag_Canvas_path_close(m_obj);
#endif
    }

    Result path_paint(Char const* cmd)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_path_paint(m_obj, cmd))
            throw Exception();
#else
        return jag_Canvas_path_paint(m_obj, cmd);
#endif
    }

    Result pattern(Char const* op, Pattern patt)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_pattern(m_obj, op, patt))
            throw Exception();
#else
        return jag_Canvas_pattern(m_obj, op, patt);
#endif
    }

    Result pattern(Char const* op, Pattern patt, Double ch1)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_pattern1(m_obj, op, patt, ch1))
            throw Exception();
#else
        return jag_Canvas_pattern1(m_obj, op, patt, ch1);
#endif
    }

    Result pattern(Char const* op, Pattern patt, Double ch1, Double ch2, Double ch3)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_pattern3(m_obj, op, patt, ch1, ch2, ch3))
            throw Exception();
#else
        return jag_Canvas_pattern3(m_obj, op, patt, ch1, ch2, ch3);
#endif
    }

    Result pattern(Char const* op, Pattern patt, Double ch1, Double ch2, Double ch3, Double ch4)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_pattern4(m_obj, op, patt, ch1, ch2, ch3, ch4))
            throw Exception();
#else
        return jag_Canvas_pattern4(m_obj, op, patt, ch1, ch2, ch3, ch4);
#endif
    }

    Result rectangle(Double x, Double y, Double width, Double height)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_rectangle(m_obj, x, y, width, height))
            throw Exception();
#else
        return jag_Canvas_rectangle(m_obj, x, y, width, height);
#endif
    }

    Result rotate(Double alpha)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_rotate(m_obj, alpha))
            throw Exception();
#else
        return jag_Canvas_rotate(m_obj, alpha);
#endif
    }

    Result scale(Double sx, Double sy)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_scale(m_obj, sx, sy))
            throw Exception();
#else
        return jag_Canvas_scale(m_obj, sx, sy);
#endif
    }

    Result scaled_image(Image image, Double x, Double y, Double sx, Double sy)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_scaled_image(m_obj, image.handle_(), x, y, sx, sy))
            throw Exception();
#else
        return jag_Canvas_scaled_image(m_obj, image.handle_(), x, y, sx, sy);
#endif
    }

    Result shading_apply(Pattern pattern)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_shading_apply(m_obj, pattern))
            throw Exception();
#else
        return jag_Canvas_shading_apply(m_obj, pattern);
#endif
    }

    Result skew(Double alpha, Double beta)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_skew(m_obj, alpha, beta))
            throw Exception();
#else
        return jag_Canvas_skew(m_obj, alpha, beta);
#endif
    }

    Result state_restore()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_state_restore(m_obj))
            throw Exception();
#else
        return jag_Canvas_state_restore(m_obj);
#endif
    }

    Result state_save()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_state_save(m_obj))
            throw Exception();
#else
        return jag_Canvas_state_save(m_obj);
#endif
    }

    Result text(Char const* start, Char const* end)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_r(m_obj, start, end))
            throw Exception();
#else
        return jag_Canvas_text_r(m_obj, start, end);
#endif
    }

    Result text(Char const* start, Char const* end, Double const* offsets, UInt offsets_length, Int const* positions, UInt positions_length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_ro(m_obj, start, end, offsets, offsets_length, positions, positions_length))
            throw Exception();
#else
        return jag_Canvas_text_ro(m_obj, start, end, offsets, offsets_length, positions, positions_length);
#endif
    }

    Result text(Char const* txt_u)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text(m_obj, txt_u))
            throw Exception();
#else
        return jag_Canvas_text(m_obj, txt_u);
#endif
    }

    Result text(Char const* txt_u, Double const* offsets, UInt offsets_length, Int const* positions, UInt positions_length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_o(m_obj, txt_u, offsets, offsets_length, positions, positions_length))
            throw Exception();
#else
        return jag_Canvas_text_o(m_obj, txt_u, offsets, offsets_length, positions, positions_length);
#endif
    }

    Result text(Double x, Double y, Char const* start, Char const* end)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_simple_r(m_obj, x, y, start, end))
            throw Exception();
#else
        return jag_Canvas_text_simple_r(m_obj, x, y, start, end);
#endif
    }

    Result text(Double x, Double y, Char const* start, Char const* end, Double const* offsets, UInt offsets_length, Int const* positions, UInt positions_length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_simple_ro(m_obj, x, y, start, end, offsets, offsets_length, positions, positions_length))
            throw Exception();
#else
        return jag_Canvas_text_simple_ro(m_obj, x, y, start, end, offsets, offsets_length, positions, positions_length);
#endif
    }

    Result text(Double x, Double y, Char const* txt_u)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_simple(m_obj, x, y, txt_u))
            throw Exception();
#else
        return jag_Canvas_text_simple(m_obj, x, y, txt_u);
#endif
    }

    Result text(Double x, Double y, Char const* txt_u, Double const* offsets, UInt offsets_length, Int const* positions, UInt positions_length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_simple_o(m_obj, x, y, txt_u, offsets, offsets_length, positions, positions_length))
            throw Exception();
#else
        return jag_Canvas_text_simple_o(m_obj, x, y, txt_u, offsets, offsets_length, positions, positions_length);
#endif
    }

    Result text_character_spacing(Double spacing)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_character_spacing(m_obj, spacing))
            throw Exception();
#else
        return jag_Canvas_text_character_spacing(m_obj, spacing);
#endif
    }

    Result text_end()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_end(m_obj))
            throw Exception();
#else
        return jag_Canvas_text_end(m_obj);
#endif
    }

    Result text_font(Font font)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_font(m_obj, font.handle_()))
            throw Exception();
#else
        return jag_Canvas_text_font(m_obj, font.handle_());
#endif
    }

    Result text_glyphs(Double x, Double y, UInt16 const* array_in, UInt length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_glyphs(m_obj, x, y, array_in, length))
            throw Exception();
#else
        return jag_Canvas_text_glyphs(m_obj, x, y, array_in, length);
#endif
    }

    Result text_glyphs(Double x, Double y, UInt16 const* array_in, UInt length, Double const* offsets, UInt offsets_length, Int const* positions, UInt positions_length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_glyphs_o(m_obj, x, y, array_in, length, offsets, offsets_length, positions, positions_length))
            throw Exception();
#else
        return jag_Canvas_text_glyphs_o(m_obj, x, y, array_in, length, offsets, offsets_length, positions, positions_length);
#endif
    }

    Result text_horizontal_scaling(Double scaling)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_horizontal_scaling(m_obj, scaling))
            throw Exception();
#else
        return jag_Canvas_text_horizontal_scaling(m_obj, scaling);
#endif
    }

    Result text_rendering_mode(Char const* mode)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_rendering_mode(m_obj, mode))
            throw Exception();
#else
        return jag_Canvas_text_rendering_mode(m_obj, mode);
#endif
    }

    Result text_rise(Double rise)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_rise(m_obj, rise))
            throw Exception();
#else
        return jag_Canvas_text_rise(m_obj, rise);
#endif
    }

    Result text_start(Double x, Double y)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_start(m_obj, x, y))
            throw Exception();
#else
        return jag_Canvas_text_start(m_obj, x, y);
#endif
    }

    Result text_translate_line(Double tx, Double ty)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_text_translate_line(m_obj, tx, ty))
            throw Exception();
#else
        return jag_Canvas_text_translate_line(m_obj, tx, ty);
#endif
    }

    Result transform(Double a, Double b, Double c, Double d, Double e, Double f)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_transform(m_obj, a, b, c, d, e, f))
            throw Exception();
#else
        return jag_Canvas_transform(m_obj, a, b, c, d, e, f);
#endif
    }

    Result translate(Double tx, Double ty)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Canvas_translate(m_obj, tx, ty))
            throw Exception();
#else
        return jag_Canvas_translate(m_obj, tx, ty);
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(Canvas***)
    {
    }

    typedef void (*unspecified_bool_type)(Canvas***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_Canvas Canvas::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &Canvas::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~Canvas() {
    }

    Canvas(Canvas const& other) {
        m_obj = other.m_obj;
    }

    Canvas& operator=(Canvas const& other) {
        m_obj = other.m_obj;
        return *this;
    }



    Canvas()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit Canvas(jag_Canvas obj)
        : m_obj(obj)
    {}

    jag_Canvas handle_() {
        return m_obj;
    }
};

class ImageDef
{
    jag_ImageDef m_obj;

public:
    Result alternate_for_printing(Image image)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_alternate_for_printing(m_obj, image.handle_()))
            throw Exception();
#else
        return jag_ImageDef_alternate_for_printing(m_obj, image.handle_());
#endif
    }

    Result bits_per_component(UInt bpc)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_bits_per_component(m_obj, bpc))
            throw Exception();
#else
        return jag_ImageDef_bits_per_component(m_obj, bpc);
#endif
    }

    Result color_key_mask(UInt const* array_in, UInt length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_color_key_mask(m_obj, array_in, length))
            throw Exception();
#else
        return jag_ImageDef_color_key_mask(m_obj, array_in, length);
#endif
    }

    Result color_space(ColorSpace cs)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_color_space(m_obj, cs))
            throw Exception();
#else
        return jag_ImageDef_color_space(m_obj, cs);
#endif
    }

    Result data(Byte const* array_in, UInt length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_data(m_obj, array_in, length))
            throw Exception();
#else
        return jag_ImageDef_data(m_obj, array_in, length);
#endif
    }

    Result decode(Double const* array_in, UInt length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_decode(m_obj, array_in, length))
            throw Exception();
#else
        return jag_ImageDef_decode(m_obj, array_in, length);
#endif
    }

    Result dimensions(UInt width, UInt height)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_dimensions(m_obj, width, height))
            throw Exception();
#else
        return jag_ImageDef_dimensions(m_obj, width, height);
#endif
    }

    Result dpi(Double xdpi, Double ydpi)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_dpi(m_obj, xdpi, ydpi))
            throw Exception();
#else
        return jag_ImageDef_dpi(m_obj, xdpi, ydpi);
#endif
    }

    Result file_name(Char const* file_name)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_file_name(m_obj, file_name))
            throw Exception();
#else
        return jag_ImageDef_file_name(m_obj, file_name);
#endif
    }

    Result format(ImageFormat format)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_format(m_obj, static_cast<jag_ImageFormat>(format)))
            throw Exception();
#else
        return jag_ImageDef_format(m_obj, static_cast<jag_ImageFormat>(format));
#endif
    }

    Result gamma(Double val)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_gamma(m_obj, val))
            throw Exception();
#else
        return jag_ImageDef_gamma(m_obj, val);
#endif
    }

    Result image_mask(ImageMaskID image_mask)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_image_mask(m_obj, image_mask))
            throw Exception();
#else
        return jag_ImageDef_image_mask(m_obj, image_mask);
#endif
    }

    Result interpolate(Int flag)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_interpolate(m_obj, flag))
            throw Exception();
#else
        return jag_ImageDef_interpolate(m_obj, flag);
#endif
    }

    Result rendering_intent(RenderingIntentType intent)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageDef_rendering_intent(m_obj, static_cast<jag_RenderingIntentType>(intent)))
            throw Exception();
#else
        return jag_ImageDef_rendering_intent(m_obj, static_cast<jag_RenderingIntentType>(intent));
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(ImageDef***)
    {
    }

    typedef void (*unspecified_bool_type)(ImageDef***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_ImageDef ImageDef::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &ImageDef::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~ImageDef() {
    }

    ImageDef(ImageDef const& other) {
        m_obj = other.m_obj;
    }

    ImageDef& operator=(ImageDef const& other) {
        m_obj = other.m_obj;
        return *this;
    }



    ImageDef()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit ImageDef(jag_ImageDef obj)
        : m_obj(obj)
    {}

    jag_ImageDef handle_() {
        return m_obj;
    }
};

class DocumentOutline
{
    jag_DocumentOutline m_obj;

public:
    Result color(Double red, Double green, Double blue)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_color(m_obj, red, green, blue))
            throw Exception();
#else
        return jag_DocumentOutline_color(m_obj, red, green, blue);
#endif
    }

    Result item(Char const* title)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_item(m_obj, title))
            throw Exception();
#else
        return jag_DocumentOutline_item(m_obj, title);
#endif
    }

    Result item(Char const* title, Char const* dest)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_item_destination(m_obj, title, dest))
            throw Exception();
#else
        return jag_DocumentOutline_item_destination(m_obj, title, dest);
#endif
    }

    Result item(Char const* title, Destination dest)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_item_destination_obj(m_obj, title, dest))
            throw Exception();
#else
        return jag_DocumentOutline_item_destination_obj(m_obj, title, dest);
#endif
    }

    Result level_down()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_level_down(m_obj))
            throw Exception();
#else
        return jag_DocumentOutline_level_down(m_obj);
#endif
    }

    Result level_up()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_level_up(m_obj))
            throw Exception();
#else
        return jag_DocumentOutline_level_up(m_obj);
#endif
    }

    Result state_restore()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_state_restore(m_obj))
            throw Exception();
#else
        return jag_DocumentOutline_state_restore(m_obj);
#endif
    }

    Result state_save()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_state_save(m_obj))
            throw Exception();
#else
        return jag_DocumentOutline_state_save(m_obj);
#endif
    }

    Result style(Int val)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_DocumentOutline_style(m_obj, val))
            throw Exception();
#else
        return jag_DocumentOutline_style(m_obj, val);
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(DocumentOutline***)
    {
    }

    typedef void (*unspecified_bool_type)(DocumentOutline***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_DocumentOutline DocumentOutline::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &DocumentOutline::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~DocumentOutline() {
    }

    DocumentOutline(DocumentOutline const& other) {
        m_obj = other.m_obj;
    }

    DocumentOutline& operator=(DocumentOutline const& other) {
        m_obj = other.m_obj;
        return *this;
    }



    DocumentOutline()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit DocumentOutline(jag_DocumentOutline obj)
        : m_obj(obj)
    {}

    jag_DocumentOutline handle_() {
        return m_obj;
    }
};

class ImageMask
{
    jag_ImageMask m_obj;

public:
    Result bit_depth(UInt bps)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageMask_bit_depth(m_obj, bps))
            throw Exception();
#else
        return jag_ImageMask_bit_depth(m_obj, bps);
#endif
    }

    Result data(Byte const* array_in, UInt length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageMask_data(m_obj, array_in, length))
            throw Exception();
#else
        return jag_ImageMask_data(m_obj, array_in, length);
#endif
    }

    Result decode(Double lbound, Double ubound)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageMask_decode(m_obj, lbound, ubound))
            throw Exception();
#else
        return jag_ImageMask_decode(m_obj, lbound, ubound);
#endif
    }

    Result dimensions(UInt width, UInt height)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageMask_dimensions(m_obj, width, height))
            throw Exception();
#else
        return jag_ImageMask_dimensions(m_obj, width, height);
#endif
    }

    Result file_name(Char const* file_name)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageMask_file_name(m_obj, file_name))
            throw Exception();
#else
        return jag_ImageMask_file_name(m_obj, file_name);
#endif
    }

    Result interpolate(Int val)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageMask_interpolate(m_obj, val))
            throw Exception();
#else
        return jag_ImageMask_interpolate(m_obj, val);
#endif
    }

    Result matte(Double const* array_in, UInt length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_ImageMask_matte(m_obj, array_in, length))
            throw Exception();
#else
        return jag_ImageMask_matte(m_obj, array_in, length);
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(ImageMask***)
    {
    }

    typedef void (*unspecified_bool_type)(ImageMask***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_ImageMask ImageMask::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &ImageMask::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~ImageMask() {
        jag_release(m_obj);
    }

    ImageMask(ImageMask const& other) {
        m_obj = other.m_obj;
        jag_addref(m_obj);
    }

    ImageMask& operator=(ImageMask const& other) {
        jag_release(m_obj);
        m_obj = other.m_obj;
        jag_addref(m_obj);
        return *this;
    }


    ImageMask()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit ImageMask(jag_ImageMask obj)
        : m_obj(obj)
    {}

    jag_ImageMask handle_() {
        return m_obj;
    }
};

class Page
{
    jag_Page m_obj;

public:
    Canvas canvas()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_Canvas obj_ = jag_Page_canvas(m_obj);
        if (!obj_)
            throw Exception();
        return Canvas(obj_);
#else
        return Canvas(jag_Page_canvas(m_obj));
#endif
    }

    Result annotation_goto(Double x, Double y, Double width, Double height, Char const* dest, Char const* style=0)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Page_annotation_goto(m_obj, x, y, width, height, dest, style))
            throw Exception();
#else
        return jag_Page_annotation_goto(m_obj, x, y, width, height, dest, style);
#endif
    }

    Result annotation_goto(Double x, Double y, Double width, Double height, Destination dest, Char const* style=0)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Page_annotation_goto_obj(m_obj, x, y, width, height, dest, style))
            throw Exception();
#else
        return jag_Page_annotation_goto_obj(m_obj, x, y, width, height, dest, style);
#endif
    }

    Result annotation_uri(Double x, Double y, Double width, Double height, Char const* uri, Char const* style=0)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Page_annotation_uri(m_obj, x, y, width, height, uri, style))
            throw Exception();
#else
        return jag_Page_annotation_uri(m_obj, x, y, width, height, uri, style);
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(Page***)
    {
    }

    typedef void (*unspecified_bool_type)(Page***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_Page Page::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &Page::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~Page() {
    }

    Page(Page const& other) {
        m_obj = other.m_obj;
    }

    Page& operator=(Page const& other) {
        m_obj = other.m_obj;
        return *this;
    }



    Page()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit Page(jag_Page obj)
        : m_obj(obj)
    {}

    jag_Page handle_() {
        return m_obj;
    }
};

class Profile
{
    jag_Profile m_obj;

public:
    Result save_to_file(Char const* fname) const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Profile_save_to_file(m_obj, fname))
            throw Exception();
#else
        return jag_Profile_save_to_file(m_obj, fname);
#endif
    }

    Result set(Char const* option, Char const* value)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Profile_set(m_obj, option, value))
            throw Exception();
#else
        return jag_Profile_set(m_obj, option, value);
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(Profile***)
    {
    }

    typedef void (*unspecified_bool_type)(Profile***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_Profile Profile::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &Profile::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~Profile() {
        jag_release(m_obj);
    }

    Profile(Profile const& other) {
        m_obj = other.m_obj;
        jag_addref(m_obj);
    }

    Profile& operator=(Profile const& other) {
        jag_release(m_obj);
        m_obj = other.m_obj;
        jag_addref(m_obj);
        return *this;
    }


    Profile()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit Profile(jag_Profile obj)
        : m_obj(obj)
    {}

    jag_Profile handle_() {
        return m_obj;
    }
};

class Document
{
    jag_Document m_obj;

public:
    Canvas canvas_create() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_Canvas obj_ = jag_Document_canvas_create(m_obj);
        if (!obj_)
            throw Exception();
        return Canvas(obj_);
#else
        return Canvas(jag_Document_canvas_create(m_obj));
#endif
    }

    ColorSpace color_space_load(Char const* spec)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        ColorSpace result_ = jag_Document_color_space_load(m_obj, spec);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_color_space_load(m_obj, spec);
#endif
    }

    Destination destination_define(Char const* dest)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Destination result_ = jag_Document_destination_define(m_obj, dest);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_destination_define(m_obj, dest);
#endif
    }

    Destination destination_reserve()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Destination result_ = jag_Document_destination_reserve(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_destination_reserve(m_obj);
#endif
    }

    DocumentOutline outline()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_DocumentOutline obj_ = jag_Document_outline(m_obj);
        if (!obj_)
            throw Exception();
        return DocumentOutline(obj_);
#else
        return DocumentOutline(jag_Document_outline(m_obj));
#endif
    }

    Font font_load(Char const* fspec)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_Font obj_ = jag_Document_font_load(m_obj, fspec);
        if (!obj_)
            throw Exception();
        return Font(obj_);
#else
        return Font(jag_Document_font_load(m_obj, fspec));
#endif
    }

    Function function_2_load(Char const* fun)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Function result_ = jag_Document_function_2_load(m_obj, fun);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_function_2_load(m_obj, fun);
#endif
    }

    Function function_3_load(Char const* fun, Function const* array_in, UInt length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Function result_ = jag_Document_function_3_load(m_obj, fun, array_in, length);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_function_3_load(m_obj, fun, array_in, length);
#endif
    }

    Function function_4_load(Char const* fun)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Function result_ = jag_Document_function_4_load(m_obj, fun);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_function_4_load(m_obj, fun);
#endif
    }

    Image image_load(ImageDef image)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_Image obj_ = jag_Document_image_load(m_obj, image.handle_());
        if (!obj_)
            throw Exception();
        return Image(obj_);
#else
        return Image(jag_Document_image_load(m_obj, image.handle_()));
#endif
    }

    Image image_load_file(Char const* image_file_path, ImageFormat image_format=IMAGE_FORMAT_AUTO)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_Image obj_ = jag_Document_image_load_file(m_obj, image_file_path, static_cast<jag_ImageFormat>(image_format));
        if (!obj_)
            throw Exception();
        return Image(obj_);
#else
        return Image(jag_Document_image_load_file(m_obj, image_file_path, static_cast<jag_ImageFormat>(image_format)));
#endif
    }

    ImageDef image_definition() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_ImageDef obj_ = jag_Document_image_definition(m_obj);
        if (!obj_)
            throw Exception();
        return ImageDef(obj_);
#else
        return ImageDef(jag_Document_image_definition(m_obj));
#endif
    }

    ImageMask define_image_mask() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_ImageMask obj_ = jag_Document_define_image_mask(m_obj);
        if (!obj_)
            throw Exception();
        return ImageMask(obj_);
#else
        return ImageMask(jag_Document_define_image_mask(m_obj));
#endif
    }

    ImageMaskID register_image_mask(ImageMask image_mask)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        ImageMaskID result_ = jag_Document_register_image_mask(m_obj, image_mask.handle_());
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_register_image_mask(m_obj, image_mask.handle_());
#endif
    }

    Int page_number() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Int result_ = jag_Document_page_number(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_page_number(m_obj);
#endif
    }

    Int version() const
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Int result_ = jag_Document_version(m_obj);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_version(m_obj);
#endif
    }

    Page page()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        jag_Page obj_ = jag_Document_page(m_obj);
        if (!obj_)
            throw Exception();
        return Page(obj_);
#else
        return Page(jag_Document_page(m_obj));
#endif
    }

    Pattern shading_pattern_load(Char const* pattern, ColorSpace color_space, Function func)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Pattern result_ = jag_Document_shading_pattern_load(m_obj, pattern, color_space, func);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_shading_pattern_load(m_obj, pattern, color_space, func);
#endif
    }

    Pattern shading_pattern_load(Char const* pattern, ColorSpace cs, Function const* array_in, UInt length)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Pattern result_ = jag_Document_shading_pattern_load_n(m_obj, pattern, cs, array_in, length);
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_shading_pattern_load_n(m_obj, pattern, cs, array_in, length);
#endif
    }

    Pattern tiling_pattern_load(Char const* pattern, Canvas canvas)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        Pattern result_ = jag_Document_tiling_pattern_load(m_obj, pattern, canvas.handle_());
        if (jag_error_code())
            throw Exception();
        return result_;
#else
        return jag_Document_tiling_pattern_load(m_obj, pattern, canvas.handle_());
#endif
    }

    Result destination_define_reserved(Destination id, Char const* dest)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Document_destination_define_reserved(m_obj, id, dest))
            throw Exception();
#else
        return jag_Document_destination_define_reserved(m_obj, id, dest);
#endif
    }

    Result finalize()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Document_finalize(m_obj))
            throw Exception();
#else
        return jag_Document_finalize(m_obj);
#endif
    }

    Result page_end()
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Document_page_end(m_obj))
            throw Exception();
#else
        return jag_Document_page_end(m_obj);
#endif
    }

    Result page_start(Double width, Double height)
    {
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
        if (jag_Document_page_start(m_obj, width, height))
            throw Exception();
#else
        return jag_Document_page_start(m_obj, width, height);
#endif
    }


public: // operators + destructor
#if defined(_MANAGED)
    static void unspecified_bool(Document***)
    {
    }

    typedef void (*unspecified_bool_type)(Document***);
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: unspecified_bool;
    }
#else
    typedef jag_Document Document::*unspecified_bool_type;
    operator unspecified_bool_type() const { // never throws
        return m_obj==0? 0: &Document::m_obj;
    }
#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const { // never throws
        return m_obj==0;
    }

    
    ~Document() {
        jag_release(m_obj);
    }

    Document(Document const& other) {
        m_obj = other.m_obj;
        jag_addref(m_obj);
    }

    Document& operator=(Document const& other) {
        jag_release(m_obj);
        m_obj = other.m_obj;
        jag_addref(m_obj);
        return *this;
    }


    Document()
        : m_obj(0)
    {}

public: // not to be used outside this file
    explicit Document(jag_Document obj)
        : m_obj(obj)
    {}

    jag_Document handle_() {
        return m_obj;
    }
};



/* ==== free functions ==== */

inline Document create_file(Char const* file_path, Profile profile=Profile())
{
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
    jag_Document obj_ = jag_create_file(file_path, profile.handle_());
    if (!obj_)
        throw Exception();
    return Document(obj_);
#else
    return Document(jag_create_file(file_path, profile.handle_()));
#endif
}


inline Document create_stream(StreamOut const* stream, Profile profile=Profile())
{
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
    jag_Document obj_ = jag_create_stream(stream, profile.handle_());
    if (!obj_)
        throw Exception();
    return Document(obj_);
#else
    return Document(jag_create_stream(stream, profile.handle_()));
#endif
}


inline Profile create_profile()
{
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
    jag_Profile obj_ = jag_create_profile();
    if (!obj_)
        throw Exception();
    return Profile(obj_);
#else
    return Profile(jag_create_profile());
#endif
}


inline Profile create_profile_from_file(Char const* fname)
{
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
    jag_Profile obj_ = jag_create_profile_from_file(fname);
    if (!obj_)
        throw Exception();
    return Profile(obj_);
#else
    return Profile(jag_create_profile_from_file(fname));
#endif
}


inline Profile create_profile_from_string(Char const* str)
{
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
    jag_Profile obj_ = jag_create_profile_from_string(str);
    if (!obj_)
        throw Exception();
    return Profile(obj_);
#else
    return Profile(jag_create_profile_from_string(str));
#endif
}


inline UInt version()
{
#ifndef JAG_DO_NOT_USE_EXCEPTIONS
    UInt result_ = jag_version();
    if (jag_error_code())
        throw Exception();
    return result_;
#else
    return jag_version();
#endif
}



}} // namespace jag::pdf

#endif
