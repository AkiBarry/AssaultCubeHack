#include "Menu.hpp"
#include "Canvas.hpp"
#include "Input.hpp"

// Namespace Functions

bool NMenu::is_activated = true;
std::vector<NMenu::CBaseElement *> NMenu::scene;

void NMenu::Activate()
{
	is_activated = true;
}

void NMenu::Deactivate()
{
	is_activated = false;
}

void NMenu::BeginScene()
{
	
}

void NMenu::EndScene()
{
	
}

void NMenu::UpdateAll()
{
	for (CBaseElement * i : scene)
	{
		if (!i->active || !i->enabled)
			continue;

		i->Update();
		i->UpdateChildren();
	}
}

void NMenu::DrawAll()
{
	for (CBaseElement * i : scene)
	{
		if (!i->drawn || !i->enabled)
			continue;

		i->DrawWrap();
		i->DrawChildren();
	}
}

void NMenu::UpdateAndDrawAll()
{
	for (CBaseElement * i : scene)
	{
		if (!i->enabled)
			continue;

		if (i->active && i->drawn)
		{
			i->UpdateWrap();
			i->DrawWrap();
			i->UpdateAndDrawChildren();
		}
		else if (i->active)
		{
			i->UpdateWrap();
			i->UpdateChildren();
		}
		else if (i->drawn)
		{
			i->DrawWrap();
			i->DrawChildren();
		}
	}
}

void NMenu::CBaseElement::UpdateWrap()
{
	Update();
}

void NMenu::CBaseElement::DrawWrap()
{
	if (Paint != nullptr)
	{
		Paint();
	}
	else
	{
		Draw();
	}
}

NMath::CVec2f & NMenu::CPositioned::GetPosition()
{
	return position;
}

NMath::CVec2f NMenu::CPositioned::GetPosition() const
{
	return position;
}

NMath::CVec2f & NMenu::CPositioned::GetAbsPosition()
{
	return abs_position;
}

NMath::CVec2f NMenu::CPositioned::GetAbsPosition() const
{
	return abs_position;
}

void NMenu::CPositioned::SetPosition(const NMath::CVec2f & val)
{
	position = val;
	const NMath::CVec2f boundary_delta = boundary_max - boundary_min;

	if (parent == nullptr)
	{
		abs_position = val;
		boundary_min = val;
	}
	else
	{
		abs_position = std::forward<NMath::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
		boundary_min = std::forward<NMath::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
	}

	boundary_max = boundary_min + boundary_delta;

	for (auto i :children)
	{
		const auto pos_i = dynamic_cast<CPositioned *>(i);

		if (pos_i == nullptr)
			continue;

		pos_i->SetPosition(pos_i->GetPosition());
	}

}

void NMenu::CPositioned::SetPosition(NMath::CVec2f && val)
{
	position = std::forward<NMath::CVec2f>(val);
	const NMath::CVec2f boundary_delta = boundary_max - boundary_min;

	if (parent == nullptr)
	{
		abs_position = std::forward<NMath::CVec2f>(val);
		boundary_min = std::forward<NMath::CVec2f>(val);
	}
	else
	{
		abs_position = std::forward<NMath::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
		boundary_min = std::forward<NMath::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
	}

	boundary_max = boundary_min + boundary_delta;

	for (auto i : children)
	{
		const auto pos_i = dynamic_cast<CPositioned *>(i);

		if (pos_i == nullptr)
			continue;

		pos_i->SetPosition(pos_i->GetPosition());
	}
}

// CBaseElement Functions

void NMenu::CBaseElement::UpdateChildren()
{
	for (auto i : children)
	{
		if (!i->active || !i->enabled)
			continue;

		i->DrawWrap();
		i->DrawChildren();
	}
}

void NMenu::CBaseElement::DrawChildren()
{
	for (auto i : children)
	{
		if (!i->drawn || !i->enabled)
			continue;

		i->DrawWrap();
		i->DrawChildren();
	}
}

void NMenu::CBaseElement::UpdateAndDrawChildren()
{
	for (auto i : children)
	{
		if (!i->enabled)
			continue;

		if (i->active && i->drawn)
		{
			i->UpdateWrap();
			i->DrawWrap();
			i->UpdateAndDrawChildren();
		}
		else if (i->active)
		{
			i->UpdateWrap();
			i->UpdateChildren();
		}
		else if (i->drawn)
		{
			i->DrawWrap();
			i->DrawChildren();
		}
	}
}

