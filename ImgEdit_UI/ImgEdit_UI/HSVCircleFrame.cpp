#include "stdafx.h"

#include "HSVCircleFrame.h"

#define CTRL_MARGIN 5
#define H_CIRCLE_WIDTH 16

#define COLOR_CIRCLE_X 35
#define COLOR_CIRCLE_Y 0
#define COLOR_CIRCLE_W 200
#define COLOR_CIRCLE_H 200

//#define FG_COLOR_PANE_X 5
//#define FG_COLOR_PANE_Y 140
#define FG_COLOR_PANE_W 32
#define FG_COLOR_PANE_H 32

//#define BG_COLOR_PANE_X (FG_COLOR_PANE_X + FG_COLOR_PANE_W - 10)
//#define BG_COLOR_PANE_Y (FG_COLOR_PANE_Y + FG_COLOR_PANE_H - 10)
#define BG_COLOR_PANE_W 32
#define BG_COLOR_PANE_H 32

#define COLOR_PANE_LAP 10

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern ImgEdit g_ImgEdit;


CHSVCircleFrame::CHSVCircleFrame()
{
	m_Select_H = 0.0;
	m_Select_S = 0.0;
	m_Select_V = 0.0;

	m_isHDrag = false;
	m_isSVDrag = false;
}

CHSVCircleFrame::~CHSVCircleFrame()
{
	_ASSERTE(_CrtCheckMemory());
}

//////////////////////////////
/*
	�I��F���ς�����Ƃ��ɌĂяo���B
	@param[in] color �ύX��̐F
*/
void CHSVCircleFrame::OnChangeSelectColor(IEColor color)
{
	if(m_SelectColor.value != color.value){
		ucvRGB2HSV(color, &m_Select_H, &m_Select_S, &m_Select_V);
		RedrawWindow();
	}
}

BEGIN_MESSAGE_MAP(CHSVCircleFrame, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

int CHSVCircleFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (0 != CWnd::OnCreate(lpCreateStruct)) {
		return -1;
	}

	//init select color
	OnChangeSelectColor(g_ImgEdit.GetSelectFGColor());

	//register event listener
	g_ImgEdit.AddEventListener( this );

	return 0;
}

void CHSVCircleFrame::OnDestroy()
{
	//unregister event listener
	g_ImgEdit.DeleteEventListener( this );

	CWnd::OnDestroy();
}

void CHSVCircleFrame::OnSize(UINT nType, int cx, int cy)
{
	RECT rc;
	this->GetClientRect(&rc);

	int client_width = rc.right - rc.left;
	int client_height = rc.bottom - rc.top;
	int circle_size = min(client_width, client_height) - 2*CTRL_MARGIN;

	if (circle_size <= 0) {
		m_FGColorPanelRect.top = 0;
		m_FGColorPanelRect.left = 0;
		m_FGColorPanelRect.bottom = 0;
		m_FGColorPanelRect.right = 0;

		m_BGColorPanelRect.top = 0;
		m_BGColorPanelRect.left = 0;
		m_BGColorPanelRect.bottom = 0;
		m_BGColorPanelRect.right = 0;

		m_ColorCircleRect.top = 0;
		m_ColorCircleRect.left = 0;
		m_ColorCircleRect.bottom = 0;
		m_ColorCircleRect.right = 0;
	} else {
		RECT ctrl_rc;
		ctrl_rc.left = (client_width - circle_size) / 2;
		ctrl_rc.right = ctrl_rc.left + circle_size;
		ctrl_rc.top = (client_height - circle_size) / 2;
		ctrl_rc.bottom = ctrl_rc.top + circle_size;

		m_BGColorPanelRect.bottom = client_height - CTRL_MARGIN;
		m_BGColorPanelRect.top = m_BGColorPanelRect.bottom - BG_COLOR_PANE_H;

		m_FGColorPanelRect.left = CTRL_MARGIN;
		m_FGColorPanelRect.right = m_FGColorPanelRect.left + FG_COLOR_PANE_W;

		m_BGColorPanelRect.left = m_FGColorPanelRect.right - COLOR_PANE_LAP;
		m_BGColorPanelRect.right = m_BGColorPanelRect.left + BG_COLOR_PANE_W;

		m_FGColorPanelRect.bottom = m_BGColorPanelRect.top + COLOR_PANE_LAP;
		m_FGColorPanelRect.top = m_FGColorPanelRect.bottom - FG_COLOR_PANE_H;

		m_ColorCircleRect = ctrl_rc;
	}

	CWnd::OnSize(nType, cx, cy);
}

