#include "SDL.h"   
#include "SDL_image.h"   
#include <iostream>  
#include <assert.h>
#include "shared/path_creator.hpp"
#include "shared/tdmap.hpp"
#include "shared/sizes.h"
#include <map>
#include <cmath>
#include <set>
#include <boost/foreach.hpp>
#include "graphics/graphics_path.hpp"

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
      loaded_textures[file] = texture;
    }
    return texture;
  }  

    /**
  * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
  * the texture's width and height
  * @param tex The source texture we want to draw
  * @param ren The renderer we want to draw to
  * @param x The x Coordinate to draw to
  * @param y The y Coordinate to draw to
  */
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int rw, int rh)
  {
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = rw;
    dst.h = rh;
    SDL_RenderCopy(ren, tex, NULL, &dst);
  }

  int row_width;
  int row_height;
  SDL_Renderer * ren;
  Path * main_path;
  int numrows;
  int numcols;


  Coordinate screen_to_game_coord(Coordinate c)
  {
    return Coordinate(c.x / row_width, c.y / row_height);
  }

  void re_render()
  {
    std::set<Coordinate> s( diff_coords.begin(), diff_coords.end() );
    diff_coords.assign( diff_coords.begin(), diff_coords.end() );

    BOOST_FOREACH(Coordinate &screen_cord, diff_coords)
    {
    
      Coordinate game_coord = screen_to_game_coord(screen_cord);

      renderTexture(background, ren, screen_cord.x, screen_cord.y, row_width, row_height);

      if(main_path->in(game_coord.x, game_coord.y))
      {
        renderTexture(tile, ren, screen_cord.x, screen_cord.y, row_width, row_height);
      }

      Tower * tower = map->get_tower_at(game_coord);

      if(tower != nullptr)
      {
        SDL_Texture * texture = loadTexture(tower->get_image_string(), ren); 
        if(texture != nullptr)
        {
          renderTexture(texture, ren, screen_cord.x, screen_cord.y, row_width, row_height);
        }
      } 

    }
  }

  void fill_screen_tiles()
  {

    for(int i = 0; i < numrows; i++)
    {
      for(int j = 0; j < numcols; j++)
      {

        Coordinate screen_cord = game_to_screen_coord(Coordinate(i, j));

        renderTexture(background, ren, screen_cord.x, screen_cord.y, row_width, row_height);

        if(main_path->in(i, j))
        {
          renderTexture(tile, ren, screen_cord.x, screen_cord.y, row_width, row_height);
        }

        Tower * tower = map->get_tower_at(i, j);

        if(tower != nullptr)
        {
          SDL_Texture * texture = loadTexture(tower->get_image_string(), ren); 

          Coordinate current_cord = game_to_screen_coord(Coordinate(i, j));

          if(texture != nullptr)
          {
            renderTexture(texture, ren, current_cord.x, current_cord.y, row_width, row_height);
          }
        }
      }
    }

  }

  TDMap * map;
  int current_anim_frame;
  SDL_Texture * tile;
  SDL_Texture * background;

  public:
    GUI(int rows, int columns, Path * path, TDMap * map)
    {

        assert(rows > 0 && columns > 0 && path != nullptr && map != nullptr);

        current_anim_frame = 0;
        row_width = DEFAULT_WIDTH/rows;
        row_height = DEFAULT_HEIGHT/columns;
        resPath = GRAPHICS_PATH;
        numrows = rows;
        numcols = columns;
        this->map = map;
        SDL_Window *win;
        main_path = path;

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
    
        background = loadTexture("grass.png", ren);
        tile = loadTexture("tile.png", ren);

        if (background == nullptr || tile == nullptr)
        {
          logSDLError(std::cout, "Getting Images");
          //TODO cleanup
          SDL_Quit();
          exit(1);
        }
        fill_screen_tiles();
        SDL_RenderPresent(ren);
        SDL_Delay(1000);        

    }

    typedef std::pair<std::pair<SDL_Texture *, Coordinate> , std::pair<Coordinate, Coordinate>> anim_type;

    std::vector<anim_type> animations;

    std::vector<Coordinate> diff_coords;

    // takes pixel coords.
    // pls take care while using.
    void set_up_animation(SDL_Texture * texture, Coordinate from, Coordinate to, bool addToDiff)
    {
        animations.push_back(anim_type(std::make_pair(texture, from), std::make_pair(from, to)));
        diff_coords.push_back(from);
        diff_coords.push_back(to);
    }

    void clear_attack_animations()
    {
        animations.clear();
    }

    bool show_animations()
    {
        re_render();
        BOOST_FOREACH(anim_type & animation, animations)
        {
            SDL_Texture * tex = animation.first.first;
            auto from_to = animation.second;
            int hdiff = from_to.second.x - from_to.first.x;
            int vdiff = from_to.second.y - from_to.first.y;
            hdiff = hdiff/ANIMATION_CONSTANT;
            vdiff = vdiff/ANIMATION_CONSTANT;
            int extra_x = hdiff == 0 ? 0 : hdiff/std::abs(hdiff);
            int extra_y = vdiff == 0 ? 0 : vdiff/std::abs(vdiff);  
            Coordinate game_coord = screen_to_game_coord(animation.first.second);
            Coordinate new_game_coord = Coordinate(game_coord.x + extra_x, game_coord.y + extra_y);
            Coordinate new_game_coord2 = Coordinate(game_coord.x, game_coord.y + extra_y);
            Coordinate new_game_coord3 = Coordinate(game_coord.x + extra_x, game_coord.y);
            diff_coords.push_back(game_to_screen_coord(new_game_coord));
            diff_coords.push_back(game_to_screen_coord(new_game_coord2));
            diff_coords.push_back(game_to_screen_coord(new_game_coord3));
            animation.first.second.x += hdiff;
            animation.first.second.y += vdiff;
            renderTexture(tex, ren, animation.first.second.x, animation.first.second.y, row_width, row_height);
        }
        if(++current_anim_frame < ANIMATION_CONSTANT)
        {
          SDL_Delay(10);
          return false;
        }
        return true;
    }

    void Update()
    {
      current_anim_frame = 0;
      for(int i = 0; i < NUM_ROWS; i++)
      {
        for(int j = 0; j < NUM_COLS; j++)
        {
          Tower * tower = map->get_tower_at(i, j);
          if(tower != nullptr)
          {
            Coordinate current_cord = game_to_screen_coord(Coordinate(i, j));
            SDL_Texture * attack_texture = loadTexture(tower->get_attack_image_string(), ren);
            BOOST_FOREACH(Coordinate c, tower->get_attack_tiles())
            {
              Coordinate screen_cord = game_to_screen_coord(c);
              set_up_animation(attack_texture, current_cord, screen_cord, true);
            }
          }
          BOOST_FOREACH(Sprite * s, map->get_sprites_at(i,j))
          {
          
            SDL_Texture * sprite_texture = loadTexture(s->image_string, ren);
            Coordinate previous_cord = game_to_screen_coord(s->get_previous_position());
            set_up_animation(sprite_texture, previous_cord, game_to_screen_coord(s->get_coordinate()), true);
          }
        }
      }
      while(!show_animations())
      {
        SDL_RenderPresent(ren);
      }
      re_render();
      SDL_Delay(300);
      SDL_RenderPresent(ren);
      clear_attack_animations();
    }

    Coordinate game_to_screen_coord(Coordinate game_coord)
    {
      return Coordinate(game_coord.x * row_width, game_coord.y * row_height);
    }

    Coordinate game_to_screen_coord(int x, int y)
    {
      return game_to_screen_coord(Coordinate(x, y));
    }
};
