#pragma once
#ifndef OFFICE_BASICS_INPUT_H
#define OFFICE_BASICS_INPUT_H

#include "SystemBasics/Config.h"
#include <set>
#include <list>
#include <bitset>

begin_basics_namespace(office)

class Input
{
public:
	friend class TextBox;

	size_t cols = 100;
	bool colsAsPixels = false;

	enum Type { Letters = 1, Numbers = 2, Special = 4, NumpadNums = 8, NumpadOps = 16, Numpad = 32 };
	enum KeyType { All, NormalOnly, ShiftOnly };

	struct Key
	{
		std::string normal = "";
		std::string shift = "";

		Key(std::string norm = "", std::string shif = "")
			: normal(norm), shift(shif)
		{
		}
	};

	void setAvailableKeys(std::set<sf::Keyboard::Key> keys, KeyType key_type = All)
	{
		m_available.clear();
		m_changed.clear();
		appendAvailableKeys(keys, key_type);
	}
	void setType(int type, KeyType key_type = All)
	{
		m_available.clear();
		m_changed.clear();
		appendType(type, key_type);
	}
	void appendAvailableKeys(std::set<sf::Keyboard::Key> keys, KeyType key_type = All)
	{
		for (auto key : keys)
			m_available.insert(key);
		switch (key_type)
		{
		case Input::All:
			for (auto key : keys)
				m_changed.erase(key);
			break;
		case Input::NormalOnly:
			for (auto key : keys)
				m_changed[key] = Input::Key(m_keys[key].normal, m_keys[key].normal);
			break;
		case Input::ShiftOnly:
			for (auto key : keys)
				m_changed[key] = Input::Key("", m_keys[key].shift);
			break;
		}
	}
	void appendType(int type, KeyType key_type = All)
	{
		std::set<sf::Keyboard::Key> keys;

		std::bitset<6> bits = type;
		
		for (size_t i = 0; i < bits.size(); i++)
			std::cout << bits[i] << "\n";

		if (bits[0]) keys.insert({
				sf::Keyboard::A,
				sf::Keyboard::B,
				sf::Keyboard::C,
				sf::Keyboard::D,
				sf::Keyboard::E,
				sf::Keyboard::F,
				sf::Keyboard::G,
				sf::Keyboard::H,
				sf::Keyboard::I,
				sf::Keyboard::J,
				sf::Keyboard::K,
				sf::Keyboard::L,
				sf::Keyboard::M,
				sf::Keyboard::N,
				sf::Keyboard::O,
				sf::Keyboard::P,
				sf::Keyboard::Q,
				sf::Keyboard::R,
				sf::Keyboard::S,
				sf::Keyboard::T,
				sf::Keyboard::U,
				sf::Keyboard::V,
				sf::Keyboard::W,
				sf::Keyboard::X,
				sf::Keyboard::Y,
				sf::Keyboard::Z,
				sf::Keyboard::Space
			});
		if (bits[1]) keys.insert({
				sf::Keyboard::Num0,
				sf::Keyboard::Num1,
				sf::Keyboard::Num2,
				sf::Keyboard::Num3,
				sf::Keyboard::Num4,
				sf::Keyboard::Num5,
				sf::Keyboard::Num6,
				sf::Keyboard::Num7,
				sf::Keyboard::Num8,
				sf::Keyboard::Num9,
			});
		if (bits[2]) keys.insert({
				sf::Keyboard::LBracket,
				sf::Keyboard::RBracket,
				sf::Keyboard::Semicolon,
				sf::Keyboard::Comma,
				sf::Keyboard::Period,
				sf::Keyboard::Apostrophe,
				sf::Keyboard::Slash,
				sf::Keyboard::Grave,
				sf::Keyboard::Equal,
				sf::Keyboard::Hyphen,
				sf::Keyboard::Tab,
				sf::Keyboard::Space
			});
		if (bits[3]) keys.insert({
				sf::Keyboard::Numpad0,
				sf::Keyboard::Numpad1,
				sf::Keyboard::Numpad2,
				sf::Keyboard::Numpad3,
				sf::Keyboard::Numpad4,
				sf::Keyboard::Numpad5,
				sf::Keyboard::Numpad6,
				sf::Keyboard::Numpad7,
				sf::Keyboard::Numpad8,
				sf::Keyboard::Numpad9
			});
		if (bits[4]) keys.insert({
				sf::Keyboard::Add,
				sf::Keyboard::Subtract,
				sf::Keyboard::Multiply,
				sf::Keyboard::Divide
			});
		if (bits[5]) keys.insert({
				sf::Keyboard::Numpad0,
				sf::Keyboard::Numpad1,
				sf::Keyboard::Numpad2,
				sf::Keyboard::Numpad3,
				sf::Keyboard::Numpad4,
				sf::Keyboard::Numpad5,
				sf::Keyboard::Numpad6,
				sf::Keyboard::Numpad7,
				sf::Keyboard::Numpad8,
				sf::Keyboard::Numpad9,
				sf::Keyboard::Add,
				sf::Keyboard::Subtract,
				sf::Keyboard::Multiply,
				sf::Keyboard::Divide
			});

		for (auto key : keys)
			m_available.insert(key);
		switch (key_type)
		{
		case Input::All:
			for (auto key : keys)
				m_changed.erase(key);
			break;
		case Input::NormalOnly:
			for (auto key : keys)
				m_changed[key] = Input::Key(m_keys[key].normal, "");
			break;
		case Input::ShiftOnly:
			for (auto key : keys)
				m_changed[key] = Input::Key("", m_keys[key].shift);
			break;
		}
	}