void CHSVCircleFrame::OnPaint()
{
	TRY
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		CRect rectWnd;
		GetWindowRect(rectWnd);
		ScreenToClient(rectWnd);
		int frame_width = rectWnd.right - rectWnd.left;
		int frame_height = rectWnd.bottom - rectWnd.top;

		CDC* pDC = CDC::FromHandle( dc.GetSafeHdc() );
		//create back buffer
		CDC memDC;
		CBitmap memBmp;
		memDC.CreateCompatibleDC( pDC );
		memBmp.CreateCompatibleBitmap(pDC, frame_width, frame_height);

		CBitmap* memOldBmp = memDC.SelectObject( &memBmp );

		//clear window rect
		rectWnd.InflateRect(1, 1);
		memDC.FillSolidRect(rectWnd, ::GetSysColor(COLOR_WINDOW));

		int height = m_ColorCircleRect.bottom - m_ColorCircleRect.top;
		int width = m_ColorCircleRect.right - m_ColorCircleRect.left;
		int center_x;
		int center_y;
		center_y = height/2;
		center_x = width/2;
		int radius = GetCircleRadius();

		double sinPI_4 = cos(CV_PI/4.0);
		int beginSV_y;
		int beginSV_x;
		int endSV_y;
		int endSV_x;
		beginSV_x = beginSV_y = center_x - (radius - H_CIRCLE_WIDTH - 4)*sinPI_4;
		endSV_x   = endSV_y   = center_x + (radius - H_CIRCLE_WIDTH - 4)*sinPI_4;

		int fg_pane_w = m_FGColorPanelRect.right - m_FGColorPanelRect.left;
		int fg_pane_h = m_FGColorPanelRect.bottom - m_FGColorPanelRect.top;
		int bg_pane_w = m_BGColorPanelRect.right - m_BGColorPanelRect.left;
		int bg_pane_h = m_BGColorPanelRect.bottom - m_BGColorPanelRect.top;

		CvSize size;
		size.width = width; size.height = height;
		IplImage* Img = cvCreateImage(size, IPL_DEPTH_8U, 4);
		IplImage* PrevColor = cvCreateImage(
			cvSize(fg_pane_w, fg_pane_h),
			IPL_DEPTH_8U,
			4);
		IplImage* PrevBGColor = cvCreateImage(
			cvSize(bg_pane_w, bg_pane_h),
			IPL_DEPTH_8U,
			4);

		cvSet(Img, cvScalar(255,255,255,255));

		//�v���r���[�l�p�`
		UCvPixel pixel = g_ImgEdit.GetSelectFGColor();

		RECT rc;
		rc.left = 0; rc.right = PrevColor->width;
		rc.top = 0; rc.bottom = PrevColor->height;
		CheckerImageRect(PrevColor, &ucvPixel(100,100,100,255), &ucvPixel(255,255,255,255), 3, &rc);
		FillAlphaBlendColor(PrevColor,  0, 0, PrevColor->width, PrevColor->height, &pixel);
		AddRectToImage(PrevColor, 2, &ucvPixel(255,255,255,255));
		AddRectToImage(PrevColor, 1, &ucvPixel(100,100,100, 255));
		//
		pixel = g_ImgEdit.GetSelectBGColor();
		rc.left = 0; rc.right = PrevBGColor->width;
		rc.top = 0; rc.bottom = PrevBGColor->height;
		CheckerImageRect(PrevBGColor, &ucvPixel(100,100,100,255), &ucvPixel(255,255,255,255), 3, &rc);
		FillAlphaBlendColor(PrevBGColor,  0, 0, PrevColor->width, PrevColor->height, &pixel);
		AddRectToImage(PrevBGColor, 2, &ucvPixel(255,255,255,255));
		AddRectToImage(PrevBGColor, 1, &ucvPixel(200,200,200,255));

		//H�~
		DrawHCircle(Img, center_x, center_y, radius, H_CIRCLE_WIDTH);
		//SV�l�p�`
		DrawSVRect(Img, beginSV_x, beginSV_y, endSV_x - beginSV_x, endSV_y - beginSV_y);

		int circle_center_x;
		int circle_center_y;

		//�I���~ H�~��
		{
			UCvPixel pixel;
			circle_center_x = -(radius - H_CIRCLE_WIDTH/2)*cos((m_Select_H/360.0)*2.0*CV_PI) + center_x;
			circle_center_y = (radius - H_CIRCLE_WIDTH/2)*sin((m_Select_H/360.0)*2.0*CV_PI) + center_y;
			DrawPoint(Img, circle_center_x, circle_center_y, 5, 2);
		}

		//�I���~ SV�l�p�`��
		{
			UCvPixel pixel;
			circle_center_x = beginSV_x + (endSV_x - beginSV_x)*m_Select_S;
			circle_center_y = endSV_y - (endSV_y - beginSV_y)*m_Select_V;
			DrawPoint(Img, circle_center_x, circle_center_y, 5, 2);
		}

		//IplImage��`�悷��f�o�C�X�R���e�L�X�g�ɓ]��
		IPLImageToDevice(
			memDC.GetSafeHdc(),
			m_ColorCircleRect.left,
			m_ColorCircleRect.top,
			width,
			height,
			Img,
			0, 0);
		IPLImageToDevice(
			memDC.GetSafeHdc(),
			m_BGColorPanelRect.left,
			m_BGColorPanelRect.top,
			bg_pane_w,
			bg_pane_h,
			PrevBGColor,
			0, 0);
		IPLImageToDevice(
			memDC.GetSafeHdc(),
			m_FGColorPanelRect.left,
			m_FGColorPanelRect.top,
			fg_pane_w,
			fg_pane_h,
			PrevColor,
			0, 0);

		//blt back buffer to display
		BitBlt(dc.GetSafeHdc(), 0, 0, frame_width, frame_height,
			memDC.GetSafeHdc(), 0, 0, SRCCOPY);

		memDC.SelectObject( memOldBmp );
		memBmp.DeleteObject();

		//���\�[�X�J��
		cvReleaseImage(&Img);
		cvReleaseImage(&PrevColor);
		cvReleaseImage(&PrevBGColor);
	}
	CATCH(CException, e)
	{
		::OutputDebugString("CHSVCircleFrame::OnPaint() exception\n");
	}
	END_CATCH
}

void CHSVCircleFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	TRY
	{
		::SetCapture( this->GetSafeHwnd() );

		bool isChangeColor = false;
		double h;
		double s, v;

		//�F�I���~�Ƀ}�E�X������Ă��邩����
		if ((h = isOnColorCircle(point)) != -1.0) {//����Ă���
			m_Select_H = (h/(2.0*CV_PI))*360;
			m_isHDrag = true;
			isChangeColor = true;
		}
		//SV�I���l�p�Ƀ}�E�X������Ă��邩����
		else if (isOnColorRect(point, &s, &v)) {
			m_Select_S = s;
			m_Select_V = v;
			m_isSVDrag = true;
			isChangeColor = true;
		}
		else if (isInRect(point.x, point.y, &m_BGColorPanelRect)) {
			g_ImgEdit.ExchangeColorFGForBG();
		}

		if (isChangeColor) {
			::InvalidateRect(m_hWnd, NULL, FALSE);

			IEColor color;
			color = ucvHSV2RGB(m_Select_H, m_Select_S, m_Select_V);

			//�C�x���g�����b�N
			LockImgEditEvent();
			//�I��F��ύX����B
			g_ImgEdit.SetSelectFGColor(color);
			UnlockImgEditEvent();
			return;
		}
		
		CWnd::OnLButtonDown(nFlags, point);
	}
	CATCH(CException, e)
	{
		::OutputDebugString("CHSVCircleFrame::OnLButtonDown() exception\n");
	}
	END_CATCH
}

void CHSVCircleFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	TRY
	{
		bool isChangeColor = false;
		if(m_isHDrag){
			//�}�E�X�̒��S����̊p�x������h��ύX
			CPoint centerPt;
			GetCircleCenterPos(centerPt);

			double dis_x = point.x - centerPt.x;
			double dis_y = centerPt.y - point.y;
			double theata = atan2(dis_y, dis_x) + CV_PI;

			m_Select_H = (theata/(2.0*CV_PI))*360;
			isChangeColor = true;
		}
		else if(m_isSVDrag){
			//SV�I���l�p�Ƀ}�E�X������Ă��邩�画��
			CPoint beginSV;
			CPoint endSV;

			GetBeginSVRectPos(beginSV);
			GetEndSVRectPos(endSV);

			double dis_x = point.x - m_ColorCircleRect.left;
			double dis_y = point.y - m_ColorCircleRect.top;

			int x = dis_x - beginSV.x;
			int y = endSV.y - dis_y;

			if(x < 0) x = 0;
			if(y < 0) y = 0;
			if(x > (endSV.x - beginSV.x)) x = (endSV.x - beginSV.x)-1;
			if(y > (endSV.y - beginSV.y)) y = (endSV.y - beginSV.y)-1;
				
			m_Select_S = (double)x/(endSV.x - beginSV.x);
			m_Select_V = (double)y/(endSV.y - beginSV.y);
			isChangeColor = true;
		}

		if(isChangeColor){
			::InvalidateRect(m_hWnd, NULL, FALSE);

			IEColor color;
			color = ucvHSV2RGB(m_Select_H, m_Select_S, m_Select_V);

			//�C�x���g�����b�N
			LockImgEditEvent();
			//�I��F��ύX����B
			g_ImgEdit.SetSelectFGColor(color);
			UnlockImgEditEvent();
		}

		CWnd::OnMouseMove(nFlags, point);
	}
	CATCH(CException, e)
	{
		::OutputDebugString("CHSVCircleFrame::OnMouseMove() exception\n");
	}
	END_CATCH
}

void CHSVCircleFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	TRY
	{
		::ReleaseCapture();

		bool isChangeColor = false;
		if(m_isHDrag){
			//�F�I���~�Ƀ}�E�X������Ă��邩����
			double h;
			if((h = isOnColorCircle(point)) != -1.0){//����Ă���
				m_Select_H = (h/(2.0*CV_PI))*360;
				isChangeColor = true;
			}
			m_isHDrag = false;
		}
		else if(m_isSVDrag){
			//SV�I���l�p�Ƀ}�E�X������Ă��邩�画��
			double s, v;
			if(isOnColorRect(point, &s, &v)){
				m_Select_S = s;
				m_Select_V = v;
				isChangeColor = true;
			}
			m_isSVDrag = false;	
		}

		if(isChangeColor){
			::InvalidateRect(m_hWnd, NULL, FALSE);

			IEColor color;
			color = ucvHSV2RGB(m_Select_H, m_Select_S, m_Select_V);

			//�C�x���g�����b�N
			LockImgEditEvent();
			
			//�I��F��ύX����B
			g_ImgEdit.SetSelectFGColor(color);
			
			UnlockImgEditEvent();
		}

		CWnd::OnLButtonUp(nFlags, point);
	}
	CATCH(CException, e)
	{
		::OutputDebugString("CHSVCircleFrame::OnLButtonUp() exception\n");
	}
	END_CATCH
}

//H�T�[�N����`��
void CHSVCircleFrame::DrawHCircle(
	IplImage* dst,
	int center_x,
	int center_y,
	int radius,
	int circle_width)
{
	assert(dst);
	if (radius <= 0) {
		return;
	}

	double aa_d = 1.0;
	double aa_min_d = radius - circle_width - aa_d;
	double aa_max_d = radius + aa_d;
	double aa_min_dd = aa_min_d * aa_min_d;
	double aa_max_dd = aa_max_d * aa_max_d;
	double max_r = radius;
	double min_r = (radius - circle_width);
	double max_rr = max_r * max_r;
	double min_rr = min_r * min_r;

	int start_x = (int)(center_x - aa_max_d);
	int start_y = (int)(center_y - aa_max_d);
	int end_x = (int)(center_x + aa_max_d);
	int end_y = (int)(center_y + aa_max_d);

	assert(start_x >= 0);
	assert(start_y >= 0);
	assert(end_x < dst->width);
	assert(end_y < dst->height);

	int x,y;
	double h_;
	UCvPixel pix;
	UCvPixel* dst_pix;
	for(y=start_y; y<=end_y; y++){
		dst_pix = GetPixelAddress32(dst, start_x, y);
		for(x=start_x; x<=end_x; x++){
			double frr = (double)(x-center_x)*(x-center_x)+(y-center_y)*(y-center_y);
			if((min_rr <= frr) && (frr <= max_rr)){
				double pi = atan2((double)(center_y-y), (double)(x-center_x));
				h_ = 360.0 * ((pi + CV_PI)/(2.0*CV_PI));
				pix = ucvHSV2RGB(h_, 1.0, 1.0);
				dst_pix->value = pix.value;
			}
			else{
				if((aa_min_dd <= frr) &&  (frr < min_rr)){
					double pi = atan2((double)(center_y-y), (double)(x-center_x));
					h_ = 360.0 * ((pi + CV_PI)/(2.0*CV_PI));
					pix = ucvHSV2RGB(h_, 1.0, 1.0);

					double fr = sqrt(frr);
					double tmp = (fr - aa_min_d)/(min_r - aa_min_d);
					pix.a = pix.a * (tmp*tmp) * (3.0 - 2.0*tmp);
					AlphaBlendPixel(dst_pix, &pix, 255, dst_pix);
				}
				else if((max_rr < frr) && (frr <= aa_max_dd)){
					double pi = atan2((double)(center_y-y), (double)(x-center_x));
					h_ = 360.0 * ((pi + CV_PI)/(2.0*CV_PI));
					pix = ucvHSV2RGB(h_, 1.0, 1.0);

					double fr = sqrt(frr);
					double tmp = 1.0 - (fr - max_r)/(aa_max_d - max_r);
					pix.a = pix.a * (tmp*tmp) * (3.0 - 2.0*tmp);
					AlphaBlendPixel(dst_pix, &pix, 255, dst_pix);
				}
			}

			dst_pix++;
		}
	}
}

