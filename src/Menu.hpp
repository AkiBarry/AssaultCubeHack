#pragma once
#include <vector>
#include "Cursor.hpp"
#include "Variable.hpp"
#include "UU.hpp"
#include "Input.hpp"
#include "Canvas.hpp"

#include <functional>
#include <sstream>
#include <windows.h>

namespace NMenu
{
	class CBaseElement;
	class CFrame;
	class CButton;
	class CCheckBox;
/*}

namespace NMenu
{*/
	extern bool is_activated;
	extern std::vector<CBaseElement*> scene;
	
	UU::CVec2f ScreenResolution(); 

	void Activate();
	void Deactivate();

	void BeginScene();
	void EndScene();

	void UpdateAll();
	void DrawAll();

	void UpdateAndDrawAll();

	template<class ItemType>
	ItemType * CreateItem(CBaseElement * parent = nullptr);

	class CBaseElement
	{
	private:
		virtual void	UpdateWrap();
		virtual void	DrawWrap();
	public:
		CBaseElement * parent = nullptr;
		std::vector<CBaseElement *> children;

		UU::CVec2f boundary_min = UU::CVec2f(0.f, 0.f);
		UU::CVec2f boundary_max = UU::CVec2f(0.f, 0.f);

		bool enabled = true;
		bool active = true;
		bool drawn = true;

		virtual void	Update() {}
		virtual void	Draw() {}

		std::function<void()> Paint = nullptr;

		void UpdateChildren();
		void DrawChildren();
		void UpdateAndDrawChildren();
		
		virtual			~CBaseElement();

		friend void UpdateAll();
		friend void DrawAll();
		friend void UpdateAndDrawAll();
	};

	class CPositioned : public virtual CBaseElement
	{
	protected:
		UU::CVec2f position = UU::CVec2f(0.f, 0.f);
		UU::CVec2f abs_position = UU::CVec2f(0.f, 0.f);
	public:
		virtual UU::CVec2f&	GetPosition();
		virtual UU::CVec2f	GetPosition() const;

		virtual UU::CVec2f&	GetAbsPosition();
		virtual UU::CVec2f	GetAbsPosition() const;
	
		virtual void			SetPosition(const UU::CVec2f& val);
		virtual void			SetPosition(UU::CVec2f&& val);

		virtual void CenterHorizontal();
		virtual void CenterVertical();
		virtual void Center();

		friend class CWindow;
	};

	class CDimensioned : public virtual CBaseElement
	{
	protected:
		UU::CVec2f size;
	public:
		virtual UU::CVec2f&	GetSize();
		virtual UU::CVec2f	GetSize() const;
		
		virtual void			SetSize(const UU::CVec2f& val);
		virtual void			SetSize(UU::CVec2f&& val);
	};

	class CText : public virtual CBaseElement, public CPositioned, public CDimensioned
	{
	private:
		std::string font;
		std::string text;


		mutable void * possible_font_caching = nullptr;
	public:
		void Draw() override;

		virtual std::string		GetFont() const;
		virtual std::string&	GetFont();

		virtual void			SetFont(const std::string& val) {}
		virtual void			SetFont(std::string&& val) {}

		virtual std::string		GetText() const;
		virtual std::string&	GetText();

		virtual void			SetText(const std::string& val);
		virtual void			SetText(std::string&& val);
	};

	class CDraggable : public virtual CPositioned
	{
	public:
		bool is_dragged = false;
		UU::CVec2f drag_start_offset;

		UU::CVec2f drag_area_min = boundary_min;
		UU::CVec2f drag_area_max = boundary_max;

		virtual void UpdateDragging();
	};

	class CTexted
	{
	private:
	public:
		virtual std::string		GetFont() const = 0;
		virtual std::string&	GetFont() = 0;

		virtual void			SetFont(std::string&& val) = 0;

		virtual std::string		GetText() const = 0;
		virtual std::string&	GetText() = 0;

		virtual void			SetText(std::string&& val) = 0;
	};

	class CColoured
	{
	protected:
		UU::CColour col = UU::CColour::White;
		UU::CColour outline_col = UU::CColour::Black;
	public:
		UU::CColour			GetColour() const;
		UU::CColour&		GetColour();
		UU::CColour			GetOutlineColour() const;
		UU::CColour&		GetOutlineColour();

		virtual void			SetColour(UU::CColour&& val);
		virtual void			SetColour(UU::CHSB&& val);

		virtual void			SetOutlineColour(UU::CColour&& val);
		virtual void			SetOutlineColour(UU::CHSB&& val);
	};

	class CWindow : public virtual CBaseElement, public virtual CPositioned, public CDimensioned, public CDraggable, public CColoured, public CTexted
	{
	protected:
		CButton * window_close_button;
		CText * window_text;
	public:
		CWindow();

		CFrame * window_frame;

		void Update() override;
		void Draw() override;

