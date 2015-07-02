#include "StdAfx.h"
#include "HalconAction.h"
#include "DetectCircularhole.h"
#include "DataUtility.h"


#define MINGRAY			_T("MinGray")
#define	MAXGRAY		     _T("MaxGray")

#define	MINCIRLEAREA    _T("MinCirleArea")
#define	MAXCIRLEAREA    _T("MaxCirleArea")

#define	MINROUNDNESS     _T("MinRoundness")
#define	MAXROUNDNESS     _T("MaxRoundness")

#define	DILATIONRADIUS  _T("DilationRadius")

#define	EDGEFILTER      _T("EdgeFilter")
#define	EDGEALPHA       _T("EdgeAlpha")
#define	EDGEMINTHRELD   _T("EdgeMinThreld")
#define	EDGEMAXTHRELD   _T("EdgeMaxThreld")

CDetectCircularhole::CDetectCircularhole(void)
	: m_MinGray(128)
	, m_MaxGray(255)
	, m_MinCirleArea(0)
	, m_MaxCirleArea(999999.0)
	, m_MinRoundness(0.5)
	, m_MaxRoundness(1.0)
	, m_DilationRadius(3.5)
	, m_EdgeFilter(_T("canny"))
	, m_EdgeAlpha(1.0)
	, m_EdgeMinThreld(20)
	, m_EdgeMaxThreld(40)
{
	
	m_ShowErrorMeg = true;
	m_ShowProcessingImage = true;
}



CDetectCircularhole::~CDetectCircularhole(void)
{
}

void CDetectCircularhole::SetImageObject(Hobject image)
{
	ho_Capture1 = image;
	Halcon::get_image_size(ho_Capture1, &hv_Width, &hv_Height);
}

bool CDetectCircularhole::DetectCirleCenter(float &row, float &column)
{
	if(RunFitCirle())
	{
		row = (float)(hv_Row[0].D());
		column = (float)(hv_Column[0].D());
		return true;
	}
	return false;
}

void CDetectCircularhole::ShowErrorMessage(bool show)
{
	m_ShowErrorMeg = show;
}

void CDetectCircularhole::ShowMessage(CString msg)
{
	if(m_ShowErrorMeg)
	{
		AfxMessageBox(msg);
	}
}

bool CDetectCircularhole::RunThreshold()
{
	try
	{
		threshold(ho_Capture1, &ho_Region, m_MinGray, m_MaxGray);
		if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
		{
			clear_window(HDevWindowStack::GetActive());
			set_color(HDevWindowStack::GetActive(),"red");
			disp_obj(ho_Region, HDevWindowStack::GetActive());
			connection(ho_Region, &ho_ConnectedRegions);

			//为了设定参数方便，显示面积在1000~2000000（Pixel）的区域面积值
			Hobject selectRegions;
			HTuple RegionCount, Area, Row, Column;
			select_shape(ho_ConnectedRegions, &selectRegions, "area", "and", m_MinCirleArea/10, m_MaxCirleArea*10);
			count_obj(selectRegions, &RegionCount);
			area_center(selectRegions, &Area, &Row, &Column);
			set_color(HDevWindowStack::GetActive(),"blue");
			for (int i=1; i<=RegionCount; i+=1)
			{
				set_tposition(HDevWindowStack::GetActive(), HTuple(Row[i-1]), HTuple(Column[i-1]));
				write_string(HDevWindowStack::GetActive(), Area[i-1].D());
			}
		}
	}
	catch(...)
	{
		ShowMessage("二值化失败！");
		return false;
	}
	return true;
}

bool CDetectCircularhole::RunSelectCirles()
{
	if(RunThreshold())
	{
		try
		{
			select_shape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("area").Append("roundness")), "and", (HTuple(m_MinCirleArea).Append(m_MinRoundness)), (HTuple(m_MaxCirleArea).Append(m_MaxRoundness)));
			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				clear_window(HDevWindowStack::GetActive());
				set_color(HDevWindowStack::GetActive(),"green");
				disp_obj(ho_SelectedRegions, HDevWindowStack::GetActive());
			}
			return true;
		}
		catch(...)
		{
			ShowMessage("检测圆形区域失败！");
		}
	}
	return false;
}

bool CDetectCircularhole::RunDilationCircle()
{
	if(RunSelectCirles())
	{
		try
		{
			dilation_circle(ho_SelectedRegions, &ho_RegionDilation, m_DilationRadius);
			fill_up(ho_RegionDilation, &ho_RegionFillUp);
			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				clear_window(HDevWindowStack::GetActive());
				set_color(HDevWindowStack::GetActive(),"blue");
				disp_obj(ho_RegionFillUp, HDevWindowStack::GetActive());
			}
			return true;
		}
		catch(...)
		{
			ShowMessage("膨胀图像失败！");
		}
	}
	return false;
}

bool CDetectCircularhole::RunDetectEdges()
{
	if(RunDilationCircle())
	{
		try
		{
			rgb1_to_gray(ho_Capture1, &ho_GrayImage);
			reduce_domain(ho_GrayImage, ho_RegionFillUp, &ho_ImageReduced);
	
			m_EdgeAlpha = (float)(m_EdgeAlpha < 0.0 ? 1.0 : m_EdgeAlpha);
			edges_sub_pix(ho_ImageReduced, &ho_Edges, (char*)(LPCTSTR(m_EdgeFilter)), m_EdgeAlpha, m_EdgeMinThreld, m_EdgeMaxThreld);

			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				clear_window(HDevWindowStack::GetActive());
				set_color(HDevWindowStack::GetActive(),"red");
				disp_obj(ho_Edges, HDevWindowStack::GetActive());
			}

			return true;
		}
		catch(...)
		{
			ShowMessage("边沿检测失败！");
		}
	}
	return false;
}

