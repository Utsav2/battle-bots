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
#include <stdlib.h>     

class GUI
{
    private:
        class Animation
        {
            private:
                size_t cycle_index;

            public:
                GUI * gui;
                SDL_Texture * texture;
                Coordinate position;
                Coordinate from;
                Coordinate to;
                bool randomize;
                std::vector<Coordinate> cycles;
                Coordinate s_size;

                Animation(GUI * gui, SDL_Texture * texture, Coordinate position, Coordinate from, Coordinate to, bool randomize, std::vector<Coordinate>& cycles, Coordinate s_size) : 
                    gui(gui), texture(texture), position(position), from(from), to(to), randomize(randomize), cycles(cycles), s_size(s_size)
                    {
                        cycle_index = 0;
                    }

                inline Coordinate get_extra_coord_displacement(int hdiff, int vdiff)
                {
                    int extra_x = hdiff == 0 ? 0 : hdiff/std::abs(hdiff);
                    int extra_y = vdiff == 0 ? 0 : vdiff/std::abs(vdiff);  
                    return Coordinate(extra_x, extra_y);
                }

               inline Coordinate get_animation_diff(Coordinate from, Coordinate to)
                {
                    int hdiff = to.x - from.x;
                    int vdiff = to.y - from.y;
                    hdiff = hdiff/ANIMATION_CONSTANT;
                    vdiff = vdiff/ANIMATION_CONSTANT;
                    return Coordinate(hdiff, vdiff);
                }

                void add_to_diff_coords(Coordinate game_coord, Coordinate extras)
                {
                    int extra_x = extras.x;
                    int extra_y = extras.y;
                    Coordinate diagonal_cord = Coordinate(game_coord.x + extra_x, game_coord.y + extra_y);
                    Coordinate horizontal_cord = Coordinate(game_coord.x, game_coord.y + extra_y);
                    Coordinate vertical_cord = Coordinate(game_coord.x + extra_x, game_coord.y);
                    gui->diff_coords.push_back(gui->game_to_screen_coord(diagonal_cord));
                    gui->diff_coords.push_back(gui->game_to_screen_coord(horizontal_cord));
                    gui->diff_coords.push_back(gui->game_to_screen_coord(vertical_cord));
                }

                void animate()
                {
                    Coordinate diffs = get_animation_diff(from, to);
                    Coordinate game_coord = gui->screen_to_game_coord(position);
                    add_to_diff_coords(game_coord, get_extra_coord_displacement(diffs.x, diffs.y));
                    position.x += diffs.x;
                    position.y += diffs.y;
                    gui->render_texture(texture, gui->ren, position, cycles[cycle_index++/8 ], s_size);
                    if (cycle_index/8 >= cycles.size())
                    {
                        cycle_index = 0;
                    }
                }
        };

        //tiles
        int numrows;
        int numcols;
        int row_width;
        int row_height;

        std::vector<Path *> paths;
        TDMap * map;
        SDL_Texture * tile;
        SDL_Texture * background;
        SDL_Window *win;
        SDL_Renderer * ren;
        std::vector<Animation> animations;
        std::vector<Coordinate> diff_coords;
        std::map<std::string, SDL_Texture *> loaded_textures;

        void logSDLError(std::ostream &os, const std::string &msg)
        {
            os << msg << " error: " << SDL_GetError() << std::endl;
        }

        SDL_Texture* load_texture(const std::string &file, SDL_Renderer *ren)
        {
            SDL_Texture *texture;

            if(loaded_textures.find(file) != loaded_textures.end())
            {
                texture = loaded_textures[file];
            }
            else
            {
                texture = IMG_LoadTexture(ren, (GRAPHICS_PATH + file).c_str());
                if (texture == nullptr)
                {
                    logSDLError(std::cout, "LoadTexture");
                }
                loaded_textures[file] = texture;
            }
            return texture;
        }  

        void render_texture(SDL_Texture * tex, SDL_Renderer * ren, Coordinate screen_location, Coordinate sprite_coordinate, Coordinate sizes)
        {
            SDL_Rect renderQuad = {sprite_coordinate.x * sizes.x, sprite_coordinate.y * sizes.y , sizes.x, sizes.y};
            render_texture(tex, ren, &renderQuad, screen_location);
        }

        void render_texture(SDL_Texture * tex, SDL_Renderer * ren, Coordinate coordinate)
        {
            render_texture(tex, ren, nullptr, coordinate);
        }

        void render_texture(SDL_Texture * tex, SDL_Renderer * ren, SDL_Rect * rect, Coordinate coordinate)
        {
            render_texture(tex, ren, rect, coordinate.x, coordinate.y, row_width, row_height);
        }