void CHSVCircleFrame::DrawSVRect(
	IplImage *dst,
	int start_x,
	int start_y,
	int width,
	int height)
{
	UCvPixel pixel;
	UCvPixel* dst_pixel;
	for(int i=0; i<height; i++){
		dst_pixel = GetPixelAddress32(dst, start_x, i+start_y);
		double v = 1.0 - (double)i/height;
		for(int j=0; j<width; j++){
			double s = (double)j/width;
			pixel = ucvHSV2RGB(m_Select_H, s, v);
			dst_pixel->value = pixel.value;
			dst_pixel++;
		}
	}
}

void CHSVCircleFrame::DrawPoint(
	IplImage* dst,
	int center_x,
	int center_y,
	int radius,
	int circle_width)
{
	assert(dst);
	assert(radius > 0);

	double aa_d = 1.0;
	double aa_min_d = radius - circle_width - aa_d;
	double aa_max_d = radius + aa_d;
	double aa_min_dd = aa_min_d * aa_min_d;
	double aa_max_dd = aa_max_d * aa_max_d;
	double max_r = radius;
	double min_r = (radius - circle_width);
	double max_rr = max_r * max_r;
	double min_rr = min_r * min_r;

	int start_x = (int)(center_x - aa_max_d);
	int start_y = (int)(center_y - aa_max_d);
	int end_x = (int)(center_x + aa_max_d);
	int end_y = (int)(center_y + aa_max_d);

	//clipping
	if (start_x < 0) {
		start_x = 0;
	}
	if (start_y < 0) {
		start_y = 0;
	}
	if (end_x < 0) {
		end_x = 0;
	}
	if (end_y < 0) {
		end_y = 0;
	}

	if (start_x >= dst->width) {
		start_x = dst->width - 1;
	}
	if (start_y >= dst->height) {
		start_y = dst->height - 1;
	}
	if (end_x >= dst->width) {
		end_x = dst->width - 1;
	}
	if (end_y >= dst->height) {
		end_y = dst->height - 1;
	}

	//
	if (start_x >= end_x) {
		return;
	}
	if (start_y >= end_y) {
		return;
	}

	int x,y;
	double h_;
	UCvPixel pix;
	UCvPixel* dst_pix;
	for(y=start_y; y<=end_y; y++){
		dst_pix = GetPixelAddress32(dst, start_x, y);
		for(x=start_x; x<=end_x; x++){
			double frr = (double)(x-center_x)*(x-center_x)+(y-center_y)*(y-center_y);
			if((min_rr <= frr) && (frr <= max_rr)){
				dst_pix->b =~ dst_pix->b;
				dst_pix->g =~ dst_pix->g;
				dst_pix->r =~ dst_pix->r;
			}
			else{
				if((aa_min_dd <= frr) &&  (frr < min_rr)){
					pix.b =~ dst_pix->b;
					pix.g =~ dst_pix->g;
					pix.r =~ dst_pix->r;

					double fr = sqrt(frr);
					double tmp = (fr - aa_min_d)/(min_r - aa_min_d);
					pix.a = dst_pix->a * (tmp*tmp) * (3.0 - 2.0*tmp);
					AlphaBlendPixel(dst_pix, &pix, 255, dst_pix);
				}
				else if((max_rr < frr) && (frr <= aa_max_dd)){
					pix.b =~ dst_pix->b;
					pix.g =~ dst_pix->g;
					pix.r =~ dst_pix->r;

					double fr = sqrt(frr);
					double tmp = 1.0 - (fr - max_r)/(aa_max_d - max_r);
					pix.a = dst_pix->a * (tmp*tmp) * (3.0 - 2.0*tmp);
					AlphaBlendPixel(dst_pix, &pix, 255, dst_pix);
				}
			}

			dst_pix++;
		}
	}
}

