class DropDownList : public Item {
private:
	vector<string> list;
	vector<Dialog *> *dialogs;
	int selected;
	int listbox_item_size;
	int listbox_height;
	bool pressed;
public:
	DropDownList(vector<string> list, vector<Dialog *> *dialogs, SDL_Rect pos);
	~DropDownList();
	void drawDropDownList();
	void onMousePress(int x, int y);
	void onMouseRelease(int x, int y);
	vector<string> getList();
	void setSelected(int selected);
	string getString();
	void reset();
	void add(string s);
	bool select(string s);
	int getListboxItemSize();
	int getListboxHeight();
};
