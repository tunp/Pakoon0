class ButtonGroup : public Frame {
private:
	string value;
	void (*value_changed_func)(void *p, ButtonGroup *bg);
	void *p;
public:
	ButtonGroup(SDL_Rect pos);
	~ButtonGroup();
	void addItem(Button *button);
	static void changeCallback(void *p, Button *b);
	void change(Button *b);
	void setValueChangedFunc(void (*value_changed_func)(void *p, ButtonGroup *bg), void *p);
	string getValue();
};
