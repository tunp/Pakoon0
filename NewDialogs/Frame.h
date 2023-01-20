class Frame : public Item {
protected:
	vector<Item *> items;
public:
	Frame(SDL_Rect pos);
	~Frame();
	SDL_Surface *getSurface();
	void draw();
	void addItem(Item *item);
	Item *getItem(int i);
	bool onMousePress(int x, int y);
	void onMouseRelease(int x, int y);
	void onMouseMove(int x, int y);
};