///////////////////////////////////////////
/*!
	�F�I���~�̒��S�̈ʒu��Ԃ��B
	@param[out] point ���S�̃N���C�A���g���W
*/
void CHSVCircleFrame::GetCircleCenterPos(CPoint& point)
{
	int height = m_ColorCircleRect.bottom - m_ColorCircleRect.top;
	int width = m_ColorCircleRect.right - m_ColorCircleRect.left;
	point.x = width/2 + m_ColorCircleRect.left;
	point.y = height/2 + m_ColorCircleRect.top;
}

//////////////////////////////////
/*!
	�F�I���~�̔��a��Ԃ��B
*/
double CHSVCircleFrame::GetCircleRadius()
{
	int height = m_ColorCircleRect.bottom - m_ColorCircleRect.top;
	int width = m_ColorCircleRect.right - m_ColorCircleRect.left;
	
	if(height < width){
		return height/2.0 - 2;
	}
	else{
		return width/2.0 - 2;
	}
}

//////////////////////////////////
/*!
	SV�l�p�`�̊J�n�ʒu��Ԃ��B
	@param[out] point
*/
void CHSVCircleFrame::GetBeginSVRectPos(CPoint& point)
{
	int height = m_ColorCircleRect.bottom - m_ColorCircleRect.top;
	int width = m_ColorCircleRect.right - m_ColorCircleRect.left;
	int center_x;
	int center_y;
	center_y = height/2;
	center_x = width/2;
	int radius = GetCircleRadius();

	double sinPI_4 = cos(CV_PI/4.0);
	point.x = point.y = center_x - (radius - H_CIRCLE_WIDTH - 4)*sinPI_4;
}

//////////////////////////////////
/*!
	SV�l�p�`�̏I���ʒu��Ԃ��B
	@param[out] point
*/
void CHSVCircleFrame::GetEndSVRectPos(CPoint& point)
{
	int height = m_ColorCircleRect.bottom - m_ColorCircleRect.top;
	int width = m_ColorCircleRect.right - m_ColorCircleRect.left;
	int center_x;
	int center_y;
	center_y = height/2;
	center_x = width/2;
	int radius = GetCircleRadius();

	double sinPI_4 = cos(CV_PI/4.0);
	point.x = point.y = center_x + (radius - H_CIRCLE_WIDTH - 1)*sinPI_4;
}

//////////////////////////////////
/*!
	lpt���F�I���~�ɏ���Ă��邩���肷��B
	@return ����Ă�����true �����łȂ��Ȃ�false
*/
double CHSVCircleFrame::isOnColorCircle(CPoint point)
{
	CPoint centerPt;
	GetCircleCenterPos(centerPt);

	double dis_x = point.x - centerPt.x;
	double dis_y = centerPt.y - point.y;
	double theata = atan2(dis_y, dis_x) + CV_PI;

	double dis = sqrt((double)(point.x - centerPt.x)*(point.x - centerPt.x) +
						(point.y - centerPt.y)*(point.y - centerPt.y));
	double radius = GetCircleRadius();
	
	if(radius - H_CIRCLE_WIDTH <= dis && dis <= radius){
		return theata;
	}

	return -1.0;
}

///////////////////////////////////
/*!
	lpt��SV�I���l�p�ɏ���Ă��邩���肷��B
	@param[in] lpt �}�E�X�N���C�A���g���W
	@param[out] s �I�����ꂽs�l
	@param[out] v �I�����ꂽv�l
	@return ����Ă��邽��true �����łȂ��Ȃ�false��Ԃ��B
*/
double CHSVCircleFrame::isOnColorRect(CPoint point, double *s, double *v)
{
	CPoint beginSV;
	CPoint endSV;

	GetBeginSVRectPos(beginSV);
	GetEndSVRectPos(endSV);

	double dis_x = point.x - m_ColorCircleRect.left;
	double dis_y = point.y - m_ColorCircleRect.top;

	if((beginSV.x <= dis_x && dis_x <= endSV.x)&&
		(beginSV.y <= dis_y && dis_y <= endSV.y)){
			int x = dis_x - beginSV.x;
			int y = endSV.y - dis_y;
			
			*s = (double)x/(endSV.x - beginSV.x);
			*v = (double)y/(endSV.y - beginSV.y);
			return true;
	}

	return false;
}