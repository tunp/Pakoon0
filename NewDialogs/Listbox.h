class Listbox : public Item {
private:
	vector<string> list;
	int selected;
	int item_size;
	void (*value_changed_func)(void *p);
	void *p;
public:
	Listbox(vector<string> list, int item_size, SDL_Rect pos);
	~Listbox();
	void drawListbox();
	bool onMousePress(int x, int y);
	void setValueChangedFunc(void (*value_changed_func)(void *p), void *p);
	int getSelected();
};
