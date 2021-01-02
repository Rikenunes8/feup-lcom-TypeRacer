#ifndef _SPRITE_H_
#define _SPRITE_H_


/** @defgroup sprite Sprite
 * @{
 *
 * Sprite and animated sprites related functions, variables and data structures
 */

/** 
 * @brief A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.
 */
typedef struct {
  int32_t x,y;             /**< current sprite position */
  uint16_t width, height;   /**< sprite dimensions */
  int32_t xspeed, yspeed;  /**< current speeds in the x and y direction */
  uint8_t *map;           /**< the sprite pixmap (use xpm_load()) */
} Sprite;

/**
 * @brief An Animated Sprite is a "sub-classing" of Sprites
 *  where each Sprite is manipulated using Sprite functions
 */
typedef struct {
	Sprite *sp;		      //!< pointer to first Sprite, the one with overall properties
	uint8_t aspeed;		  //!< no. frames per pixmap 
	uint8_t cur_aspeed; //! no. frames left to next change
	uint8_t num_fig; 	  //!< number of pixmaps 
	uint8_t cur_fig; 	  //!< current pixmap 
	uint8_t **map;      //!< pointer to array of each AnimSprite pixmaps
} AnimSprite;


/**
 * @brief Creates a sprite
 * 
 * @param xpm XPM to be load
 * @param x X position
 * @param y Y position
 * @param x_speed X speed
 * @param y_speed Y speed
 * @return Returns NULL on invalid pixmap.
 */
Sprite * create_sprite(xpm_map_t xpm, int32_t x, int32_t y, int8_t x_speed, int8_t y_speed);

/** 
 * @brief Updates sprite according to its attributes
 * @param sprite Sprite to update
 */
void animate_sprite(Sprite* sprite);

/** 
 * @brief Frees memory alocated by sprite
 */
void destroy_sprite(Sprite *sprite);
/**
 * @brief Sets the sprite
 * 
 * @param sprite Sprite to set
 * @param x New x position
 * @param y New y position
 * @param x_speed New x speed
 * @param y_speed New y speed
 */
void set_sprite(Sprite* sprite, uint16_t x, uint16_t y, int32_t x_speed, int32_t y_speed);
/**
 * @brief Draws sprite
 * 
 * @param sprite Sprite to be drawn
 */
void draw_sprite(Sprite* sprite);
/**
 * @brief Checks if a position is inside the sprite
 * 
 * @param sprite Sprite
 * @param x x coordinate
 * @param y y coordinate
 * @return Return true if colision, false otherwise 
 */
bool check_collison(Sprite* sprite, int32_t x, int32_t y);


/**
 * @brief Creates an Animated Sprite from multiple pixmaps
 * At least one pixmap must be specified.
 * 
 * @param x X position
 * @param y Y position
 * @param x_speed X speed
 * @param y_speed Y speed
 * @param aspeed Number of frames to change figure
 * @param no_xpm Number of figures
 * @param xpm First figure
 * @param ... More figures
 * @return Returns NULL on invalid pixmap.
 */
AnimSprite * create_asprite(int32_t x, int32_t y, int8_t x_speed, int8_t y_speed, uint8_t aspeed, uint8_t no_xpm, xpm_map_t xpm, ...);

/** 
 * @brief Updates an Animated Sprite according to its atributtes
 * @param asp Animated sprite to be animated
 */
void animate_asprite(AnimSprite *asp);
/**
 * @brief Sets the animated sprite
 * 
 * @param asp Animated sprite to be set
 * @param aspeed New number of frames until change to next figure
 * @param cur_aspeed New current frames left to next figure
 * @param num_fig New number of figures
 */
void set_asprite(AnimSprite* asp, uint8_t aspeed, uint8_t cur_aspeed, uint8_t num_fig);
/**
 * @brief Sest the sprite of animated sprite
 * 
 * @param asp Animated sprite to be set
 * @param x New x position
 * @param y new y position
 * @param x_speed New x speed
 * @param y_speed New y speed
 */
void set_asprite_sprite(AnimSprite* asp, uint16_t x, uint16_t y, int32_t x_speed, int32_t y_speed);

/** 
 * @brief Destroys an Animated Sprite, releases all resources allocated.
 * @param asp Animated sprite
 */
void destroy_asprite(AnimSprite *asp);
/**
 * @brief Draws animated sprite
 * 
 * @param asp Anmated sprite to be drawn
 */
void draw_asprite(AnimSprite* asp);
/**
 * @brief Checks if a position is inside the animated sprite
 * 
 * @param asp Animated sprite
 * @param x x coordinate
 * @param y y coordinate
 * @return Return true if colision, false otherwise 
 */
bool check_asp_collison(AnimSprite* asp, int32_t x, int32_t y);


/** @} end of sprite */

#endif
