#include "SDL.h"   
#include "SDL_image.h"   
#include <iostream>  
#include <assert.h>
#include "shared/path_creator.h"
#include "shared/tdmap.hpp"
#include "shared/sizes.h"
#include <map>

using namespace std;

class GUI
{

private:

  void logSDLError(std::ostream &os, const std::string &msg)
  {
    os << msg << " error: " << SDL_GetError() << std::endl;
  }
  
  std::map<std::string, SDL_Texture *> loaded_textures;
  std::string resPath;


  SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
  {
    SDL_Texture *texture;

    if(loaded_textures.find(file) != loaded_textures.end())
    {
      texture = loaded_textures[file];
    }
    else
    {
      texture = IMG_LoadTexture(ren, (resPath + file).c_str());
      if (texture == nullptr)
      {
        logSDLError(std::cout, "LoadTexture");
      }
    }
    return texture;
  }  

    /**
  * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
  * the texture's width and height
  * @param tex The source texture we want to draw
  * @param ren The renderer we want to draw to
  * @param x The x coordinate to draw to
  * @param y The y coordinate to draw to
  */
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int rw, int rh)
  {
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = rw;
    dst.h = rh;
    //Query the texture to get its width and height to use
    SDL_RenderCopy(ren, tex, NULL, &dst);
  }



  int row_width;
  int row_height;
  SDL_Renderer * ren;


  void fill_screen_tiles(int numrows, int numcols, Path * path, SDL_Renderer *ren, SDL_Texture *background, SDL_Texture * tile)
  {

    for(int i = 0; i < numrows; i++)
    {
      for(int j = 0; j < numcols; j++)
      {

        coordinate screen_cord = game_to_screen_coord(coordinate(i, j));

        renderTexture(background, ren, screen_cord.first, screen_cord.second, row_width, row_height);

        if(path->in(i, j))
        {
          renderTexture(tile, ren, screen_cord.first, screen_cord.second, row_width, row_height);
        }
      }
    }

  }

  TDMap * map;

  public:
    GUI(int rows, int columns, Path * path, TDMap * map)
    {

        assert(rows > 0 && columns > 0 && path != nullptr && map != nullptr);

        row_width = DEFAULT_WIDTH/rows;
        row_height = DEFAULT_HEIGHT/columns;
        resPath = "/home/utsav/projects/final_project/src/native/graphics/";

        this->map = map;

        SDL_Window *win;

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
          logSDLError(std::cout, "SDL_Init");
          exit(1);
        }

        win = SDL_CreateWindow("Tower Defense", 
          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        
        if (win == nullptr)
        {
          logSDLError(std::cout, "CreateWindow");
          SDL_Quit();
          exit(1);

        }
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (ren == nullptr)
        {
          logSDLError(std::cout, "CreateRenderer");
          //TODO free window
          SDL_Quit();
          exit(1);
        }
    
        SDL_Texture *background = loadTexture("grass.png", ren);
        SDL_Texture * tile = loadTexture("tile.png", ren);
        SDL_Texture *image = loadTexture("fg.bmp", ren);

        if (background == nullptr || image == nullptr || tile == nullptr)
        {
          logSDLError(std::cout, "Getting Images");
          //TODO cleanup
          SDL_Quit();
          exit(1);
        }
        
        fill_screen_tiles(rows, columns, path, ren, background, tile);
        SDL_RenderPresent(ren);
    }

    void Update()
    {

      for(int i = 0; i < NUM_ROWS; i++)
      {
        for(int j = 0; j < NUM_COLS; j++)
        {

          Tile& t = map->at(i, j);

          if(t.tower != nullptr)
          {
            SDL_Texture * texture = loadTexture(t.tower->get_image_string(), ren); 

            if(texture != nullptr)
            {
              coordinate screen_cord = game_to_screen_coord(coordinate(i, j));
              renderTexture(texture, ren, screen_cord.first, screen_cord.second, row_width, row_height);
            }
          }
        }
      }
      SDL_RenderPresent(ren);
      SDL_Delay(10000);
    }

    coordinate game_to_screen_coord(coordinate game_coord)
    {
      return coordinate(game_coord.first * row_width, game_coord.second * row_height);
    }

    coordinate game_to_screen_coord(int x, int y)
    {
      return game_to_screen_coord(coordinate(x, y));
    }
};
