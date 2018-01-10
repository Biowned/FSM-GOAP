#include "Text.h"

Text::Text(const std::string newText, const Vector2D& newPosition, SDL_Renderer* rendRef, const float& textSize, const bool& ctr) : text(newText),
position(newPosition),
rendererReference(rendRef),
centered(ctr) {
	font = TTF_OpenFont("../res/tt.ttf", textSize); //this opens a font style and sets a size
	if (font == NULL)
	{
		std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
	}
}

Text::~Text() {

}

void Text::RenderText() {

	SDL_Color White = { 255, 255, 255 };

	surface = TTF_RenderText_Solid(font, text.c_str(), White);

	texture = SDL_CreateTextureFromSurface(rendererReference, surface);

	SDL_Rect Message_rect;
	if (centered) {
		Message_rect.x = position.x - surface->w / 2;
		Message_rect.y = position.y - surface->h / 2;
	}
	else {
		Message_rect.x = position.x;
		Message_rect.y = position.y;
	}
	Message_rect.w = surface->w;
	Message_rect.h = surface->h;

	SDL_RenderCopy(rendererReference, texture, NULL, &Message_rect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

}