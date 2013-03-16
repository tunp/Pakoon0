class DlgSounds : public Dialog {
private:
	CGetawayView *pView;
	int		m_rbSoundSpace;
public:
	DlgSounds(CGetawayView *pView);
	~DlgSounds();
	static void soundSpaceCallback(void *p, ButtonGroup *bg);
	static void yeahCallback(void *p, Button *b);
	void yeah();
	static void noWayCallback(void *p, Button *b);
};
