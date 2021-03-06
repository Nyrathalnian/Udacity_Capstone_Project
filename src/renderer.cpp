#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Fish Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_SOFTWARE);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  //Initialize background texture
  if(background == NULL)
  {
    background = load_image("coral_bckgrnd.png", sdl_renderer);
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Fish &fish, SDL_Point const &food, Fish_Food &fish_food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  //Clear screen
  SDL_RenderClear(sdl_renderer);
  
  //RenderCopy for background image
  SDL_RenderCopy(sdl_renderer, background, NULL, NULL);
  
  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 178, 56, 255, 1.0);
  fish_food.food_rect.h = fish_food.get_size();
  fish_food.food_rect.w = fish_food.get_size();
  fish_food.food_rect.x = static_cast<int>(food.x) * block.w;
  fish_food.food_rect.y = static_cast<int>(food.y) * block.h;
  SDL_RenderFillRect(sdl_renderer, &fish_food.food_rect);

  // Render Fish
  fish.head_rect.h = fish.size;
  fish.head_rect.w = fish.size;
  fish.head_rect.x = static_cast<int>(fish.head_x) * block.w;
  fish.head_rect.y = static_cast<int>(fish.head_y) * block.h;
  if (fish.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 30, 217, 255, 1.0);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 253, 16, 111, 1.0);
  }
  SDL_RenderFillRect(sdl_renderer, &fish.head_rect);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Fish Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

//Load a texture to be used for the Background
SDL_Texture* Renderer::load_image(const std::string &img_path, SDL_Renderer* renderer)
{
  auto background = IMG_Load(img_path.c_str()); //creates a SDL_Surface*
  if(background == NULL)
  {
    std::cout << "SDL_Surface image load failed: " << SDL_GetError() << '\n';
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, background); //creates a SDL_Texture from a SDL_Surface
  if(texture == NULL)
  {
    std::cout << "SDL_Texure load failed: " << SDL_GetError() << '\n';
  }
  
  SDL_FreeSurface(background);  //free memory from allocated surface
  return texture;
}