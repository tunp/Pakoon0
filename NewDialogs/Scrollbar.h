class Scrollbar : public Item {
private:
	bool vertical;
	int min_value;
	int max_value;
	int value;
	int bar_size;
	int mouse_offset;
	bool pressed;
	void (*value_changed_func)(void *p);
	void *p;
public:
	Scrollbar(int min_value, int max_value, int value, SDL_Rect pos);
	~Scrollbar();
	void drawScrollbar();
	void onMousePress(int x, int y);
	void onMouseRelease(int x, int y);
	void onMouseMove(int x, int y);
	int getBarPos();
	void setValueByBarPos(int pos);
	void setValueChangedFunc(void (*value_changed_func)(void *p), void *p);
	int getValue();
};