        void render_texture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect* clip_rect, int x, int y, int rw, int rh)
        {
            SDL_Rect dst;
            dst.x = x;
            dst.y = y;
            dst.w = rw;
            dst.h = rh;
            SDL_RenderCopy(ren, tex, clip_rect, &dst);
        }

        Coordinate screen_to_game_coord(Coordinate c)
        {
            return Coordinate(c.x / row_width, c.y / row_height);
        }

        Coordinate game_to_screen_coord(Coordinate game_coord)
        {
            return Coordinate(game_coord.x * row_width, game_coord.y * row_height);
        }

        Coordinate game_to_screen_coord(int x, int y)
        {
            return game_to_screen_coord(Coordinate(x, y));
        }

        void render_game_screen(Coordinate game_coord)
        {

            Coordinate screen_cord = game_to_screen_coord(game_coord);
            render_texture(background, ren, screen_cord);

            BOOST_FOREACH(Path * path, paths)
            {
                if(path->in(game_coord))
                {
                    render_texture(tile, ren, screen_cord);
                }
            }

            Tower * tower = map->get_tower_at(game_coord);
            if(tower != nullptr)
            {
                SDL_Texture * texture = load_texture(tower->get_image_string(), ren); 
                if(texture != nullptr)
                {
                    render_texture(texture, ren, screen_cord);
                }
            } 
        }

        void re_render()
        {
            std::set<Coordinate> s(diff_coords.begin(), diff_coords.end());
            diff_coords.assign( s.begin(), s.end() );
            BOOST_FOREACH(Coordinate &screen_cord, diff_coords)
            {
                render_game_screen(screen_to_game_coord(screen_cord));
            }
        }

        void fill_screen_tiles()
        {
            for(int i = 0; i < numrows; i++)
            {
                for(int j = 0; j < numcols; j++)
                {
                    render_game_screen(Coordinate(i, j));
                }
            }
        }

        bool init()
        {
            return SDL_Init(SDL_INIT_EVERYTHING) == 0;
        }

        bool create_window()
        {
            win = SDL_CreateWindow("Tower Defense", 
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_SHOWN);

            return win != nullptr;
        }

        bool create_renderer()
        {    
            ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            return ren != nullptr;

        }

        bool create_textures()
        {
            background = load_texture("grass_night.jpg", ren);
            tile = load_texture("tile2.png", ren);
            return background != nullptr || tile != nullptr;
        }

    public:
        GUI(int rows, int columns, std::vector<Path *> paths, TDMap * map)
        {

            assert(rows > 0 && columns > 0 && paths.size() != 0 && map != nullptr);

            row_width = DEFAULT_WIDTH/rows;
            row_height = DEFAULT_HEIGHT/columns;
            numrows = rows;
            numcols = columns;
            this->map = map;
            this->paths = paths;

            /* Doesnt work with short circuiting in an if... wonder why */
            bool completed;
            completed = init();
            completed = completed && create_window();
            completed = completed && create_renderer();
            completed = completed && create_textures();

            if (!completed)
            {
                logSDLError(std::cout, "Initialization of components");
                SDL_Quit();
                exit(1);
            } 
            fill_screen_tiles();
            SDL_RenderPresent(ren);
        }

        // takes pixel coords.
        // pls take care while using.
        void set_up_animation(SDL_Texture * texture, Coordinate from, Coordinate to, bool randomize)
        {
            int w, h;
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            std::vector<Coordinate> cycles;
            cycles.push_back(Coordinate(0, 0));
            set_up_animation(texture, from, to, randomize, cycles, Coordinate(w, h));
        }

        void set_up_animation(SDL_Texture * texture, Coordinate from, Coordinate to, bool randomize, std::vector<Coordinate>& s_locations, Coordinate s_size)
        {
            animations.push_back(Animation(this, texture, from, from, to, randomize, s_locations, s_size));
            diff_coords.push_back(from);
            diff_coords.push_back(to);
        }

        void clear_animations()
        {
            re_render();
            animations.clear();
        }

        void show_animations()
        {
            re_render();
            BOOST_FOREACH(Animation & animation, animations)
            {
                animation.animate();
            }
        }

        void Update()
        {
            for(int i = 0; i < NUM_ROWS; i++)
            {
                for(int j = 0; j < NUM_COLS; j++)
                {
                    BOOST_FOREACH(Sprite * s, map->get_sprites_at(i,j))
                    {
                        SDL_Texture * sprite_texture = load_texture(s->get_spritesheet()->get_file_name(), ren);
                        if(s->is_attacked())
                        {
                            SDL_SetTextureColorMod(sprite_texture, 255, 128, 128);
                        }
                        else
                        {
                            SDL_SetTextureColorMod(sprite_texture, 255, 255, 255);                            
                        }
                        Coordinate previous_cord = game_to_screen_coord(s->get_previous_position());
                        set_up_animation(sprite_texture, previous_cord, game_to_screen_coord(s->get_coordinate()), true, s->get_sscords(), s->get_spritesheet()->get_box_size());
                    }

                    Tower * tower = map->get_tower_at(i, j);
                    if(tower != nullptr)
                    {
                        Coordinate current_cord = game_to_screen_coord(Coordinate(i, j));
                        SDL_Texture * attack_texture = load_texture(tower->get_attack_image_string(), ren);
                        BOOST_FOREACH(Coordinate c, tower->get_attack_tiles())
                        {
                            Coordinate screen_cord = game_to_screen_coord(c);
                            set_up_animation(attack_texture, current_cord, screen_cord, false);
                        }
                    }
                }
            }
            for(int i = 0; i < ANIMATION_CONSTANT; i++)
            {
                show_animations();
                SDL_RenderPresent(ren);
            }
            SDL_RenderPresent(ren);
            clear_animations();
        }
};