		void	SetPosition(UU::CVec2f&& val) override;
		void	SetSize(UU::CVec2f&& val) override;

		void	SetWindowPosition(UU::CVec2f&& val);

		std::string		GetFont() const override;
		std::string&	GetFont() override;
		
		void			SetFont(std::string&& val) override;
		
		std::string		GetText() const override;
		std::string&	GetText() override;
		
		void			SetText(std::string&& val) override;

		void			ShowCloseButton(bool val);

		static float window_side_thickness, window_top_thickness;
	};

	class CFrame : public virtual CBaseElement, public CPositioned, public CDimensioned, public CColoured
	{
	public:
		void Draw() override;
	};

	class CCheckBox : public virtual CBaseElement, public CPositioned, public CDimensioned, public CColoured, public CTexted
	{
	protected:
		CVariable<bool> * var = nullptr;
		CText * checkbox_text;
	public:
		CCheckBox();

		void Update() override;
		void Draw() override;

		void				SetSize(UU::CVec2f&& val) override;

		void				SetVariable(CVariable<bool> * val);

		bool	&			GetVal();
		bool				GetVal() const;

		void				SetVal(bool&& val);

		std::string		GetFont() const override;
		std::string&	GetFont() override;

		void			SetFont(std::string&& val) override;

		std::string		GetText() const override;
		std::string&	GetText() override;

		void			SetText(std::string&& val) override;

		bool is_clicked = false;
	};

	class CButton : public virtual CBaseElement, public CPositioned, public CDimensioned, public CColoured, public CTexted
	{
	protected:
		CText * button_text = nullptr;
	public:
		CButton();

		void Update() override;
		void Draw() override;

		void SetColour(UU::CColour&& val) override;

		std::string		GetFont() const override;
		std::string&	GetFont() override;

		void			SetFont(std::string&& val) override;

		std::string		GetText() const override;
		std::string&	GetText() override;

		void			SetText(std::string&& val) override;

		UU::CColour hover_col;
		UU::CColour clicked_col;

		std::function<void()> OnClick = nullptr;
		bool is_clicked = false;

	};

	template<typename T>
	class CSlider : public virtual CBaseElement, public CPositioned, public CDimensioned, public CColoured, public CTexted
	{
	protected:
		CVariable<T> * var = nullptr;
		T min = T(0);
		T max = T(1);
		CText * slider_text;
		float slider_width = 20.f;
		bool is_dragged = false;
		float drag_start_offset;
	public:
		CSlider();

		void Update() override;
		void Draw() override;

		void SetColour(UU::CColour&& val) override;

		void			SetMinMax(T a, T b);

		void			SetVariable(CVariable<T> * val);

		T &				GetVal();
		T				GetVal() const;

		void			SetVal(const T& val);
		void			SetVal(T&& val);

		std::string		GetFont() const override;
		std::string&	GetFont() override;

		void			SetFont(std::string&& val) override;

		std::string		GetText() const override;
		std::string&	GetText() override;

		void			SetText(std::string&& val) override;

		UU::CColour hover_col;
		UU::CColour dragged_col;
	};

	class CSheet : public CBaseElement
	{
		
	};
}

template<class ItemType>
ItemType * NMenu::CreateItem(CBaseElement * parent /* = nullptr */)
{
	ItemType * temp_item = new ItemType();

	if (parent == nullptr)
		scene.push_back(dynamic_cast<CBaseElement *>(temp_item));
	else
	{
		parent->children.push_back(dynamic_cast<CBaseElement *>(temp_item));
		temp_item->parent = parent;
	}

	return temp_item;
}

/* Slider definitions */

template<typename T>
NMenu::CSlider<T>::CSlider()
{
	slider_text = CreateItem<CText>(this);
	slider_text->SetPosition(UU::CVec2f(0.f, slider_text->GetSize()[1]));
}

template<typename T>
void NMenu::CSlider<T>::Update()
{
	if (!drawn || !enabled)
		return;

	auto mouse_position = NInput::GetMousePos();

	if(is_dragged)
	{
		float slider_pos = (mouse_position[0] - drag_start_offset - boundary_min[0]) / (boundary_max[0] - boundary_min[0]);

		if (slider_pos < 0.f)
		{
			SetVal(min);
		}
		else if (slider_pos > 1.f)
		{
			SetVal(max);
		}
		else
		{
			if constexpr (std::is_integral_v<T>)
			{
				SetVal(T(0.5f + slider_pos * (max - min) + min));
			}
			else
			{
				SetVal(T(slider_pos * (max - min) + min));
			}
		}
		
		if (NInput::WasKeyReleased(VK_LBUTTON))
		{
			is_dragged = false;
		}
	}
	else
	{
		if (NInput::WasKeyPressed(VK_LBUTTON))
		{
			float slider_pos = size[0] * (float)(var->GetVal() - min) / (float)(max - min);
			UU::CVec2f slide_bounds_min = UU::CVec2f(abs_position[0] + slider_pos - slider_width / 2.f, boundary_min[1]);
			UU::CVec2f slide_bounds_max = UU::CVec2f(abs_position[0] + slider_pos + slider_width / 2.f, boundary_max[1]);;

			if (mouse_position.WithinAABox(slide_bounds_min, slide_bounds_max))
			{
				is_dragged = true;
				drag_start_offset = mouse_position[0] - slide_bounds_min[0];
			}
			else if (mouse_position.WithinAABox(boundary_min, boundary_max))
			{
				float slider_frac = (mouse_position[0] - boundary_min[0]) / (boundary_max[0] - boundary_min[0]);
				SetVal(T(slider_frac * (max - min) + min));
			}
		}
	}
}

