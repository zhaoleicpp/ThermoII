#pragma once
class CUserInterface : public CClientDC
{
public:
	static const int m_max_width=1024*4;
	static const int m_max_height=m_max_width*3/4;
	unsigned char m_pixel[m_max_height*m_max_width*3];
	double m_left_x,m_right_x,m_up_y,m_down_y;//mm
	int m_width,m_height;//mm
	void PreparePaint();
	void FinishPaint(CDC &dc=*(CDC*)NULL);
	void SetPixel(int x,int y,unsigned char red,unsigned char green,unsigned char blue,bool do_anyway=false);
	double m_pixel_from_mm;
	void UpdateSize();
	CString m_text;
public:
	static CUserInterface * Create(class CWnd &window);
	void SetPixel(double x,double y,unsigned char red,unsigned char green,unsigned char blue);
	void SetSize(int width,int height);
	void Line(double x0,double y0,double x1,double y1,unsigned char red,unsigned char green,unsigned char blue);
	void Update(bool skip_screen=true);
	void OnPaint(CDC &dc=*(CDC*)NULL);
	double GetPixelPerMm();
	void MmFromPixel(double &x_mm,double &y_mm,int x_pixle,int y_pixel);
	CUserInterface(class CWnd &window);
	~CUserInterface(void);
};
extern CUserInterface *g_user_interface;