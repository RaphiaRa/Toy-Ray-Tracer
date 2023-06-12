#include <SDL.h>
#include <toy_tracer/camera.hpp>
#include <toy_tracer/math.hpp>
#include <toy_tracer/mesh.hpp>
#include <toy_tracer/renderer.hpp>
#include <toy_tracer/scene_node.hpp>

using Vector3 = toy_tracer::math::Vector<float, 3>;
int main(int argc, char** argv)
{
    toy_tracer::Renderer renderer(800, 600);
    toy_tracer::Camera camera(4, 3, 1.0f);
    renderer.setCamera(camera);

    toy_tracer::Mesh mesh = toy_tracer::Mesh::fromStlFile("../data/monkey.stl");

    toy_tracer::SceneGraph graph;
    graph.addObserver(&renderer);

    toy_tracer::SceneNode cameraNode("camera");
    cameraNode.attach(&camera);
    cameraNode.setPos(Vector3{ 0.0f, 0.0f, -1.1f });
    graph.rootNode().attach(&cameraNode);

    toy_tracer::SceneNode meshNode("mesh");
    meshNode.attach(&mesh);
    meshNode.translate(Vector3{ 0.0f, 0.0f, 2.0f });
    meshNode.rotateX(toy_tracer::math::degToRad(80.0f));
    meshNode.rotateZ(toy_tracer::math::degToRad(-10.0f));

    graph.rootNode().attach(&meshNode);

    std::vector<uint8_t> buffer(800 * 600 * 3);
    graph.rootNode().update();
    renderer.render(buffer.data(), buffer.size());

    // Create SDL2 window, renderer and surface
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window        = SDL_CreateWindow("Example01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface* surface      = SDL_CreateRGBSurfaceFrom(buffer.data(), 800, 600, 24, 800 * 3, 0x0000FF, 0x00FF00, 0xFF0000, 0);

    // Render the surface to the window
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
    SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);

    // Wait for user to close the window
    SDL_Event event;
    while (true) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }
    }
    // cleanup
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
