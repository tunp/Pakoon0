class DlgMainMenu : public Dialog {
private:
	CGetawayView *pView;
public:
	DlgMainMenu(CGetawayView *pView);
	~DlgMainMenu();
	static void visualsCallback(void *p, Button *b);
	void visuals();
	static void soundsCallback(void *p, Button *b);
	void sounds();
	static void controlsCallback(void *p, Button *b);
	void controls();
	static void contCallback(void *p, Button *b);
	void cont();
	static void quitCallback(void *p, Button *b);
	void quit();
};
