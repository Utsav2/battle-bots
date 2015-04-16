#include "SDL.h"   
#include "SDL_image.h"   
#include <iostream>  
#include <assert.h>
#include "shared/path_creator.h"
#include "shared/tdmap.hpp"
#include "shared/sizes.h"

using namespace std;

class GUI
{

private:

  void logSDLError(std::ostream &os, const std::string &msg)
  {
    os << msg << " error: " << SDL_GetError() << std::endl;
  }
  
  SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
  {
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr)
    {
      logSDLError(std::cout, "LoadTexture");
      std::cout<<file.c_str()<<std::endl;
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


  void fill_screen_tiles(int numrows, int numcols, Path * path, SDL_Renderer *ren, SDL_Texture *background, SDL_Texture * tile)
  {

    int row_width = DEFAULT_WIDTH/numrows;
    int row_height = DEFAULT_HEIGHT/numcols;
    for(int i = 0; i < numrows; i++)
    {
      for(int j = 0; j < numcols; j++)
      {
        if(path->in(i, j))
        {
          renderTexture(tile, ren, i * row_width, j * row_height, row_width, row_height);
        }
        else
        {
          renderTexture(background, ren, i * row_width, j * row_height, row_width, row_height);
        }
      }
    }

  }

  TDMap * map;

  public:
    GUI(int rows, int columns, Path * path, TDMap * map)
    {

        assert(rows > 0 && columns > 0 && path != nullptr && map != nullptr);
	IMG_Init(IMG_INIT_PNG);
        this->map = map;

        SDL_Window *win;
        SDL_Renderer *ren;

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
    
        const std::string resPath = "/home/shiv/Programming/battle-bots/native/graphics/";
        SDL_Texture *background = loadTexture(resPath + "fg.bmp", ren);
        SDL_Texture * tile = loadTexture(resPath + "tile.jpg", ren);
        SDL_Texture *image = loadTexture(resPath + "grass.png", ren);
        
        if (background == nullptr || image == nullptr || tile == nullptr)
            {
          logSDLError(std::cout, "Getting Images");
          //TODO cleanup
          SDL_Quit();
          exit(1);
        }
        
        fill_screen_tiles(rows, columns, path, ren, background, tile);
        SDL_RenderPresent(ren);

        SDL_Delay(10000);

    }

    void Update()
    {

    }
};