bool CDetectCircularhole::RunFitCirle()
{
	if(RunDetectEdges())
	{
		try
		{
			length_xld(ho_Edges, &hv_Length);
			tuple_max(hv_Length, &hv_Max);
			select_shape_xld(ho_Edges, &ho_SelectedXLD, "contlength", "and", hv_Max, hv_Max);
			fit_circle_contour_xld(ho_SelectedXLD, "algebraic", -1, 0, 0, 3, 2, &hv_Row, &hv_Column, 
				&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);

			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				clear_window(HDevWindowStack::GetActive());
				set_color(HDevWindowStack::GetActive(),"red");
				disp_obj(ho_Capture1, HDevWindowStack::GetActive());
				disp_obj(ho_Edges, HDevWindowStack::GetActive());
				disp_cross(HDevWindowStack::GetActive(), hv_Row, hv_Column, 20, 0);

				set_color(HDevWindowStack::GetActive(),"green");
				disp_cross(HDevWindowStack::GetActive(), hv_Height/2, hv_Width/2, 20, 0);
			}

			return true;
		}
		catch(...)
		{
			ShowMessage("选择和拟合失败！");
		}
	}
	return false;
}

void CDetectCircularhole::LoadConfig()
{
	try
	{
		this->m_MinGray = GetPrivateProfileInt(_T("Threshold"),  MINGRAY, 128, m_ConfigPath);
		this->m_MaxGray = GetPrivateProfileInt(_T("Threshold"),  MAXGRAY, 255, m_ConfigPath);

		DataUtility::ConvertStringToFloat(GetFloatConfigString(_T("Circle Area"), MINCIRLEAREA), this->m_MinCirleArea, 0.0);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(_T("Circle Area"), MAXCIRLEAREA), this->m_MaxCirleArea, 999999.0);

		DataUtility::ConvertStringToFloat(GetFloatConfigString(_T("Circle Roundness"), MINROUNDNESS), this->m_MinRoundness, 0.5);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(_T("Circle Roundness"), MAXROUNDNESS), this->m_MaxRoundness, 1.0);

		DataUtility::ConvertStringToFloat(GetFloatConfigString(_T("Dilation Radius"), DILATIONRADIUS), this->m_DilationRadius, 3.5);

		this->m_EdgeFilter = GetFloatConfigString(_T("Edge"), EDGEFILTER, "canny");

		DataUtility::ConvertStringToFloat(GetFloatConfigString(_T("Edge"), EDGEALPHA), this->m_EdgeAlpha, 1.0);

		this->m_EdgeMinThreld = GetPrivateProfileInt(_T("Edge"), EDGEMINTHRELD, 20, m_ConfigPath);
		this->m_EdgeMaxThreld = GetPrivateProfileInt(_T("Edge"), EDGEMAXTHRELD, 40, m_ConfigPath);

	}
	catch(...)
	{
	}
}

CString CDetectCircularhole::GetFloatConfigString(CString section,  CString key, CString defautValue)
{
	CString ret = _T("");
	char buf[256];
	int len = GetPrivateProfileString(section, key, "",buf, 256, m_ConfigPath);
	if(len > 0)
	{
		for(int i=0;i<len;i++)
		{
		   CString str;
		   str.Format("%c",buf[i]);
		   ret+=str;
		}
	}
	else
	{
		ret = defautValue;
	}
	return ret;
}

void CDetectCircularhole::SaveConfig()
{
	try
	{
		CString text;
		text.Format(_T("%d"), this->m_MinGray);

		WritePrivateProfileString(_T("Threshold"),  MINGRAY, text, m_ConfigPath);

		text.Format(_T("%d"), this->m_MaxGray);
		WritePrivateProfileString(_T("Threshold"),  MAXGRAY, text, m_ConfigPath);

		text.Format(_T("%f"), this->m_MinCirleArea);
		WritePrivateProfileString(_T("Circle Area"), MINCIRLEAREA, text, m_ConfigPath);

		text.Format(_T("%f"), this->m_MaxCirleArea);
		WritePrivateProfileString(_T("Circle Area"), MAXCIRLEAREA, text, m_ConfigPath);

		text.Format(_T("%f"), this->m_MinRoundness);
		WritePrivateProfileString(_T("Circle Roundness"), MINROUNDNESS, text, m_ConfigPath);

		text.Format(_T("%f"), this->m_MaxRoundness);
		WritePrivateProfileString(_T("Circle Roundness"), MAXROUNDNESS, text, m_ConfigPath);

		text.Format(_T("%f"), this->m_DilationRadius);
		WritePrivateProfileString(_T("Dilation Radius"), DILATIONRADIUS, text, m_ConfigPath);

		text.Format(_T("%s"), this->m_EdgeFilter);
		WritePrivateProfileString(_T("Edge"), EDGEFILTER, text, m_ConfigPath);

		text.Format(_T("%f"), this->m_EdgeAlpha);
		WritePrivateProfileString(_T("Edge"), EDGEALPHA, text, m_ConfigPath);

		text.Format(_T("%d"), this->m_EdgeMinThreld);
		WritePrivateProfileString(_T("Edge"), EDGEMINTHRELD, text, m_ConfigPath);

		text.Format(_T("%d"), this->m_EdgeMaxThreld);
		WritePrivateProfileString(_T("Edge"), EDGEMAXTHRELD, text, m_ConfigPath);
	}
	catch(...)
	{
	}
}

void CDetectCircularhole::SetConfigPath(CString path)
{
	m_ConfigPath = path;
}