NMenu::CBaseElement::~CBaseElement()
{
	for (auto elem : children)
	{
		elem->~CBaseElement();
	}
}

NMath::CVec2f & NMenu::CDimensioned::GetSize()
{
	return size;
}

NMath::CVec2f NMenu::CDimensioned::GetSize() const
{
	return size;
}

void NMenu::CDimensioned::SetSize(const NMath::CVec2f & val)
{
	size = val;
	boundary_max = std::forward<NMath::CVec2f>(boundary_min + val);
}

void NMenu::CDimensioned::SetSize(NMath::CVec2f && val)
{
	size = std::forward<NMath::CVec2f>(val);
	boundary_max = std::forward<NMath::CVec2f>(boundary_min + val);
}

void NMenu::CDraggable::UpdateDragging()
{
	const auto mouse_position = NInput::GetMousePos();

	if (is_dragged)
	{
		if (NInput::IsKeyDown(VK_LBUTTON))
		{
			SetPosition(mouse_position + drag_start_offset);
		}
		else
		{
			is_dragged = false;
		}
	}
	else
	{
		if (NInput::WasKeyPressed(VK_LBUTTON) && mouse_position.WithinAABox(drag_area_min, drag_area_max))
		{
			if (parent == nullptr)
			{
				for (auto i = scene.rbegin(); i != scene.rend(); ++i) 
				{
					auto val = *i;

					if (val == this)
					{
						std::advance(i, 1);
						scene.erase(i.base());
						scene.push_back(this);
						break;
					}

					if (!val->enabled || !val->active)
						continue;

					if (mouse_position.WithinAABox(val->boundary_min, val->boundary_max))
						return;
				}

			}

			for (auto i : children)
			{
				if (!i->enabled || !i->active)
					continue;

				if (mouse_position.WithinAABox(i->boundary_min, i->boundary_max))
					return;
			}

			is_dragged = true;
			drag_start_offset = GetPosition() - mouse_position;
		}
	}

}

CColour NMenu::CColoured::GetColour() const
{
	return col;
}

CColour & NMenu::CColoured::GetColour()
{
	return col;
}

CColour NMenu::CColoured::GetOutlineColour() const
{
	return outline_col;
}

CColour & NMenu::CColoured::GetOutlineColour()
{
	return outline_col;
}

void NMenu::CColoured::SetColour(CColour && val)
{
	col = std::forward<CColour>(val);
}
void NMenu::CColoured::SetColour(CHSB && val)
{
	col = std::forward<CColour>(val.ToColour());
}

void NMenu::CColoured::SetOutlineColour(CColour && val)
{
	outline_col = std::forward<CColour>(val);
}

void NMenu::CColoured::SetOutlineColour(CHSB && val)
{
	outline_col = std::forward<CColour>(val.ToColour());
}

// CText Functions

void NMenu::CText::Draw()
{
	NCanvas::NDraw::Text(text, abs_position, 16.f, CColour(0, 0, 0));
}

std::string NMenu::CText::GetFont() const
{
	return font;
}

std::string & NMenu::CText::GetFont()
{
	return font;
}

// CWindow Functions

float NMenu::CWindow::window_side_thickness = 3.f;
float NMenu::CWindow::window_top_thickness = 20.f;

NMenu::CWindow::CWindow()
{
	boundary_max = boundary_min + NMath::CVec2f(2 * window_side_thickness, window_top_thickness + window_side_thickness);

	drag_area_min = boundary_min;
	drag_area_max = boundary_max;
	
	window_frame = CreateItem<CFrame>(this);
	window_frame->SetPosition(NMath::CVec2f(0.f, 0.f));
	window_frame->SetSize(GetSize());

	window_close_button = CreateItem<CButton>(this);
	window_close_button->SetPosition(size - NMath::CVec2f(20.f, 0.f));
	window_close_button->SetSize(NMath::CVec2f(40.f, window_top_thickness - 3.f));
	window_close_button->SetColour(CColour(200, 80, 80));
	window_close_button->OnClick = [&] {enabled = false; };

	window_text = CreateItem<CText>(this);
	window_text->SetPosition(NMath::CVec2f(0.f, -window_side_thickness));
}

void NMenu::CWindow::Update()
{
	UpdateDragging();
}

