#pragma once
#include <vector>
#include "Cursor.hpp"
#include "Variable.hpp"
#include "Colour.hpp"
#include <functional>

namespace NMenu
{
	class CBaseElement;
	class CFrame;
	class CButton;
	class CCheckBox;
}

namespace NMenu
{
	extern bool is_activated;
	extern std::vector<CBaseElement*> scene;

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

		NMath::CVec2f boundary_min = NMath::CVec2f(0.f, 0.f);
		NMath::CVec2f boundary_max = NMath::CVec2f(0.f, 0.f);

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
		NMath::CVec2f position = NMath::CVec2f(0.f, 0.f);
		NMath::CVec2f abs_position = NMath::CVec2f(0.f, 0.f);
	public:
		virtual NMath::CVec2f&	GetPosition();
		virtual NMath::CVec2f	GetPosition() const;

		virtual NMath::CVec2f&	GetAbsPosition();
		virtual NMath::CVec2f	GetAbsPosition() const;
	
		virtual void			SetPosition(const NMath::CVec2f& val);
		virtual void			SetPosition(NMath::CVec2f&& val);

		friend class CWindow;
	};

	class CDimensioned : public virtual CBaseElement
	{
	protected:
		NMath::CVec2f size;
	public:
		virtual NMath::CVec2f&	GetSize();
		virtual NMath::CVec2f	GetSize() const;
		
		virtual void			SetSize(const NMath::CVec2f& val);
		virtual void			SetSize(NMath::CVec2f&& val);
	};

	class CText : public virtual CBaseElement, public CPositioned, public CDimensioned
	{
	private:
		std::string font;

		mutable void * possible_font_caching = nullptr;
	public:
		std::string text;

		void Draw() override;

		virtual std::string		GetFont() const;
		virtual std::string&	GetFont();

		virtual void			SetFont(const std::string& val) {}
		virtual void			SetFont(std::string&& val) {}
	};

	class CDraggable : public virtual CPositioned
	{
	public:
		bool is_dragged = false;
		NMath::CVec2f drag_start_offset;

		NMath::CVec2f drag_area_min = boundary_min;
		NMath::CVec2f drag_area_max = boundary_max;

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
		CColour col = CColour(255, 255, 255);
		CColour outline_col = CColour(0, 0, 0);
	public:
		CColour			GetColour() const;
		CColour&		GetColour();
		CColour			GetOutlineColour() const;
		CColour&		GetOutlineColour();

		virtual void			SetColour(CColour&& val);
		virtual void			SetColour(CHSB&& val);

		virtual void			SetOutlineColour(CColour&& val);
		virtual void			SetOutlineColour(CHSB&& val);
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

		void	SetPosition(NMath::CVec2f&& val) override;
		void	SetSize(NMath::CVec2f&& val) override;

		void	SetWindowPosition(NMath::CVec2f&& val);

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

		void				SetSize(NMath::CVec2f&& val) override;

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

		void SetColour(CColour&& val) override;

		std::string		GetFont() const override;
		std::string&	GetFont() override;

		void			SetFont(std::string&& val) override;

		std::string		GetText() const override;
		std::string&	GetText() override;

		void			SetText(std::string&& val) override;

		CColour hover_col;
		CColour clicked_col;

		std::function<void()> OnClick = nullptr;
		bool is_clicked = false;

	};

	class CSlider : public virtual CBaseElement, public CPositioned, public CDimensioned
	{
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