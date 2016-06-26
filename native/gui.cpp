#include "SDL.h"   
#include "SDL_image.h"   
#include "SDL_ttf.h"
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


#define TOWER_HOVER_SURFACE "tower_hover"
#define SIDEBAR_OFFSET 1

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
                size_t frame_rate;
                int direction;


                Animation(GUI * gui, SDL_Texture * texture, Coordinate position, Coordinate from, Coordinate to, bool randomize, std::vector<Coordinate>& cycles, Coordinate s_size, size_t frame_rate, int direction) : 
                    gui(gui), texture(texture), position(position), from(from), to(to), randomize(randomize), cycles(cycles), s_size(s_size), frame_rate(frame_rate), direction(direction)
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
                    if(direction == -1)
                        animate(cycle_index);
                    else
                        animate(direction * frame_rate);
                }

                void animate(size_t specific_cycle_index)
                {
                    cycle_index = specific_cycle_index;
                    Coordinate diffs = get_animation_diff(from, to);
                    Coordinate game_coord = gui->screen_to_game_coord(position);
                    add_to_diff_coords(game_coord, get_extra_coord_displacement(diffs.x, diffs.y));
                    position.x += diffs.x;
                    position.y += diffs.y;
                    gui->render_texture(texture, gui->ren, position, cycles[cycle_index++/frame_rate], s_size);
                    if (cycle_index/frame_rate >= cycles.size())
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

        std::vector<Path> paths;
        TDMap * map;
        SDL_Texture * tile;
        SDL_Texture * background;
        SDL_Window *win;
        SDL_Renderer * ren;
        std::vector<Animation> animations;
        std::vector<Coordinate> diff_coords;
        std::map<std::string, SDL_Texture *> loaded_textures;
        typedef std::pair<std::string, bool> file_string_colored;
        std::map<std::string, SDL_Surface *> loaded_surfaces;
        std::map<file_string_colored, SDL_Texture*> loaded_sprite_textures;
        std::map<Coordinate, bool> colors;
        SDL_Thread * event_thread;
        bool quit;
        SDL_Texture * tile_hover;
        SDL_Texture * sidebar;
        SDL_mutex *lock;
        SDL_cond *cond;
        TTF_Font * font; 
        int previous_score;
        int actual_width;



        void logSDLError(std::ostream &os, const std::string &msg)
        {
            os << msg << " error: " << SDL_GetError() << std::endl;
        }

        SDL_Surface * load_surface(const std::string & file)
        {
            if(loaded_surfaces.find(file) != loaded_surfaces.end())
            {
                return loaded_surfaces[file];
            }
            else
            {
                SDL_Surface * surface = IMG_Load((GRAPHICS_PATH + file).c_str());
                loaded_surfaces[file] = surface;
                return surface;
            }

        }

        SDL_Texture * load_sprite_texture(const std::string &file, bool colored)
        {
            file_string_colored f = std::make_pair(file, colored);
            if(loaded_sprite_textures.find(f) != loaded_sprite_textures.end())
            {
                return loaded_sprite_textures[f];
            }
            else
            {   
                SDL_Surface * surface = load_surface(file);
                SDL_Texture * texture = SDL_CreateTextureFromSurface(ren, surface);
                if(colored)
                    SDL_SetTextureColorMod(texture, 255, 0, 0);
                loaded_sprite_textures[f] = texture;
                return texture;
            }
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
                SDL_Surface * surface = load_surface(file);
                if (surface == nullptr)
                {
                    logSDLError(std::cout, "LoadTexture");                    
                }
                texture = SDL_CreateTextureFromSurface(ren, surface);
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

        void render_texture(SDL_Texture * tex, SDL_Renderer * ren, Coordinate coordinate, int width, int height)
        {
            render_texture(tex, ren, nullptr, coordinate.x, coordinate.y, width, height);
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

        void render_game_screen(Coordinate game_coord, bool colorize)
        {

            Coordinate screen_cord = game_to_screen_coord(game_coord);

            if(false)
            {
                SDL_SetTextureColorMod(background, 255, 128, 128);
                SDL_SetTextureColorMod(tile, 255, 128, 128);
            }
            else
            {
                SDL_SetTextureColorMod(background, 255, 255, 255);
                SDL_SetTextureColorMod(tile, 255, 255, 255);
            }

            render_texture(background, ren, screen_cord);

            BOOST_FOREACH(Path & path, paths)
            {
                if(path.in(game_coord))
                {
                    render_texture(tile, ren, screen_cord);
                }
            }

            Tower * tower = map->get_tower_at(game_coord);

            SDL_SetRenderTarget(ren, background);

            if(tower != nullptr)
            {
                SDL_Texture * texture = load_texture(tower->get_image_string(), ren); 
                if(texture != nullptr)
                {
                    render_texture(texture, ren, screen_cord);
                }
            } 
            SDL_SetRenderTarget(ren, nullptr);
        }

        void re_render()
        {
            fill_screen_tiles(false);
        }

        void fill_screen_tiles(bool colorize)
        {
            SDL_LockMutex(lock);

            for(int i = 0; i < numrows; i++)
            {
                for(int j = 0; j < numcols; j++)
                {
                    render_game_screen(Coordinate(i, j), colorize);
                }
            }

            SDL_UnlockMutex(lock);

        }

        void render_sidebar()
        {
            for(int i = numrows; i < numrows + SIDEBAR_OFFSET; i++)
            {
                for(int j = 0; j < numcols; j++)
                {
                    render_sidebar(game_to_screen_coord(i, j));
                }
            }
        }

        void render_sidebar(Coordinate screen_cord)
        {
            render_texture(sidebar, ren, screen_cord, actual_width, row_height);
        }

        Tower * last_tower;

        void handle_mouse_over_at(Coordinate screen_cord)
        {

            Tower * tower = map->get_tower_at(screen_to_game_coord(screen_cord));
            SDL_LockMutex(lock);
            if(tower != nullptr)
            {
                for(int i = 0; i < numrows; i++)
                {
                    for(int j = 0; j < numcols; j++)
                    {
                        if(tower->can_attack(Coordinate(i, j)))
                        {

                            colors[Coordinate(i, j)] = true;
                        }
                    }
                }
            }

            else 
            {
                colors.clear();
            }
            
            SDL_UnlockMutex(lock);

            last_tower = tower;

        }

        static int handle_event(void * ptr)
        {
             return ((GUI*)ptr)->handle_event();
        }

        int handle_event()
        {

            SDL_Event event;

            while(!quit)
            {

                /* Poll for events */
                while(SDL_PollEvent( &event ))
                {
                    
                    switch( event.type )
                    {
                        /* SDL_QUIT event (window close) */
                        case SDL_QUIT:
                            std::cout << "Tried to quit!" << std::endl;
                            quit = true;
                            break;

                        case SDL_MOUSEMOTION:
                            handle_mouse_over_at(Coordinate(event.motion.x, event.motion.y));
                            break;

                        default:
                            break;
                    }

                }

            }

            SDL_Quit();
            return 0;
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
            ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
            return ren != nullptr;
        }

        bool create_textures()
        {

            background = load_texture("grass_night.jpg", ren);
            tile = load_texture("tile2.png", ren);
            sidebar = load_texture("tile.png", ren);
            SDL_Surface * tile_hover_s = SDL_CreateRGBSurface(0, row_width, row_height, 32, 0, 0, 0, 0);
            SDL_FillRect(tile_hover_s, nullptr, SDL_MapRGB(tile_hover_s->format, 255, 0, 0));
            tile_hover = SDL_CreateTextureFromSurface(ren, tile_hover_s);
            loaded_textures[TOWER_HOVER_SURFACE] = tile_hover;
            return background != nullptr || tile != nullptr || tile_hover != nullptr;
        }

        bool create_font()
        {
            if( TTF_Init() == -1 )
            {
                return false;    
            }

            std::string res_path = GRAPHICS_PATH;
            std::string extra = "Lato-Regular.ttf";

            font = TTF_OpenFont((res_path + extra).c_str() , 28 );
            
            if(font == nullptr)
            {
                return false;
            }  

            previous_score = -1;

            return true;
        }

        SDL_Texture * get_font_texture(const std::string &message, SDL_Color color)
        {

            if(loaded_textures.find(message) != loaded_textures.end())
                return loaded_textures[message];

            SDL_Surface * surf = TTF_RenderText_Solid(font, message.c_str(), color);
            if (surf == nullptr)
            {
                logSDLError(std::cout, "TTF_RenderText");
                return nullptr;
            }
            SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);

            if (texture == nullptr)
            {
                logSDLError(std::cout, "CreateTexture");
            }

            loaded_textures[message] = texture;

            return texture;
        }

    public:  

       bool has_quit_game()
        {
            return quit;
        }

        GUI(int rows, int columns, std::vector<Path> &paths, TDMap * map)
        {

            assert(rows > 0 && columns > 0 && paths.size() != 0 && map != nullptr);

            actual_width = DEFAULT_WIDTH/rows;
            //row_width = (rows * actual_width - SIDEBAR_OFFSET * actual_width)/rows; 
            row_width = actual_width;
            row_height = DEFAULT_HEIGHT/columns;
            numrows = rows;
            numcols = columns;
            this->map = map;
            this->paths = paths;
            lock = SDL_CreateMutex();
            /* Doesnt work with short circuiting in an if... wonder why */
            
            bool completed;
            completed = init();
            completed = completed && create_window();
            completed = completed && create_renderer();
            completed = completed && create_textures();
            completed = completed && create_font();


            if (!completed)
            {
                logSDLError(std::cout, "Initialization of components");
                SDL_Quit();
                exit(1);
            } 

            SDL_Thread * event_thread = SDL_CreateThread(handle_event, "hello", this);
            
            fill_screen_tiles(false);
            SDL_RenderPresent(ren); 
        }

        std::vector<Animation> tower_anims;
        std::vector<Animation> sprite_anims;

        // takes pixel coords.
        // pls take care while using.

        void set_up_tower_animation(SDL_Texture * texture, Coordinate from, Coordinate to, bool randomize, std::vector<Coordinate>& s_locations, Coordinate s_size, size_t frame_rate, int direction)
        {
            set_up_animation(tower_anims, texture, from, to, randomize, s_locations, s_size, frame_rate, direction);
        }

        void set_up_sprite_animation(Sprite * sprite, SDL_Texture * texture, Coordinate from, Coordinate to, bool randomize, std::vector<Coordinate>& s_locations, Coordinate s_size, size_t frame_rate, int direction)
        {
            if(from.x < 0 && randomize)
            {
                Coordinate offsets(((rand() % (row_width + 1)) - (row_width)/2)/2, (rand() % (row_height + 1)) - (row_height)/2);
                sprite->set_random_position(offsets);
            }

            Coordinate offsets = sprite->get_offset();
            from.x += offsets.x;
            from.y += offsets.y;
            to.x += offsets.x;
            to.y += offsets.y;

            set_up_animation(sprite_anims, texture, from, to, randomize, s_locations, s_size, frame_rate, direction);
        }

        void set_up_animation(std::vector<Animation>& which, SDL_Texture * texture, Coordinate from, Coordinate to, bool randomize, std::vector<Coordinate>& s_locations, Coordinate s_size, size_t frame_rate, int direction)
        {
            which.push_back(Animation(this, texture, from, from, to, randomize, s_locations, s_size, frame_rate, direction));
            diff_coords.push_back(from);
            diff_coords.push_back(to);
        }

        void clear_animations()
        {
            re_render();
            tower_anims.clear();
            sprite_anims.clear();
            SDL_LockMutex(lock);
            colors.clear();
            SDL_UnlockMutex(lock);

        }

        void show_sprite_animations()
        {
            re_render();
            BOOST_FOREACH(Animation & animation, sprite_anims)
            {
                animation.animate();
            }
        }

        void show_tower_animations()
        {
            BOOST_FOREACH(Animation & animation, tower_anims)
            {
                animation.animate();
            }            
        }

        void Update()
        {
            
            SDL_Color red = {255, 0, 0, 0};
            int score = map->get_number_of_sprites();
            if(score != previous_score)
            {
                render_sidebar();
                SDL_Texture * tex = get_font_texture(std::to_string(score), red);
                Coordinate right_top = game_to_screen_coord(Coordinate(numrows, 0));
                render_texture(tex, ren, right_top);
            }
            previous_score = score;

            SDL_LockMutex(lock);
            for(int i = 0; i < NUM_ROWS; i++)
            {
                for(int j = 0; j < NUM_COLS; j++)
                {
                    BOOST_FOREACH(Sprite * s, map->get_sprites_at(i,j))
                    {
                        SDL_Texture * sprite_texture = load_sprite_texture(s->get_spritesheet()->get_file_name(), s->is_attacked());
                        Coordinate previous_cord = game_to_screen_coord(s->get_previous_position());
                        Coordinate next_cord = game_to_screen_coord(s->get_coordinate());
                        set_up_sprite_animation(s, sprite_texture, previous_cord, next_cord, true, s->get_sscords(), s->get_spritesheet()->get_box_size(), s->get_spritesheet()->get_frame_rate(), -1);                        
                    }

                    Tower * tower = map->get_tower_at(i, j);
                    if(tower != nullptr)
                    {
                        Coordinate current_cord = game_to_screen_coord(Coordinate(i, j));
                        SDL_Texture * texture = load_texture(tower->get_spritesheet()->get_file_name(), ren);
                        BOOST_FOREACH(Coordinate c, tower->get_attack_tiles())
                        {
                            colors[c] = true;
                            Coordinate screen_cord = game_to_screen_coord(c);
                            int xdirection = current_cord.x < screen_cord.x ? 2 : current_cord.x == screen_cord.x ? 1 : 0;
                            int ydirection = current_cord.y < screen_cord.y ? 0 : 1;
                            int direction = xdirection + (tower->get_sscords().size()/tower->get_spritesheet()->get_frame_rate() * ydirection);
                            set_up_tower_animation(texture, current_cord, screen_cord, false, tower->get_sscords(), tower->get_spritesheet()->get_box_size(), tower->get_spritesheet()->get_frame_rate(), direction);
                        }
                    }
                }
            }

            SDL_UnlockMutex(lock);
            for(int i = 0; i < ANIMATION_CONSTANT; i++)
            {
                show_sprite_animations();
                show_tower_animations();
                SDL_RenderPresent(ren);
            }
            SDL_RenderPresent(ren);
            clear_animations();
        }
};
