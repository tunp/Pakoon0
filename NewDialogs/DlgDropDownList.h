class DlgDropDownList : public Dialog {
private:
	DropDownList *ddl;
public:
	DlgDropDownList(DropDownList *ddl, SDL_Rect pos, bool add_scrollbar);
	~DlgDropDownList();
	static void listboxValueChangedCallback(void *p);
	void listboxValueChanged();
	static void scrollbarValueChangedCallback(void *p);
	void scrollbarValueChanged();
};
