class DlgControls : public Dialog {
private:
	CGetawayView *pView;
	
	BSimulation *m_pSimulation;
	int		m_rbUse;
	
	vector<DropDownList *> lists;
public:
	DlgControls(CGetawayView *pView);
	~DlgControls();
	static void yeahCallback(void *p, Button *b);
	void yeah();
	static void noWayCallback(void *p, Button *b);
	int GetKeyFromString(string rsKey);
	void OnKeyboard();
	void FillAndSelectCombo(int nCombo, DropDownList *ddl, bool bKeyboard, bool bContinuous);
};
