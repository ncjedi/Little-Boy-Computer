#include <GLFW/glfw3.h>
#include <stdio.h>
#include "bus.h"
#include "Cart.h"

GLFWwindow* window;

void drawGrid() {
    int gridSize = 64;
    float size = 2.0f / gridSize;

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            float x = -1.0f + i * size;
            float y = -1.0f + j * size;

            glBegin(GL_QUADS);
            if ((i + j) % 2 == 0) glColor3f(0.8f, 0.8f, 0.8f);
            else glColor3f(0.5f, 0.5f, 0.5f);

            glVertex2f(x, y);
            glVertex2f(x + size, y);
            glVertex2f(x + size, y + size);
            glVertex2f(x, y + size);
            glEnd();
        }
    }
}

void draw_background()
{
    int gridSize = 64;
    float size = 2.0f / gridSize;
    uint16_t tile_counter = 0x0401;
    uint16_t rom_address = 0x0000;
    uint8_t colour = 0x00;
    int8_t scroll_offset_x = getValue(0x0799);
    int8_t scroll_offset_y = getValue(0x0800);

    while (tile_counter < 0x0481)
    {
        rom_address = 0x0000;
        rom_address |= getValue(tile_counter); // get the high bit from the VRAM
        tile_counter++; //increment the tile counter
        rom_address = rom_address << 8; //shift the high bit to it's position
        rom_address |= getValue(tile_counter); // get the low bit from the VRAM
        tile_counter++; //increment the tile counter for the next pass

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int tile_count = (tile_counter - 0x0483)/2; //+2 because the counter already incremented
                colour = getValue(rom_address); //get the colour of the pixel at the rom address
                rom_address++;

                //colour = 0b00011001; //TEST!!!!!!

                float x = (- 1.0f + (i + 8 * (tile_count % 8)) * size); //FOR FUTURE REFRERENCE: the glVertex2f function goes from -1 to +1 with -1 being the far left and +1 being the far right
                float y = (- 1.0f + (j + 8 * (tile_count / 8)) * size); //same as above but for up and down

                //printf("offset: %d number put into x: %f x before: %f\n", scroll_offset_x, (float)scroll_offset_x / 32.0f,x);

                x += (float)scroll_offset_x / 32.0f; //smooth scrolling offset
                y += (float)scroll_offset_y / 32.0f; //smooth scrolling offset

                //printf("offset: %d number put into x: %f x after: %f\n\n", scroll_offset_x, (float)scroll_offset_x / 32.0f, x);

                while (x <= -1)
                {
                    x += 2;
                }
                while (x >= 1)
                {
                    x -= 2;
                }

                while (y <= -1)
                {
                    y += 2;
                }
                while (y >= 1)
                {
                    y -= 2;
                }

                float blue = (float)(colour & 0b00000011) / (float)0b00000011;
                colour = colour >> 2;
                float green = (float)(colour & 0b00000011) / (float)0b00000011;
                colour = colour >> 2;
                float red = (float)(colour & 0b00000011) / (float)0b00000011;

                glBegin(GL_QUADS);
                /*if ((i + j) % 2 == 0) glColor3f(0.8f, 0.8f, 0.8f);
                else glColor3f(0.5f, 0.5f, 0.5f);*/

                glColor3f(red, green, blue);

                glVertex2f(x, y);
                glVertex2f(x + size, y);
                glVertex2f(x + size, y + size);
                glVertex2f(x, y + size);
                glEnd();
            }
        }
    }
}

void GetKeys()
{
    //function keys
    if (glfwGetKey(window, GLFW_KEY_F4))
    {
        exit();
    }
    if (glfwGetKey(window, GLFW_KEY_F1))
    {
        readCart();
    }
}

void GPUtick() {
    glClear(GL_COLOR_BUFFER_BIT);
    //drawGrid();
    /*TEST*/
    for (int i = 0; i < 128; i+=2)
    {
        setValue(0x0401 + i, 0x8F);
        setValue(0x0402 + i, 0xFF);
    }
    setValue(0x0799, 0x40);
    /*END TEST*/
    draw_background();
    glfwSwapBuffers(window);
    glfwPollEvents();
    GetKeys();
}

void GPUinit()
{
    if (!glfwInit()) return -1;
    window = glfwCreateWindow(640, 640, "64x64 Grid", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
}