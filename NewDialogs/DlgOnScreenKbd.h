class DlgOnScreenKbd : public Dialog {
private:
	CGetawayView *pView;
  int esc_code;
public:
	DlgOnScreenKbd(CGetawayView *pView);
	~DlgOnScreenKbd();
	bool onMousePress(int x, int y);
	bool onMouseRelease(int x, int y);
	bool onMouseMove(int x, int y);
  bool onFingerDown(int x, int y, int finger_id);
	static void keyPressCallback(void *p, Button *b);
	static void keyReleaseCallback(void *p, Button *b);
	void keyPress(int *key_press);
	void keyRelease(int *key_press);
};
