#ifndef ZELTALIB_SCENE_HPP
#define ZELTALIB_SCENE_HPP

#include <map>

#include <SFML/Graphics.hpp>

#include <Zelta/Core/SceneManager.hpp>
#include <Zelta/Core/Animatable.hpp>

namespace zt {

    class SceneManager;
    //Forward declaration

    /**
     * @brief Representa la escena de un juego, como el menú principal, la pantalla de créditos o la de juego..
     * Esta clase está hecha para ser heredada. Es especialmente útil reimplementar los métodos manageEvents(),
     * logic() y render(), pero  también onActivate(), onDeactivate() y onPause().
     * */
    class Scene : public Animatable{
        friend class SceneManager;

    public:

        /**
         * @brief Estados que puede tener una escena.
         * */
        enum class State {
            ACTIVE, PAUSED, INACTIVE
        };

        /**
         * @brief Construye una escena <b>inactiva</b>.
         * @param window Referencia a la ventana donde se dibujarán los elementos de la escena.
         * */
        Scene(std::string name, sf::RenderWindow& window);
        virtual ~Scene();
        /**
         * @return Estado actual de la escena. @see State
         * */
        State getState() const;
        
        std::string getName() const;
        /**
         * @brief Permite seleccionar la ventana sobre la que se dibujará la escena.
         * @param window Ventana objetivo.
         * */
        virtual void setWindow(sf::RenderWindow& window);
        virtual sf::RenderWindow& getWindow();

    protected:
        /**
         * @brief Estado actual.
         * */
        State state;
        /**
         * @brief Referencia a la ventana sobre la que se dibujará la escena.
         * */
        sf::RenderWindow *window;
        sf::Event events;
        /**
         * @brief Nombre de la escena. Debe ser único y elegido por el usuario.
         */
        std::string name;

        /**
         * @brief Método que es llamado cuando la escena es activada.
         * @note Al reimplementar este método se debe llamar al método onActivate() de la clase padre.
         * */
        virtual void onActivate();
        /**
         * @brief Método que es llamado cuando la escena es desactivada.
         * @note Al reimplementar este método se debe llamar al método onDeactivate() de la clase padre.
         * */
        virtual void onDeactivate();
        /**
         * @brief Método que es llamado cuando la escena es pausada.
         * @note Al reimplementar este método se debe llamar al método onPause() de la clase padre.
         * */
        virtual void onPause();
        
        virtual bool isActive() const;
        virtual bool isInactive() const;
        virtual bool isPaused() const;
        
        /**
         * @brief Cambia el estado actual.
         * @param state Nuevo estado.
         * */
        virtual void setState(State state);

        /**
         * @brief Se encarga de llamar a los métodos manageEvents(), logic() y render(). El programador
         * <b>NO</b> llama a este método, de eso se encarga el SceneManager.
         * @param deltaTime Segundos que han transcurrido desde la última vez que llamamos
         * a esta función. Se e
         * Este método <b>no es un bucle</b>, sino que está hecho para ser llamado desde
         * un bucle (el verdadero bucle de juego), que está en el método manage() del
         * gestor de estados, que a su vez debería ser llamado por el método run() de Application.
         * <br><br>
         * Los métodos manageEvents() y logic() <b>sólo son llamados</b> si la escena está activa.El
         * método render() es llamado también cuando está en pausa.
         * <br>Si la escena está inactiva ninguno de los métodos es llamado.
         * <br><br>
         * Si el programador quiere <b>otro comportamiento</b>, puede reimplementar este método a su gusto.
         * */
        virtual void advanceTime(float deltaTime);
        /**
         * @brief Método pensado para ser reimplementado. Aquí iría todo lo relacionado con la gestión de eventos.
         * */
        virtual void manageEvents(float deltaTime);
        /**
         * @brief Lógica del juego (IA, colisiones, etc).
         * */
        virtual void logic(float deltaTime) = 0;
        /**
         * @brief En ella se dibujará la escena.
         * El programador <b>no</b> necesita llamar a los métodos
         * window->clear() ni window->display() porque estos son llamados en el instante apropiado por el
         * SceneManager.
         * */
        virtual void render() = 0;

        /**
         * @brief Un método rápido para finalizar la escena si el botón de cerrar ha sido pulsado.
         * Se debe utilizar dentro manageEvents(), dentro del bucle que analiza sf::Events.
         * */
        virtual void checkIfWindowClosed();

        /**
         * @brief Método para dibujar de manera más ágil en la ventana, para no tener que hacer:
         * window->draw(something), hacemos sólo: draw(something).
         * */
        virtual void draw(const sf::Drawable &drawable) const;
    };

}

#endif // SCENE_HPP