template<typename T>
void NMenu::CSlider<T>::Draw()
{
	auto mouse_position = NInput::GetMousePos();
	float slider_pos = size[0] * (float)(var->GetVal() - min) / (float)(max - min);


	NCanvas::Draw::SetColour(UU::CColour::Black);
	NCanvas::Draw::Line(
		abs_position + UU::CVec2f(0.f, size[1] / 2.f),
		abs_position + UU::CVec2f(size[0], size[1] / 2.f));

	if (is_dragged)
	{
		NCanvas::Draw::SetColour(dragged_col);
		NCanvas::Draw::FilledRect(
			abs_position + UU::CVec2f(slider_pos - slider_width / 2.f, 0.f), 
			UU::CVec2f(slider_width, size[1]));
	}
	else
	{
		UU::CVec2f slide_bounds_min = UU::CVec2f(abs_position[0] + slider_pos - slider_width / 2.f, boundary_min[1]);
		UU::CVec2f slide_bounds_max = UU::CVec2f(abs_position[0] + slider_pos + slider_width / 2.f, boundary_max[1]);;
		
		if (mouse_position.WithinAABox(slide_bounds_min, slide_bounds_max))
		{
			NCanvas::Draw::SetColour(hover_col);
			NCanvas::Draw::FilledRect(
				abs_position + UU::CVec2f(slider_pos - slider_width / 2.f, 0.f), 
				UU::CVec2f(slider_width, size[1]));
		}
		else
		{
			NCanvas::Draw::SetColour(col);
			NCanvas::Draw::FilledRect(
				abs_position + UU::CVec2f(slider_pos - slider_width / 2.f, 0.f), 
				UU::CVec2f(slider_width, size[1]));
		}
	}

	NCanvas::Draw::SetColour(UU::CColour::Black);
	NCanvas::Draw::OutlinedRect(
		abs_position + UU::CVec2f(slider_pos - slider_width / 2.f, 0.f), 
		UU::CVec2f(slider_width, size[1]));

	std::ostringstream out;
	out << GetVal();

	NCanvas::Draw::SetColour(UU::CColour::Black);
	NCanvas::Draw::Text(out.str(), abs_position + size, "consolas.ttf", 16);
}

template<typename T>
void NMenu::CSlider<T>::SetColour(UU::CColour && val)
{
	col = val;
	hover_col = UU::CColour(UU::Min(val.r + 15, 255), UU::Min(val.g + 15, 255), UU::Min(val.b + 15, 255), static_cast<int32_t>(val.a));
	dragged_col = UU::CColour(UU::Max(val.r - 15, 0), UU::Max(val.g - 15, 0), UU::Max(val.b - 15, 0), static_cast<int32_t>(val.a));
}

template<typename T>
void NMenu::CSlider<T>::SetMinMax(T a, T b)
{
	min = a;
	max = b;
}

template<typename T>
void NMenu::CSlider<T>::SetVariable(CVariable<T> * val)
{
	var = val;
}

template<typename T>
T & NMenu::CSlider<T>::GetVal()
{
	return var->GetVal();
}

template<typename T>
T NMenu::CSlider<T>::GetVal() const
{
	return var->GetVal();
}

template<typename T>
void NMenu::CSlider<T>::SetVal(const T & val)
{
	var->SetVal(val);
}

template<typename T>
void NMenu::CSlider<T>::SetVal(T && val)
{
	var->SetVal(std::forward<T>(val));
}

template<typename T>
std::string NMenu::CSlider<T>::GetFont() const
{
	return slider_text->GetFont();
}

template<typename T>
std::string & NMenu::CSlider<T>::GetFont()
{
	return slider_text->GetFont();
}

template<typename T>
void NMenu::CSlider<T>::SetFont(std::string && val)
{
	slider_text->SetFont(std::forward<std::string>(val));
}

template<typename T>
std::string NMenu::CSlider<T>::GetText() const
{
	return slider_text->GetText();
}

template<typename T>
std::string & NMenu::CSlider<T>::GetText()
{
	return slider_text->GetText();
}

template<typename T>
void NMenu::CSlider<T>::SetText(std::string && val)
{
	slider_text->SetText(std::forward<std::string>(val));
}
