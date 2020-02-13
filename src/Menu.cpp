#include "Menu.hpp"
#include "Canvas.hpp"
#include "Input.hpp"
#include "Console.hpp"
#include "Globals.hpp"

// Namespace Functions

bool NMenu::is_activated = true;
std::vector<NMenu::CBaseElement *> NMenu::scene;

UU::CVec2f NMenu::ScreenResolution()
{
	GLfloat viewport[4];
	
	glGetFloatv(GL_VIEWPORT, viewport);

	return UU::CVec2f(viewport[2], viewport[3]);
}

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
	UpdateAll();
	DrawAll();
	/*for (CBaseElement * i : scene)
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
	}*/
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

UU::CVec2f & NMenu::CPositioned::GetPosition()
{
	return position;
}

UU::CVec2f NMenu::CPositioned::GetPosition() const
{
	return position;
}

UU::CVec2f & NMenu::CPositioned::GetAbsPosition()
{
	return abs_position;
}

UU::CVec2f NMenu::CPositioned::GetAbsPosition() const
{
	return abs_position;
}

void NMenu::CPositioned::SetPosition(const UU::CVec2f & val)
{
	position = val;
	const UU::CVec2f boundary_delta = boundary_max - boundary_min;

	if (parent == nullptr)
	{
		abs_position = val;
		boundary_min = val;
	}
	else
	{
		abs_position = std::forward<UU::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
		boundary_min = std::forward<UU::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
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

void NMenu::CPositioned::SetPosition(UU::CVec2f && val)
{
	position = std::forward<UU::CVec2f>(val);
	const UU::CVec2f boundary_delta = boundary_max - boundary_min;

	if (parent == nullptr)
	{
		abs_position = std::forward<UU::CVec2f>(val);
		boundary_min = std::forward<UU::CVec2f>(val);
	}
	else
	{
		abs_position = std::forward<UU::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
		boundary_min = std::forward<UU::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
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

void NMenu::CPositioned::CenterHorizontal()
{
	UU::CVec2f offset(0.f, 0.f);
	{
		auto dim = dynamic_cast<CDimensioned*>(this);
		if (dim != nullptr)
		{
			offset[0] += dim->GetSize()[0] / 2.f;
		}
	}
	SetPosition(ScreenResolution() - offset);
}

void NMenu::CPositioned::CenterVertical()
{
	UU::CVec2f offset(0.f, 0.f);
	{
		auto dim = dynamic_cast<CDimensioned*>(this);
		if (dim != nullptr)
		{
			offset[1] += dim->GetSize()[1] / 2.f;
		}
	}
	SetPosition(ScreenResolution() / 2.f - offset);
}

void NMenu::CPositioned::Center()
{
	UU::CVec2f offset(0.f, 0.f);
	{
		auto dim = dynamic_cast<CDimensioned*>(this);
		if (dim != nullptr)
		{
			offset += dim->GetSize() / 2.f;
		}
	}

	SetPosition(ScreenResolution() / 2.f - offset);
}

// CBaseElement Functions

void NMenu::CBaseElement::UpdateChildren()
{
	for (auto i : children)
	{
		if (!i->active || !i->enabled)
			continue;

		i->UpdateWrap();
		i->UpdateChildren();
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

UU::CVec2f & NMenu::CDimensioned::GetSize()
{
	return size;
}

UU::CVec2f NMenu::CDimensioned::GetSize() const
{
	return size;
}

void NMenu::CDimensioned::SetSize(const UU::CVec2f & val)
{
	size = val;
	boundary_max = std::forward<UU::CVec2f>(boundary_min + val);
}

void NMenu::CDimensioned::SetSize(UU::CVec2f && val)
{
	size = std::forward<UU::CVec2f>(val);
	boundary_max = std::forward<UU::CVec2f>(boundary_min + val);
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

UU::CColour NMenu::CColoured::GetColour() const
{
	return col;
}

UU::CColour & NMenu::CColoured::GetColour()
{
	return col;
}

UU::CColour NMenu::CColoured::GetOutlineColour() const
{
	return outline_col;
}

UU::CColour & NMenu::CColoured::GetOutlineColour()
{
	return outline_col;
}

void NMenu::CColoured::SetColour(UU::CColour && val)
{
	col = std::forward<UU::CColour>(val);
}
void NMenu::CColoured::SetColour(UU::CHSB && val)
{
	col = std::forward<UU::CColour>(val.ToColour());
}

void NMenu::CColoured::SetOutlineColour(UU::CColour && val)
{
	outline_col = std::forward<UU::CColour>(val);
}

void NMenu::CColoured::SetOutlineColour(UU::CHSB && val)
{
	outline_col = std::forward<UU::CColour>(val.ToColour());
}

// CText Functions

void NMenu::CText::Draw()
{
	NCanvas::NDraw::Text(text, abs_position, 16.f, UU::CColour(0, 0, 0));
}

std::string NMenu::CText::GetFont() const
{
	return font;
}

std::string & NMenu::CText::GetFont()
{
	return font;
}

std::string NMenu::CText::GetText() const
{
	return text;
}

std::string & NMenu::CText::GetText()
{
	return text;
}

void NMenu::CText::SetText(const std::string & val)
{
	text = val;
}
void NMenu::CText::SetText(std::string && val)
{
	text = std::move(val);
}

// CWindow Functions

float NMenu::CWindow::window_side_thickness = 3.f;
float NMenu::CWindow::window_top_thickness = 20.f;

NMenu::CWindow::CWindow()
{
	boundary_max = boundary_min + UU::CVec2f(2 * window_side_thickness, window_top_thickness + window_side_thickness);

	drag_area_min = boundary_min;
	drag_area_max = boundary_max;
	
	window_frame = CreateItem<CFrame>(this);
	window_frame->SetPosition(UU::CVec2f(0.f, 0.f));
	window_frame->SetSize(GetSize());

	window_close_button = CreateItem<CButton>(this);
	window_close_button->SetPosition(size - UU::CVec2f(20.f, 0.f));
	window_close_button->SetSize(UU::CVec2f(40.f, window_top_thickness - 3.f));
	window_close_button->SetColour(UU::CColour(200, 80, 80));
	window_close_button->OnClick = [&] {enabled = false; };

	window_text = CreateItem<CText>(this);
	window_text->SetPosition(UU::CVec2f(0.f, -window_side_thickness));
}

void NMenu::CWindow::Update()
{
	UpdateDragging();
}

void NMenu::CWindow::Draw()
{
	NCanvas::NDraw::Rect(boundary_min + UU::CVec2f(1.f, 1.f), boundary_max - boundary_min - UU::CVec2f(2.f, 2.f), col);
	NCanvas::NDraw::OutlinedRect(boundary_min, boundary_max - boundary_min, outline_col);
}

void NMenu::CWindow::SetPosition(UU::CVec2f && val)
{
	position = std::forward<UU::CVec2f>(val);
	const UU::CVec2f boundary_delta = std::forward<UU::CVec2f>(boundary_max - boundary_min);

	if (parent == nullptr)
	{
		abs_position = std::forward<UU::CVec2f>(val);
		boundary_min = std::forward<UU::CVec2f>(val - UU::CVec2f(window_side_thickness, window_top_thickness));
	}
	else
	{
		abs_position = std::forward<UU::CVec2f>(dynamic_cast<CPositioned *>(parent)->abs_position + position);
		boundary_min = std::forward<UU::CVec2f>(parent->boundary_min + val - UU::CVec2f(window_side_thickness, window_top_thickness));
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

void NMenu::CWindow::SetSize(UU::CVec2f && val)
{
	size = std::forward<UU::CVec2f>(val);
	boundary_max = std::forward<UU::CVec2f>(position + val + UU::CVec2f(window_side_thickness, window_side_thickness));

	drag_area_max = boundary_max;

	window_frame->SetSize(std::forward<UU::CVec2f>(val));
	window_close_button->SetPosition(UU::CVec2f(val[0] - 40.f, -window_top_thickness));
}

void NMenu::CWindow::SetWindowPosition(UU::CVec2f && val)
{
	SetPosition(val + UU::CVec2f(window_side_thickness, window_top_thickness));
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
	return window_text->GetText();
}

std::string & NMenu::CWindow::GetText()
{
	return window_text->GetText();
}

void NMenu::CWindow::SetText(std::string && val)
{
	window_text->GetText() = std::forward<std::string>(val);
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
	checkbox_text->SetPosition(UU::CVec2f(size[0] + 2.f, size[1]));
}

void NMenu::CCheckBox::Update()
{
	auto mouse_position = NInput::GetMousePos();

	if (is_clicked)
	{
		if (NInput::WasKeyReleased(VK_LBUTTON))
		{
			if (mouse_position.WithinAABox(boundary_min, boundary_max)
				|| mouse_position.WithinAABox(checkbox_text->boundary_min, checkbox_text->boundary_max))
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
		NCanvas::NDraw::Rect(abs_position + UU::CVec2f(1.f, 1.f), size - UU::CVec2f(2.f, 2.f), UU::CColour(200, 200, 200));
	}
	else if (mouse_position.WithinAABox(boundary_min, boundary_max))
	{
		NCanvas::NDraw::Rect(abs_position + UU::CVec2f(1.f, 1.f), size - UU::CVec2f(2.f, 2.f), UU::CColour(255, 255, 255));
		NCanvas::NDraw::Rect(abs_position + UU::CVec2f(2.f, 2.f), size - UU::CVec2f(4.f, 4.f), UU::CColour(200, 200, 200));
	}
	else
	{
		NCanvas::NDraw::Rect(abs_position + UU::CVec2f(1.f, 1.f), size - UU::CVec2f(2.f, 2.f), UU::CColour(255, 255, 255));
	}

	if (GetVal())
		NCanvas::NDraw::Rect(abs_position + UU::CVec2f(2.f, 2.f), size - UU::CVec2f(4.f, 4.f), col);

	NCanvas::NDraw::OutlinedRect(abs_position, size, outline_col);
}

void NMenu::CCheckBox::SetSize(UU::CVec2f && val)
{
	CDimensioned::SetSize(std::forward<UU::CVec2f>(val));
	checkbox_text->SetPosition(size + UU::CVec2f(size[0] * 0.2f, 0.f));
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
	return checkbox_text->GetText();
}

std::string & NMenu::CCheckBox::GetText()
{
	return checkbox_text->GetText();
}

void NMenu::CCheckBox::SetText(std::string && val)
{
	checkbox_text->GetText() = std::forward<std::string>(val);
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
	button_text->SetPosition(UU::CVec2f(0.f, 0.f));
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

void NMenu::CButton::SetColour(UU::CColour && val)
{
	col = val;
	hover_col = UU::CColour(UU::Min(val.r + 15, 255), UU::Min(val.g + 15, 255), UU::Min(val.b + 15, 255), static_cast<int32_t>(val.a));
	clicked_col = UU::CColour(UU::Max(val.r - 15, 0), UU::Max(val.g - 15, 0), UU::Max(val.b - 15, 0), static_cast<int32_t>(val.a));
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
	return button_text->GetText();
}

std::string & NMenu::CButton::GetText()
{
	return button_text->GetText();
}

void NMenu::CButton::SetText(std::string && val)
{
	button_text->SetText(std::forward<std::string>(val));
	button_text->SetPosition(UU::CVec2f(0.f, size[1]));
}