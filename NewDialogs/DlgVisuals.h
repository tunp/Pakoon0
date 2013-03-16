class DlgVisuals : public Dialog {
private:
	CGetawayView *pView;
	
	BSimulation *m_pSimulation;
	int          m_nOrigScreenMode;
	int          m_nOrigColorMode;
	bool         m_bOrigMipMap;
	
	int		m_rbSkyDetail;
	int		m_rbDistantDetail;
	int		m_rbColor;
	int		m_rbWaterDetail;
	int		m_rbScreenFormat;
	int		m_rbTextureSmoothness;
public:
	DlgVisuals(CGetawayView *pView);
	~DlgVisuals();
	static void skyChangeCallback(void *p, ButtonGroup *bg);
	static void distantChangeCallback(void *p, ButtonGroup *bg);
	static void colorChangeCallback(void *p, ButtonGroup *bg);
	static void screenFormatChangeCallback(void *p, ButtonGroup *bg);
	static void waterDetailChangeCallback(void *p, ButtonGroup *bg);
	static void textureSmoothnessChangeCallback(void *p, ButtonGroup *bg);
	static void yeahCallback(void *p, Button *b);
	void yeah();
	static void noWayCallback(void *p, Button *b);
	void parseResolution(string s, int &w, int &h);
};
