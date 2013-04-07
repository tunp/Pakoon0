class Dialog {
protected:
	SDL_Surface *surface;
	SDL_Surface *background;
	SDL_Rect pos;
	vector<Item *> items;
private:
	bool default_pos;
	bool exit;
public:
	bool loadBackground(string name);
	void addItem(Item *item);
	void deleteItems();
	void draw();
	SDL_Surface *getSurface();
	void deleteSurfaceAndBackground();
	void onMousePress(int x, int y);
	void onMouseRelease(int x, int y);
	void onMouseMove(int x, int y);
	void setPos(SDL_Rect pos);
	SDL_Rect *getPos();
	void setExit(bool exit);
	bool isExit();
	bool isDefaultPos();
	void setDefaultPos(bool default_pos);
};
