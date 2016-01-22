#ifndef ESE_TEXTURECONTAINER_HPP
#define ESE_TEXTURECONTAINER_HPP

#include <SFML/Graphics.hpp>

#include <ESE/Core/ResourceManager.hpp>
#include <ESE/Core/Log.hpp>

namespace ESE {

    /**
     * @brief Es un contenedor de texturas que hereda de ResourceManager.
     * */
    class Textures : public ESE::ResourceManager<sf::Texture> {
    private:
        Textures() {}
    public:
        virtual void loadFromFile(const std::string & name, const std::string & file) {
            if (resources[name].loadFromFile(file) == false) {
                std::cout << "Error" << std::endl;
            }
        }

    };

}

#include <ESE/Core/TextureContainer.cpp>

#endif // TEXTURECONTAINER_HPP