void NMenu::CWindow::Draw()
{
	NCanvas::NDraw::Rect(boundary_min + NMath::CVec2f(1.f, 1.f), boundary_max - boundary_min - NMath::CVec2f(2.f, 2.f), col);
	NCanvas::NDraw::OutlinedRect(boundary_min, boundary_max - boundary_min, outline_col);
}

void NMenu::CWindow::SetPosition(NMath::CVec2f && val)
{
	position = std::forward<NMath::CVec2f>(val);
	const NMath::CVec2f boundary_delta = std::forward<NMath::CVec2f>(boundary_max - boundary_min);

	if (parent == nullptr)
	{
		abs_position = std::forward<NMath::CVec2f>(val);
		boundary_min = std::forward<NMath::CVec2f>(val - NMath::CVec2f(window_side_thickness, window_top_thickness));
	}
	else
	{
		abs_position = std::forward<NMath::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
		boundary_min = std::forward<NMath::CVec2f>(parent->boundary_min + val - NMath::CVec2f(window_side_thickness, window_top_thickness));
	}

	boundary_max = boundary_min + boundary_delta;

	drag_area_min = boundary_min;
	drag_area_max = boundary_max;

	for (auto i : children)
	{
		const auto pos_i = dynamic_cast<CPositioned *>(i);

		if (pos_i == nullptr)
			continue;

		pos_i->SetPosition(pos_i->GetPosition());
	}
}

void NMenu::CWindow::SetSize(NMath::CVec2f && val)
{
	size = std::forward<NMath::CVec2f>(val);
	boundary_max = std::forward<NMath::CVec2f>(position + val + NMath::CVec2f(window_side_thickness, window_side_thickness));

	drag_area_max = boundary_max;

	window_frame->SetSize(std::forward<NMath::CVec2f>(val));
	window_close_button->SetPosition(NMath::CVec2f(val[0] - 40.f, -window_top_thickness));
}

void NMenu::CWindow::SetWindowPosition(NMath::CVec2f && val)
{
	SetPosition(val + NMath::CVec2f(window_side_thickness, window_top_thickness));
}

std::string NMenu::CWindow::GetFont() const
{
	return window_text->GetFont();
}
std::string & NMenu::CWindow::GetFont()
{
	return window_text->GetFont();
}

void NMenu::CWindow::SetFont(std::string && val)
{
	return window_text->SetFont(std::forward<std::string>(val));
}

std::string NMenu::CWindow::GetText() const
{
	return window_text->text;
}

std::string & NMenu::CWindow::GetText()
{
	return window_text->text;
}

void NMenu::CWindow::SetText(std::string && val)
{
	window_text->text = std::forward<std::string>(val);
}

void NMenu::CWindow::ShowCloseButton(bool val)
{
	window_close_button->enabled = val;
}

void NMenu::CFrame::Draw()
{
	if (!drawn)
		return;

	NCanvas::NDraw::Rect(abs_position, size, col);
	NCanvas::NDraw::OutlinedRect(abs_position, size, outline_col);
}

NMenu::CCheckBox::CCheckBox()
{
	checkbox_text = CreateItem<CText>(this);
	checkbox_text->SetPosition(NMath::CVec2f(size[0] + 2.f, size[1]));
}

void NMenu::CCheckBox::Update()
{
	auto mouse_position = NInput::GetMousePos();

	if (is_clicked)
	{
		if (NInput::WasKeyReleased(VK_LBUTTON))
		{
			if (mouse_position.WithinAABox(boundary_min, boundary_max))
			{
				var->SetVal(!var->GetVal());
			}

			is_clicked = false;
		}
	}
	else
	{
		if (NInput::WasKeyPressed(VK_LBUTTON))
		{
			if (mouse_position.WithinAABox(boundary_min, boundary_max))
				is_clicked = true;
		}
	}
}
void NMenu::CCheckBox::Draw()
{
	auto mouse_position = NInput::GetMousePos();

	if (is_clicked)
	{
		NCanvas::NDraw::Rect(abs_position + NMath::CVec2f(1.f, 1.f), size - NMath::CVec2f(2.f, 2.f), CColour(200, 200, 200));
	}
	else if (mouse_position.WithinAABox(boundary_min, boundary_max))
	{
		NCanvas::NDraw::Rect(abs_position + NMath::CVec2f(1.f, 1.f), size - NMath::CVec2f(2.f, 2.f), CColour(255, 255, 255));
		NCanvas::NDraw::Rect(abs_position + NMath::CVec2f(2.f, 2.f), size - NMath::CVec2f(4.f, 4.f), CColour(200, 200, 200));
	}
	else
	{
		NCanvas::NDraw::Rect(abs_position + NMath::CVec2f(1.f, 1.f), size - NMath::CVec2f(2.f, 2.f), CColour(255, 255, 255));
	}

	if (GetVal())
		NCanvas::NDraw::Rect(abs_position + NMath::CVec2f(2.f, 2.f), size - NMath::CVec2f(4.f, 4.f), col);

	NCanvas::NDraw::OutlinedRect(abs_position, size, outline_col);
}

