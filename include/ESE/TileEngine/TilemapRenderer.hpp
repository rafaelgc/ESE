#ifndef TILEMAPRENDERER_HPP
#define TILEMAPRENDERER_HPP

#include <cstring>
#include <iostream>

#include <SFML/Graphics/Drawable.hpp>

#include <ESE/External/pugixml.hpp>
#include <ESE/TileEngine/TileDrawable.hpp>
#include <iostream>

namespace ESE {

    template <class T>
    class TilemapRenderer : public sf::Drawable {
    protected:
        sf::Texture *tileset;
        std::map<int, sf::IntRect>rects;
        /**
         * @brief Ancho y alto en tiles del mapa.
         * */
        int width, height;
        /**
         * @brief Ancho y alto de cata tile.
         * */
        int tileWidth, tileHeight;
        std::vector<T>tiles;
        
        sf::View view;

    public:

        TilemapRenderer() {
            tileset = nullptr;
            width = height = 0;
            tileWidth = tileHeight = 0;
        }

        ~TilemapRenderer() {
            /*for (unsigned int i=0; i<tiles.size(); i++){
                if (tiles[i]){
                    delete tiles[i];
                }
            }*/
        }

        /**
         * @brief Permite seleccionar el tileset que se utilizará para renderizar los tiles.
         * */
        void setTileset(sf::Texture *tileset) {
            this->tileset = tileset;
        }

        /**
         * @brief Cada tipo de tile, representado con un número entero, tiene relacionado un
         * trozo de imagen extraido del tileset. Mediante esta función establecemos qué trozo
         * de imagen se relacionará con cada tipo de tile.
         * @param number El identificador del tile al que queremos relacionar con el trozo de imagen.
         * @param rect Mediante rect delimitamos el trozo de imagen que queremos relacionar con el
         * tipo de tile.
         * */
        void setTilesetCoords(int number, sf::IntRect rect) {
            rects[number] = rect;
        }

        /**
         * @brief Este método permite establecer los distintos trozos de imagen conociendo únicamente
         * el ancho y alto de cada tile, de tal modo que nos ahorrará el esfuerzo de llamar al método
         * setTilesetCoords(). <b>Si llamamos a este método no es necesario llamar a setTileDimensions()</b>
         * */
        void detectTilesetCoords(int tileWidth, int tileHeight) {
            this->tileWidth = tileWidth;
            this->tileHeight = tileHeight;

            rects.clear();
            //No haremos nada si no hay un tileset establecido.
            if (tileset) {
                //Vamos a calcular cuántos tiles tiene de ancho.
                unsigned int w = tileset->getSize().x / tileWidth;
                unsigned int h = tileset->getSize().y / tileHeight;

                for (unsigned int itY = 0; itY < h; itY++) {
                    for (unsigned int itX = 0; itX < w; itX++) {
                        setTilesetCoords(itY * w + itX + 1, sf::IntRect(itX*tileWidth, itY*tileHeight, tileWidth, tileHeight));
                    }
                }

            }
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {

            for (const T& tile : tiles) {
                if (tile.getPosition().x + tile.getSize().x < view.getCenter().x - view.getSize().x / 2) { continue; }
                if (tile.getPosition().x > view.getCenter().x + view.getSize().x / 2) { continue; }
                if (tile.getPosition().y + tile.getSize().y < view.getCenter().y - view.getSize().y / 2) { continue; }
                if (tile.getPosition().y > view.getCenter().y + view.getSize().y / 2) { continue; }
                
                target.draw(tile, states);
            }
        }

        /**
         * @brief Este método sirve para especificar el ancho y alto, en píxeles, de cada
         * tile. <b>Es importante</b> hacer esto antes de añadir algún tile al tilemap, pues
         * para calcular la posición de cada tile es necesario conocer su ancho y alto. También
         * cabe mencionar que si llamamos al método detectTilesetCoords() no es necesario llamar
         * a este método.
         * */
        void setTileDimensions(int tileWidth, int tileHeight) {
            this->tileWidth = tileWidth;
            this->tileHeight = tileHeight;
        }

        /**
         * @brief Permite añadir un tile de cierto tipo y en cierta posición. Es imprescindible
         * haber llamado antes al método reserve() si queremos que el puntero que devuelve el método
         * sea un puntero válido.
         * @see reserve()
         * @param type Tipo de tile.
         * @param colX Posición horizontal, <b>en casillas</b> (no en píxeles), del tile.
         * @param colY Posición vertical, <b>en casillas</b> (no en píxeles), del tile.
         * @return El métdoo devuelve un std::pair cuyo primer elemento es la posición
         * en el vector del tile que acabamos de añadir y un puntero a dicho tile.
         * */
        std::pair<int, T*> addTile(int type, int colX, int rowY) {
            //int index = colRowToIndex(colX,rowY);
            //std::cout << width << " -->" << colX << ", " << rowY << ": " << std::endl;
            T tmpTile;
            tmpTile.setTexture(*tileset);
            tmpTile.setTextureRect(rects[type]);
            tmpTile.setPosition(colX*tileWidth, rowY * tileHeight);
            tiles.push_back(tmpTile);
            return std::make_pair(tiles.size() - 1, &tiles.back());
        }

        /**
         * Este método sirve para obtener un cierto tile.
         * @param Posición del tile que queremos obtener dentro del array.
         * @return Puntero al tile que hay en la posición especificada.
         * */
        T* tileAt(int index) {
            return (&tiles[index]);
        }

        /**
         * @brief Reserva espacio para el mapa en el vector que lo contiene. Es imprescindible
         * llamarlo antes de llamar al método addTile().
         * @see addTile()
         * @param tilemapWidth Ancho, en casillas, del mapa.
         * @param tilemapHeight Alto, en casillas, del mapa.
         */
        void reserve(int tilemapWidth, int tilemapHeight) {
            tiles.reserve(tilemapWidth * tilemapHeight);
            //tiles.resize(tilemapWidth*tilemapHeight);
        }

        /**
         * @param x Posición horizontal.
         * @param y Posición vertical.
         * @return Índice del tile que podemos encontrar en la posición indicada.
         */
        int indexAtPosition(float x, float y) {
            sf::Vector2i pos = tileCoordsAtPosition(x, y);
            return colRowToIndex(pos.x, pos.y);
        }

        /**
         * @return Posición, en tiles, de la casilla que hay en la posición indicada. 
         */
        sf::Vector2i tileCoordsAtPosition(float x, float y) {
            int tX = x / tileWidth;
            int tY = y / tileHeight;
            return sf::Vector2i(tX, tY);
        }

        int colRowToIndex(int col, int row) {
            return row * width + col;
        }

        /**
         * @brief Permite elegir las dimensiones, en tiles, del mapa. <b>Es imprescindible</b> llamar
         * a este método antes de añadir cualquier tile al renderizador.
         * @param width
         * @param height
         */
        void setTilemapDimensions(int width, int height) {
            this->width = width;
            this->height = height;
        }

        std::vector<T*>* getTiles() {
            return &tiles;
        }

        void clear() {
            tiles.clear();
            tiles.resize(0);

        }
        
        void updateView(const sf::View& view) {
            this->view = view;
        }

    };

}
#endif