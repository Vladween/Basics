#pragma once
#ifndef OFFICE_BASICS_TEXTBOX_H
#define OFFICE_BASICS_TEXTBOX_H

#include "Label.h"
#include "Input.h"

begin_basics_namespace(office)

class TextBox : public Label, public EventUpdatable
{
public:
	Input input;
	TextState bg_text;

	ElementEvent<void(TextBox*, sf::Keyboard::Key)> onKeyPressed = ElementEvent<void(TextBox*, sf::Keyboard::Key)>(&onKeyReleased),
		onKeyReleased = ElementEvent<void(TextBox*, sf::Keyboard::Key)>(&onKeyPressed);
	ElementEvent<void(TextBox*)> onMouseDown = ElementEvent<void(TextBox*)>(&onMouseUp),
		onMouseUp = ElementEvent<void(TextBox*)>(&onMouseDown, [](TextBox* _this) { _this->onMouseOver.whenExecuted()(_this); });
	std::function<void(TextBox*, sf::String)> onTextChanged = [](TextBox*, sf::String) {};
	std::function<void(TextBox*)> onEditPermissionChanged = [](TextBox*) {};

	const bool& canEdit() const
	{
		return m_canEdit;
	}
	void setActivationButton(const sf::Mouse::Button& btn)
	{
		m_activation_btn = btn;
		m_held_inside = false;
		m_held_outside = false;
	}
	const sf::Mouse::Button& getActivationButton() const
	{
		return m_activation_btn;
	}
protected:
	void _textbox_event_update(AppInfo& app)
	{
		if (app.event.mouseButton.button == m_activation_btn)
		{
			if (app.event.type == sf::Event::MouseButtonPressed)
			{
				if (background.getGlobalBounds().contains(usedMousePos(app)))
				{
					m_held_inside = true;
					onMouseDown(this);
				}
				else m_held_outside = true;
			}
			else if (app.event.type == sf::Event::MouseButtonReleased)
			{
				bool prev = m_canEdit;
				if (m_held_inside)
				{
					m_canEdit = true;
					m_held_inside = false;
					onMouseUp(this);
				}
				else if (m_held_outside)
				{
					m_canEdit = false;
					m_held_outside = false;
				}
				if (prev != m_canEdit) onEditPermissionChanged(this);
			}
		}

		if (!m_canEdit) return;

		if (app.event.type == sf::Event::KeyPressed)
		{
			onKeyPressed(this, app.event.key.code);

			sf::String str = text.getString();
			if (input.updated(text) && str != text.getString())
			{
				onTextChanged(this, str);
			}
		}
		else if (app.event.type == sf::Event::KeyReleased)
		{
			onKeyReleased(this, app.event.key.code);
		}
	}

	void _textbox_update(AppInfo& app)
	{
		_label_update(app);
		resolveTextAlign(bg_text, background.getGlobalBounds());
	}

	void event_update(AppInfo& app) override
	{
		_textbox_event_update(app);
	}
	void update(AppInfo& app) override
	{
		_textbox_update(app);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
	{
		target.draw(background);
		if (text.getString() == "") target.draw(bg_text);
		else target.draw(text);
	}
private:
	bool m_canEdit = false, m_held_inside = false, m_held_outside = false;
	sf::Mouse::Button m_activation_btn = sf::Mouse::Left;
};


end_basics_namespace(office)

#endif