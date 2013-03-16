class Button : public Item {
private:
	string text;
	SDL_Color color;
	bool pressed;
	bool selected;
	SDL_Surface *button_up;
	SDL_Surface *button_down;
	void (*button_func)(void *p, Button *b);
	void *p;
public:
	//void init();
	//void destroy();
	Button(string text, SDL_Color color, SDL_Rect pos);
	~Button();
	void drawButton();
	void onMousePress(int x, int y);
	void onMouseRelease(int x, int y);
	void setButtonFunc(void (*button_func)(void *p, Button *b), void *p);
	void setText(string text);
	string getText();
	void setColor(SDL_Color color);
	void setSelected(bool selected);
	void selectSurface();
};