void NMenu::CCheckBox::SetSize(NMath::CVec2f && val)
{
	CDimensioned::SetSize(std::forward<NMath::CVec2f>(val));
	checkbox_text->SetPosition(size + NMath::CVec2f(size[0] * 0.2f, 0.f));
}

void NMenu::CCheckBox::SetVariable(CVariable<bool> * val)
{
	if (var != nullptr)
		delete var;

	var = std::move(val);
}

void NMenu::CCheckBox::SetVal(bool && val)
{
	var->SetVal(std::forward<bool>(val));
}

std::string NMenu::CCheckBox::GetFont() const
{
	return checkbox_text->GetFont();
}

std::string & NMenu::CCheckBox::GetFont()
{
	return checkbox_text->GetFont();
}

void NMenu::CCheckBox::SetFont(std::string && val)
{
	checkbox_text->SetFont(std::forward<std::string>(val));
}

std::string NMenu::CCheckBox::GetText() const
{
	return checkbox_text->text;
}

std::string & NMenu::CCheckBox::GetText()
{
	return checkbox_text->text;
}

void NMenu::CCheckBox::SetText(std::string && val)
{
	checkbox_text->text = std::forward<std::string>(val);
}

bool & NMenu::CCheckBox::GetVal()
{
	return var->GetVal();
}
bool NMenu::CCheckBox::GetVal() const
{
	return var->GetVal();
}

NMenu::CButton::CButton()
{
	button_text = CreateItem<CText>(this);
	button_text->SetPosition(NMath::CVec2f(0.f, 0.f));
}

void NMenu::CButton::Update()
{
	if (!drawn || !enabled)
		return;

	auto mouse_position = NInput::GetMousePos();

	if(is_clicked)
	{
		if (NInput::WasKeyReleased(VK_LBUTTON))
		{
			if (mouse_position.WithinAABox(boundary_min, boundary_max))
			{
				if (OnClick != nullptr)
					OnClick();
			}

			is_clicked = false;
		}
	}
	else
	{
		if (NInput::WasKeyPressed(VK_LBUTTON))
		{
			if (mouse_position.WithinAABox(boundary_min, boundary_max))
				is_clicked = true;
		}
	}
}

void NMenu::CButton::Draw()
{
	auto mouse_position = NInput::GetMousePos();

	if (is_clicked)
	{
		NCanvas::NDraw::Rect(abs_position, size, clicked_col);
	}
	else if (mouse_position.WithinAABox(boundary_min, boundary_max))
	{
		NCanvas::NDraw::Rect(abs_position, size, hover_col);
	}
	else
	{
		NCanvas::NDraw::Rect(abs_position, size, col);
	}

	NCanvas::NDraw::OutlinedRect(abs_position, size, outline_col);
}

void NMenu::CButton::SetColour(CColour && val)
{
	col = std::forward<CColour>(val);
	hover_col = std::forward<CColour>(CColour(NMath::Min(val.r + 15, 255), NMath::Min(val.g + 15, 255), NMath::Min(val.b + 15, 255), val.a));
	clicked_col = std::forward<CColour>(CColour(NMath::Max(val.r - 15, 0), NMath::Max(val.g - 15, 0), NMath::Max(val.b - 15, 0), val.a));
}

std::string NMenu::CButton::GetFont() const
{
	return button_text->GetFont();
}

std::string & NMenu::CButton::GetFont()
{
	return button_text->GetFont();
}

void NMenu::CButton::SetFont(std::string && val)
{
	button_text->SetFont(std::forward<std::string>(val));
}

std::string NMenu::CButton::GetText() const
{
	return button_text->text;
}

std::string & NMenu::CButton::GetText()
{
	return button_text->text;
}

void NMenu::CButton::SetText(std::string && val)
{
	button_text->text = std::forward<std::string>(val);
	button_text->SetPosition(NMath::CVec2f(0.f, size[1]));
}
