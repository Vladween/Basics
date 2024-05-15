#pragma once
#ifndef GAME_BASICS_MAP_H
#define GAME_BASICS_MAP_H

#include "Objects.h"
#include <fstream>

begin_basics_namespace(game)

class TileBase
{
public:
	friend class Map;

	void setTileOrigin(sf::Vector2f origin)
	{
		m_tile_origin = origin;
	}
	sf::Vector2f getTileOrigin() const
	{
		return m_tile_origin;
	}
protected:
	virtual std::unique_ptr<TileBase> unique_copy() const
	{
		return std::make_unique<TileBase>(*this);
	}
private:
	sf::Vector2f m_tile_origin;
	virtual void _setPosition(sf::Vector2f position)
	{
	}
	virtual void _setSize(sf::Vector2f size)
	{
	}
	virtual sf::Vector2f _getSize() const
	{
		return {};
	}
	virtual void _Draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const
	{
	}
};

template<typename ObjectT>
class Tile : public TileBase, public ObjectT
{
public:
	static_assert(std::is_base_of<Object, ObjectT>::value, "'ObjectT must be derived from Object!");
	static_assert(!std::is_base_of<RigidBody, ObjectT>::value, "'ObjectT' must be Trigger or Collider!");
protected:
	std::unique_ptr<TileBase> unique_copy() const override = 0;
private:
	void _setPosition(sf::Vector2f position) override final
	{
		this->ObjectT::setPosition(position);
	}
	void _setSize(sf::Vector2f size) override final
	{
		this->ObjectT::setSize(size);
	}
	sf::Vector2f _getSize() const override final
	{
		return this->ObjectT::getSize();
	}
	void _Draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override final
	{
		target.draw(*this, states);
	}
};

class ObjectTile : public Tile<Object>
{
protected:
	std::unique_ptr<TileBase> unique_copy() const override
	{
		return std::make_unique<ObjectTile>(*this);
	}
};

class TriggerTile : public Tile<Trigger>
{
protected:
	std::unique_ptr<TileBase> unique_copy() const override
	{
		return std::make_unique<TriggerTile>(*this);
	}
};

class ColliderTile : public Tile<Collider>
{
protected:
	std::unique_ptr<TileBase> unique_copy() const override
	{
		return std::make_unique<ColliderTile>(*this);
	}
};


class Map : public sf::Drawable
{
public:
	sf::RectangleShape background;

	void setTileSet(const std::vector<std::vector<std::string>> tileSet)
	{
		m_tileSet = tileSet;
		update();
	}

	bool loadTileSet(std::string filePath, std::string& exception_str)
	{
		std::vector<std::vector<std::string>> tileSet;

		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			file.open(filePath);
		}
		catch (std::system_error& e)
		{
			exception_str = e.code().message();
			return false;
		}

		while (!file.eof())
		{
			std::string str;
			std::getline(file, str, '\n');

			std::vector<std::string> line;
			for (auto sym : str)
			{
				std::string sym_str = "";
				sym_str += sym;
				line.push_back(sym_str);
			}

			tileSet.push_back(line);
		}

		setTileSet(tileSet);

		return true;
	}
	bool loadTileSet(std::string filePath)
	{
		std::string e;
		return loadTileSet(filePath, e);
	}
	const std::vector<std::vector<std::string>> getTileSet() const
	{
		return m_tileSet;
	}

	void setTileSize(sf::Vector2f tileSize)
	{
		m_tileSize = tileSize;
		update();
	}
	sf::Vector2f getTileSize() const
	{
		return m_tileSize;
	}

	void setTileTemplate(std::string code, const TileBase& tile)
	{
		m_tilesMap[code] = tile.unique_copy();
		update();
	}
	const std::unique_ptr<TileBase>& getTileTemplate(std::string code) const
	{
		return m_tilesMap.at(code);
	}

	const std::vector<std::vector<std::unique_ptr<TileBase>>>& getTiles() const
	{
		return m_tiles;
	}
	sf::FloatRect getBounds() const
	{
		if (m_tiles.size() == 0) return {background.getPosition(), sf::Vector2f(0, 0)};

		size_t max = m_tiles[0].size();
		for (size_t i = 1; i < m_tiles.size(); i++)
		{
			if (m_tiles[i].size() > max)
				max = m_tiles[i].size();
		}
		return { background.getPosition().x, background.getPosition().y, max * m_tileSize.x, m_tiles.size() * m_tileSize.y };
	}
protected:
	void update()
	{
		m_tiles.clear();

		if (m_tiles.size() != m_tileSet.size())
			m_tiles.resize(m_tileSet.size());
		for (size_t i = 0; i < m_tiles.size(); i++)
		{
			if (m_tiles[i].size() != m_tileSet[i].size())
				m_tiles[i].resize(m_tileSet[i].size());
			for (size_t j = 0; j < m_tiles[i].size(); j++)
			{
				if (m_tilesMap.find(m_tileSet[i][j]) == m_tilesMap.end()) continue;

				m_tiles[i][j] = m_tilesMap[m_tileSet[i][j]]->unique_copy();
				if(m_tiles[i][j]->_getSize() == sf::Vector2f(0, 0))
					m_tiles[i][j]->_setSize(m_tileSize);
				m_tiles[i][j]->_setPosition(background.getPosition() + sf::Vector2f{ j * m_tileSize.x, i * m_tileSize.y } + m_tiles[i][j]->getTileOrigin());
			}
		}

		if (m_tiles.size() == 0) return;

		size_t max = m_tiles[0].size();
		for (size_t i = 1; i < m_tiles.size(); i++)
		{
			if (m_tiles[i].size() > max)
				max = m_tiles[i].size();
		}

		background.setSize({ max * m_tileSize.x, m_tiles.size() * m_tileSize.y });
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
	{
		_draw(target, states);
	}


	void _draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const
	{
		target.draw(background);
		draw_tiles(target, states);
	}
	void draw_tiles(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const
	{
		sf::FloatRect view_bounds(target.getView().getCenter() - target.getView().getSize() / 2.f, target.getView().getSize());
		for (size_t i = (size_t)view_bounds.top / (size_t)m_tileSize.y; i < size_t(view_bounds.top + view_bounds.height) / (size_t)m_tileSize.y + 1; i++)
		{
			if (i >= m_tiles.size()) continue;

			for (size_t j = (size_t)view_bounds.left / (size_t)m_tileSize.x; j < size_t(view_bounds.left + view_bounds.width) / (size_t)m_tileSize.x + 1; j++)
			{
				if (j >= m_tiles[i].size()) continue;

				if (m_tiles[i][j] != nullptr)
					m_tiles[i][j]->_Draw(target, states);
			}				
		}
	}
private:
	std::vector<std::vector<std::string>> m_tileSet;
	std::map<std::string, std::unique_ptr<TileBase>> m_tilesMap;
	std::vector<std::vector<std::unique_ptr<TileBase>>> m_tiles;
	sf::Vector2f m_tileSize;
};

end_basics_namespace(game)

#endif