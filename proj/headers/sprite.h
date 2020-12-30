/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>

/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/** A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits. 
 */
typedef struct {
  int32_t x,y;             /**< current sprite position */
  uint16_t width, height;   /**< sprite dimensions */
  int32_t xspeed, yspeed;  /**< current speeds in the x and y direction */
  uint8_t *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/** An Animated Sprite is a "sub-classing" of Sprites
 *  where each Sprite is manipulated using Sprite functions
 */
typedef struct {
	Sprite *sp;		///< pointer to first Sprite, the one with overall properties
	uint8_t aspeed;		///< no. frames per pixmap 
	uint8_t cur_aspeed; // no. frames left to next change
	uint8_t num_fig; 	///< number of pixmaps 
	uint8_t cur_fig; 	///< current pixmap 
	uint8_t **map;     ///< pointer to array of each AnimSprite pixmaps
} AnimSprite;

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite * create_sprite(xpm_map_t xpm, int32_t x, int32_t y, int8_t x_speed, int8_t y_speed);

/** Animate the sprite "fig" according to its attributes in memory,
 * whose address is "base".  The animation detects the screen borders
 * and change the speed according; it also detects collision with
 * other objects, including the player pad. Collisions with the screen
 * border generates a perfect reflection, while collision with other
 * objects generates a random perturbation in the object speed. Other
 * strategies can be devised: use quasi-elastic collision based on the
 * objects "mass" and speed, generate spin effect based on amount of
 * tangential speed direction relative to the object center of
 * "mass"...  Returns the kind of collision detected, RIGHT_HIT or
 * LEFT_HIT, if a collision with the players pad (WHITE colored!) is
 * detected.
 */
int animate_sprite(Sprite* sprite);

/** The "fig" sprite is erased from memory whose address is "base"
 * and used resources released.
 */
void destroy_sprite(Sprite *sprite);

int set_sprite(Sprite* sprite, uint16_t x, uint16_t y, int32_t x_speed, int32_t y_speed);

void draw_sprite(Sprite* sprite);

bool check_collison(Sprite* sprite, int32_t x, int32_t y);


/** Create an Animated Sprite from multiple pixmaps
*   At least one pixmap must be specified.
*/
AnimSprite * create_asprite(int32_t x, int32_t y, int8_t x_speed, int8_t y_speed, uint8_t aspeed, uint8_t no_xpm, xpm_map_t xpm, ...);

/** Animate an Animated Sprite
*/
void animate_asprite(AnimSprite *asp);

void set_asprite(AnimSprite* asp, uint8_t aspeed, uint8_t cur_aspeed, uint8_t num_fig);

void set_asprite_sprite(AnimSprite* asp, uint16_t x, uint16_t y, int32_t x_speed, int32_t y_speed);

/** Destroy an Animated Sprite from video memoty and
* release all resources allocated.
*/
void destroy_asprite(AnimSprite *asp);

void draw_asprite(AnimSprite* asp);

bool check_asp_collison(AnimSprite* asp, int32_t x, int32_t y);


/** @} end of sprite */

#endif
