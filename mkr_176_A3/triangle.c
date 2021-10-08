#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "triangle.h"
#include "drawline.h"


#define TRIANGLE_PENCOLOR 0xBBBB0000

/* 
 * Print triangle coordinates along with a message
 */
void print_triangle(triangle_t *triangle, char *msg)
{
    printf("%s: %d,%d - %d,%d - %d,%d\n",
           msg,
           triangle->x1, triangle->y1,
           triangle->x2, triangle->y2,
           triangle->x3, triangle->y3);
}

/*
 * Return 0 if triangle coordinates are outside the surface boundary. 1 otherwise.
 */
int sanity_check_triangle(SDL_Surface *surface, triangle_t *triangle)
{
    if (triangle->sx1 < 0 || triangle->sx1 >= surface->w ||
        triangle->sx2 < 0 || triangle->sx2 >= surface->w ||
        triangle->sx3 < 0 || triangle->sx3 >= surface->w ||
        triangle->sy1 < 0 || triangle->sy1 >= surface->h ||
        triangle->sy2 < 0 || triangle->sy2 >= surface->h ||
        triangle->sy3 < 0 || triangle->sy3 >= surface->h)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
 * Scale triangle, altering the on-screen coordinates(e.g. triangle->sx1)
 */
void scale_triangle(triangle_t *triangle)
{
    triangle->sx1 = triangle->x1 * triangle->scale;
    triangle->sx2 = triangle->x2 * triangle->scale;
    triangle->sx3 = triangle->x3 * triangle->scale;

    triangle->sy1 = triangle->y1 * triangle->scale;
    triangle->sy2 = triangle->y2 * triangle->scale;
    triangle->sy3 = triangle->y3 * triangle->scale;
    // TODO: Replace the code below with code that scales each triangle coordinate.
    // The scaling factor is specified in triangle->scale.
    // Remember that this function MUST write to the on-surface coordinates.
    // Do not alter the model coordinates.
}

/*
 * Move the triangle to the center of the surface,
 * altering the on-screen coordinates(e.g. triangle->sx1)
 */
void translate_triangle(triangle_t *triangle)
{
    const int screen_w = 1024;
    triangle->tx =  screen_w/2;
   

    triangle->sx1 = triangle->sx1 + triangle->tx;
    triangle->sx2 = triangle->sx2 + triangle->tx;
    triangle->sx3 = triangle->sx3 + triangle->tx;

    
    const int screen_h = 768;
    triangle->ty = screen_h/2;

    triangle->sy1 = triangle->sy1 + triangle->ty;
    triangle->sy2 = triangle->sy2 + triangle->ty;
    triangle->sy3 = triangle->sy3 + triangle->ty;

    // TODO: Insert code that moves the triangle on the surface.
    // The translation coordinates are specified in triangle->tx and triangle->ty.
    // Remember to use the on-surface coordinates (triangle->sx1, etc.)
}

/*
 * Calculate the triangle bounding box,
 * altering fields of the triangle's rect(e.g. triangle->rect.x)
 */
void calculate_triangle_bounding_box(triangle_t *triangle)
{
   int left_x, top_y, right_x, bottom_y;

    
    //left_x
    left_x = triangle->sx1;
    if (left_x > triangle->sx2) {
        left_x = triangle->sx2;
    }
    if (left_x > triangle->sx3) {
        left_x = triangle->sx3;
    }
    
    //right_x
    right_x = triangle->sx1;
    if (right_x < triangle->sx2) {
        right_x = triangle->sx2;
    }
    if  (right_x < triangle->sx3) {
        right_x = triangle->sx3;
    }

   

    triangle->rect.w = right_x - left_x;
    triangle->rect.x = left_x;


    
    //top_y
    top_y = triangle->sy1;
    if (top_y > triangle->sy2) {
        top_y = triangle->sy2;
    }
    if (top_y > triangle->sy3) {
        top_y = triangle->sy3;
    }

    //bottom_y
    bottom_y = triangle->sy1;
    if (bottom_y < triangle->sy2) {
        bottom_y = triangle->sy2;
    }
    if (bottom_y < triangle->sy3) {
        bottom_y = triangle->sy3;
    }

    
    triangle->rect.h = bottom_y - top_y;
    triangle->rect.y = top_y;
}

    // TODO: Insert code that calculates the bounding box of a triangle.
    // Remember to use the on-surface coordinates (triangle->sx1, etc.)
    // The bounding box coordinates should be written to
    // triangle->rect.x, triangle->rect.y, triangle->rect.w, triangle->rect.h


/*
 * Fill the triangle on the surface with the triangle's color
 */

 

void fill_triangle(SDL_Surface *surface, triangle_t *triangle)
{
    int pixel = 0;

    for (int i = 0 ;i <= triangle->rect.w; i++)
    {
        for (int j = 0; j <= triangle->rect.h; j++)
        {  
            for (int k = triangle->rect.h; k >= 0; k--)
            {
                if  (get_pixel(surface, triangle->rect.x + i,triangle->rect.y + triangle->rect.h - k) == TRIANGLE_PENCOLOR)
                {
                   pixel = k;
                   continue;
                }   
            }
             if (get_pixel(surface, triangle->rect.x + i, triangle->rect.y + j) == TRIANGLE_PENCOLOR)
            {
                draw_line(surface, triangle->rect.x + i, triangle->rect.y + j, triangle->rect.x + i, triangle->rect.y + triangle->rect.h - pixel, triangle->fillcolor);
            } 
        }
    }   
}
 
    // TODO: Insert code that fills the triangle with the color specified in triangle->fillcolor.
    // Hint: Draw the triangle with color TRIANGLE_PENCOLOR (this color can not
    // occur in e.g. the teapot or the example triangles).  Thus, if your
    // approach to filling the triangle relies on looking for the edges of
    // the triangle on the surface (via the GetPixel function), you will find those
    // edges even if the triangle overlaps with a triangle that has already
    // been drawn on the surface.


/*
 * Draw a filled triangle on the given surface
 */
void draw_triangle(SDL_Surface *surface, triangle_t *triangle)
{
    int isOK;

    /* Scale. */
    scale_triangle(triangle);

    /* Translate. */
    translate_triangle(triangle);

    /* Determine bounding box */
    calculate_triangle_bounding_box(triangle);

    /* Sanity check that triangle is within surface boundaries. */
    isOK = sanity_check_triangle(surface, triangle);
    if (!isOK)
    {
        print_triangle(triangle, "Triangle outside surface boundaries");
        return;
    }

    draw_line(surface, triangle->sx1, triangle->sy1, triangle->sx2, triangle->sy2, TRIANGLE_PENCOLOR);
    draw_line(surface, triangle->sx2, triangle->sy2, triangle->sx3, triangle->sy3, TRIANGLE_PENCOLOR);
    draw_line(surface, triangle->sx3, triangle->sy3, triangle->sx1, triangle->sy1, TRIANGLE_PENCOLOR);
    /* 
     * TODO: Insert calls to draw_line to draw the triangle.
     * Remember to use the on-surface coordinates (triangle->sx1, etc.)
     */

    /* Fill triangle */
    fill_triangle(surface, triangle);
}