	bool updated(sf::Text& text)
	{
		bool res = false;

		for (auto key : m_available)
		{
			bool inBounds;
			if (colsAsPixels) inBounds = text.getGlobalBounds().width + text.getCharacterSize() < cols;
			else inBounds = text.getString().getSize() < cols;

			if (!sf::Keyboard::isKeyPressed(key) || !inBounds) continue;

			Key& input_key = (m_changed.find(key) != m_changed.end()) ? m_changed[key] : m_keys[key];

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			{
				text.setString(text.getString() + input_key.shift);
			}
			else
			{
				text.setString(text.getString() + input_key.normal);
			}
			res = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && text.getString().getSize() != 0)
		{
			std::string str = text.getString().toAnsiString();
			str.pop_back();
			text.setString(str);
			res = true;
		}

		return res;
	}
private:
	std::set<sf::Keyboard::Key> m_available = {
		sf::Keyboard::A,
		sf::Keyboard::B,
		sf::Keyboard::C,
		sf::Keyboard::D,
		sf::Keyboard::E,
		sf::Keyboard::F,
		sf::Keyboard::G,
		sf::Keyboard::H,
		sf::Keyboard::I,
		sf::Keyboard::J,
		sf::Keyboard::K,
		sf::Keyboard::L,
		sf::Keyboard::M,
		sf::Keyboard::N,
		sf::Keyboard::O,
		sf::Keyboard::P,
		sf::Keyboard::Q,
		sf::Keyboard::R,
		sf::Keyboard::S,
		sf::Keyboard::T,
		sf::Keyboard::U,
		sf::Keyboard::V,
		sf::Keyboard::W,
		sf::Keyboard::X,
		sf::Keyboard::Y,
		sf::Keyboard::Z,

		sf::Keyboard::Num0,
		sf::Keyboard::Num1,
		sf::Keyboard::Num2,
		sf::Keyboard::Num3,
		sf::Keyboard::Num4,
		sf::Keyboard::Num5,
		sf::Keyboard::Num6,
		sf::Keyboard::Num7,
		sf::Keyboard::Num8,
		sf::Keyboard::Num9,

		sf::Keyboard::Numpad0,
		sf::Keyboard::Numpad1,
		sf::Keyboard::Numpad2,
		sf::Keyboard::Numpad3,
		sf::Keyboard::Numpad4,
		sf::Keyboard::Numpad5,
		sf::Keyboard::Numpad6,
		sf::Keyboard::Numpad7,
		sf::Keyboard::Numpad8,
		sf::Keyboard::Numpad9,
		sf::Keyboard::Add,
		sf::Keyboard::Subtract,
		sf::Keyboard::Multiply,
		sf::Keyboard::Divide,

		sf::Keyboard::LBracket,
		sf::Keyboard::RBracket,
		sf::Keyboard::Semicolon,
		sf::Keyboard::Comma,
		sf::Keyboard::Period,
		sf::Keyboard::Apostrophe,
		sf::Keyboard::Slash,
		sf::Keyboard::Grave,
		sf::Keyboard::Equal,
		sf::Keyboard::Hyphen,
		sf::Keyboard::Tab,
		sf::Keyboard::Space
	};
	std::map<sf::Keyboard::Key, Key> m_changed = {};

	static std::map<sf::Keyboard::Key, Key> m_keys;
};

