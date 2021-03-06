#pragma once
#include "object.h"
#include "style.h"
#include "types.h"
#include "xh_scanner.h"
#include "context.h"

namespace litehtml
{
	struct css_text
	{
		typedef std::vector<css_text>	vector;

		tstring	text;
		tstring	baseurl;
		tstring	media;
		
		css_text()
		{
		}

		css_text(const tchar_t* txt, const tchar_t* url, const tchar_t* media_str)
		{
			text	= txt ? txt : _t("");
			baseurl	= url ? url : _t("");
			media	= media_str ? media_str : _t("");
		}

		css_text(const css_text& val)
		{
			text	= val.text;
			baseurl	= val.baseurl;
			media	= val.media;
		}
	};

	struct stop_tags_t
	{
		const litehtml::tchar_t*	tags;
		const litehtml::tchar_t*	stop_parent;
	};

	struct ommited_end_tags_t
	{
		const litehtml::tchar_t*	tag;
		const litehtml::tchar_t*	followed_tags;
	};

	class html_tag;

	class document : public object
	{
	public:
		typedef object_ptr<document>	ptr;
	private:
		element::ptr						m_root;
		document_container*					m_container;
		fonts_map							m_fonts;
		css_text::vector					m_css;
		litehtml::css						m_styles;
		litehtml::web_color					m_def_color;
		litehtml::context*					m_context;
		litehtml::size						m_size;
		static stop_tags_t 					m_stop_tags[];
		static ommited_end_tags_t			m_ommited_end_tags[];
		elements_vector						m_parse_stack;
		position::vector					m_fixed_boxes;
		media_query_list::vector			m_media_lists;
		element::ptr						m_over_element;
	public:
		document(litehtml::document_container* objContainer, litehtml::context* ctx);
		virtual ~document();

		litehtml::document_container*	container()	{ return m_container; }
		uint_ptr						get_font(const tchar_t* name, int size, const tchar_t* weight, const tchar_t* style, const tchar_t* decoration, font_metrics* fm);
		int								render(int max_width, render_type rt = render_all);
		void							draw(uint_ptr hdc, int x, int y, const position* clip);
		web_color						get_def_color()	{ return m_def_color; }
		int								cvt_units(const tchar_t* str, int fontSize, bool* is_percent = 0) const;
		int								cvt_units(css_length& val, int fontSize, int size = 0) const;
		int								width() const;
		int								height() const;
		void							add_stylesheet(const tchar_t* str, const tchar_t* baseurl, const tchar_t* media);
		bool							on_mouse_over(int x, int y, int client_x, int client_y, position::vector& redraw_boxes);
		bool							on_lbutton_down(int x, int y, int client_x, int client_y, position::vector& redraw_boxes);
		bool							on_lbutton_up(int x, int y, int client_x, int client_y, position::vector& redraw_boxes);
		bool							on_mouse_leave(position::vector& redraw_boxes);
		litehtml::element::ptr			create_element(const tchar_t* tag_name);
		element::ptr					root();
		void							get_fixed_boxes(position::vector& fixed_boxes);
		void							add_fixed_box(const position& pos);
		void							add_media_list(media_query_list::ptr list);
		bool							media_changed();

		static litehtml::document::ptr createFromString(const tchar_t* str, litehtml::document_container* objPainter, litehtml::context* ctx, litehtml::css* user_styles = 0);
		static litehtml::document::ptr createFromUTF8(const byte* str, litehtml::document_container* objPainter, litehtml::context* ctx, litehtml::css* user_styles = 0);
		static litehtml::document::ptr createFromStream(litehtml::instream& str, litehtml::document_container* objPainter, litehtml::context* ctx, litehtml::css* user_styles = 0);
	
	private:
		//void			load_default_styles();
		litehtml::element*	add_root();
		litehtml::element*	add_body();
		litehtml::uint_ptr	add_font(const tchar_t* name, int size, const tchar_t* weight, const tchar_t* style, const tchar_t* decoration, font_metrics* fm);

		void begin_parse();

		void parse_tag_start(const tchar_t* tag_name);
		void parse_tag_end(const tchar_t* tag_name);
		void parse_attribute(const tchar_t* attr_name, const tchar_t* attr_value);
		void parse_word(const tchar_t* val);
		void parse_space(const tchar_t* val);
		void parse_comment_start();
		void parse_comment_end();
		void parse_cdata_start();
		void parse_cdata_end();
		void parse_data(const tchar_t* val);
		void parse_push_element(element::ptr el);
		bool parse_pop_element();
		bool parse_pop_element(const tchar_t* tag, const tchar_t* stop_tags = _t(""));
		void parse_pop_void_element();
		void parse_pop_to_parent(const tchar_t* parents, const tchar_t* stop_parent);
		void parse_close_omitted_end(const tchar_t* tag);
		void parse_open_omitted_start(const tchar_t* tag);
		bool update_media_lists(const media_features& features);
	};

	inline element::ptr document::root()
	{
		return m_root;
	}
}
