#include <stdio.h>
#include "SDL.h"
#include "mnist.h"

int main(int argc, char* argv[])
{
	idx_t *mnist_data = mnist_open(argv[1]);
	printf("type = %d, dims = %d\n", mnist_data->type, mnist_data->dims);
	for (int i = 0; i < mnist_data->dims; ++i) {
		printf("dim[%d] = %d\n", i, mnist_data->dim[i]);
	}

	if (-1 == SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("line: %d, %s\n", __LINE__, SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (NULL == win) {
		printf("line: %d, %s\n", __LINE__, SDL_GetError());
		return 1;
	}	
 
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (NULL == ren) {
		printf("line: %d, %s\n", __LINE__, SDL_GetError());
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, mnist_data->dim[2], mnist_data->dim[1]);
	if (NULL == tex) {
		printf("line: %d, %s\n", __LINE__, SDL_GetError());
		return 1;
	}

	uint8_t *pImage = (uint8_t *)calloc(mnist_data->dim[2] * mnist_data->dim[1], 3);

	SDL_Event e;
	int click_cnt = 0;
	int quit = 0;
	SDL_Rect rect = {100, 100, 28, 28};

	while (!quit){
		//Our main loop
		int next = 0;

		//Event polling
		while (SDL_PollEvent(&e)){
			//If user closes he window
			if (e.type == SDL_QUIT)
				quit = 1;
			if (e.type == SDL_MOUSEBUTTONDOWN)
				next = 1;
		}
		
		if (next) {
			next = 0;

			uint8_t *pSrc = mnist_data->data + mnist_data->dim[2] * mnist_data->dim[1] * click_cnt++;
			uint8_t *pDst = pImage;

			for (int i = 0; i < mnist_data->dim[2] * mnist_data->dim[1]; ++i) {
				*pDst++ = *pSrc;
				*pDst++ = *pSrc;
				*pDst++ = *pSrc++;
			}

			if (-1 == SDL_UpdateTexture(tex, NULL, pImage, mnist_data->dim[2] * 3)) {
				printf("line: %d, %s\n", __LINE__, SDL_GetError());
				return 1;
			}

			if (-1 == SDL_RenderCopy(ren, tex, NULL, &rect)) {
				printf("line: %d, %s\n", __LINE__, SDL_GetError());
				return 1;
			}

			SDL_RenderPresent(ren);
		}
	}

	free(pImage);

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	
	mnist_close(mnist_data);
	return 0;
}