std::map<sf::Keyboard::Key, Input::Key> Input::m_keys = {
	// Letters
	{sf::Keyboard::A, Input::Key("a", "A")},
	{sf::Keyboard::B, Input::Key("b", "B")},
	{sf::Keyboard::C, Input::Key("c", "C")},
	{sf::Keyboard::D, Input::Key("d", "D")},
	{sf::Keyboard::E, Input::Key("e", "E")},
	{sf::Keyboard::F, Input::Key("f", "F")},
	{sf::Keyboard::G, Input::Key("g", "G")},
	{sf::Keyboard::H, Input::Key("h", "H")},
	{sf::Keyboard::I, Input::Key("i", "I")},
	{sf::Keyboard::J, Input::Key("j", "J")},
	{sf::Keyboard::K, Input::Key("k", "K")},
	{sf::Keyboard::L, Input::Key("l", "L")},
	{sf::Keyboard::M, Input::Key("m", "M")},
	{sf::Keyboard::N, Input::Key("n", "N")},
	{sf::Keyboard::O, Input::Key("o", "O")},
	{sf::Keyboard::P, Input::Key("p", "P")},
	{sf::Keyboard::Q, Input::Key("q", "Q")},
	{sf::Keyboard::R, Input::Key("r", "R")},
	{sf::Keyboard::S, Input::Key("s", "S")},
	{sf::Keyboard::T, Input::Key("t", "T")},
	{sf::Keyboard::U, Input::Key("u", "U")},
	{sf::Keyboard::V, Input::Key("v", "V")},
	{sf::Keyboard::W, Input::Key("w", "W")},
	{sf::Keyboard::X, Input::Key("x", "X")},
	{sf::Keyboard::Y, Input::Key("y", "Y")},
	{sf::Keyboard::Z, Input::Key("z", "Z")},

	// Numbers
	{sf::Keyboard::Num0, Input::Key("0", ")")},
	{sf::Keyboard::Num1, Input::Key("1", "!")},
	{sf::Keyboard::Num2, Input::Key("2", "@")},
	{sf::Keyboard::Num3, Input::Key("3", "#")},
	{sf::Keyboard::Num4, Input::Key("4", "$")},
	{sf::Keyboard::Num5, Input::Key("5", "%")},
	{sf::Keyboard::Num6, Input::Key("6", "^")},
	{sf::Keyboard::Num7, Input::Key("7", "&")},
	{sf::Keyboard::Num8, Input::Key("8", "*")},
	{sf::Keyboard::Num9, Input::Key("9", "(")},

	// Numpad
	{sf::Keyboard::Numpad0, Input::Key("0", "")},
	{sf::Keyboard::Numpad1, Input::Key("1", "")},
	{sf::Keyboard::Numpad2, Input::Key("2", "")},
	{sf::Keyboard::Numpad3, Input::Key("3", "")},
	{sf::Keyboard::Numpad4, Input::Key("4", "")},
	{sf::Keyboard::Numpad5, Input::Key("5", "")},
	{sf::Keyboard::Numpad6, Input::Key("6", "")},
	{sf::Keyboard::Numpad7, Input::Key("7", "")},
	{sf::Keyboard::Numpad8, Input::Key("8", "")},
	{sf::Keyboard::Numpad9, Input::Key("9", "")},
	{sf::Keyboard::Add,		Input::Key("+", "")},
	{sf::Keyboard::Subtract,Input::Key("-", "")},
	{sf::Keyboard::Multiply,Input::Key("*", "")},
	{sf::Keyboard::Divide,	Input::Key("/", "")},

	// Special
	{sf::Keyboard::LBracket,   Input::Key("[", "{")},
	{sf::Keyboard::RBracket,   Input::Key("]", "}")},
	{sf::Keyboard::Semicolon,  Input::Key(";", ":")},
	{sf::Keyboard::Comma,	   Input::Key(",", "<")},
	{sf::Keyboard::Period,	   Input::Key(".", ">")},
	{sf::Keyboard::Apostrophe, Input::Key("'", "\"")},
	{sf::Keyboard::Slash,	   Input::Key("/", "?")},
	{sf::Keyboard::Grave,	   Input::Key("`", "~")},
	{sf::Keyboard::Equal,	   Input::Key("=", "+")},
	{sf::Keyboard::Hyphen,	   Input::Key("-", "_")},
	{sf::Keyboard::Tab,		   Input::Key("\t", "")},
	{sf::Keyboard::Space,	   Input::Key(" ", "")}
};

end_basics_namespace(office)

#endif