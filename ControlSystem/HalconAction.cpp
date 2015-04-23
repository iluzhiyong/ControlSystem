///////////////////////////////////////

#include"stdafx.h"
#include"HalconAction.h"

//void action()
//{
//	HObject  ho_Image;
//	ReadImage(&ho_Image, "G:/Machine Vision/picture.png");
//}

//void action()
//{
//
//  // Local iconic variables
//  HObject  ho_Rim, ho_Edges, ho_Holes, ho_Hole;
//
//  // Local control variables
//  HTuple  hv_Width, hv_Height, hv_WindowID, hv_Row;
//  HTuple  hv_Column, hv_Radius, hv_Number, hv_i, hv_DistanceMin;
//  HTuple  hv_DistanceMax, hv_SVal, hv_Ascent, hv_Descent;
//  HTuple  hv_Width1, hv_Height1;
//
//  //Example program for distance_pc
//  //Determine the minimal and maximal radius of drill holes
//  //
//  if (HDevWindowStack::IsOpen())
//    CloseWindow(HDevWindowStack::Pop());
//  ReadImage(&ho_Rim, "G:/Machine Vision/picture.png");
//  GetImageSize(ho_Rim, &hv_Width, &hv_Height);
//  SetWindowAttr("background_color","black");
//  OpenWindow(0,0,hv_Width,hv_Height,0,"","",&hv_WindowID);
//  HDevWindowStack::Push(hv_WindowID);
//  if (HDevWindowStack::IsOpen())
//    DispObj(ho_Rim, HDevWindowStack::GetActive());
//  //set_display_font(hv_WindowID, 14, "mono", "false", "false");
//  //Segment the circular holes
//  //The alpha parameter was choosen different than the default value
//  //to ensure stronger smoothing and thusmore connected edge components
//  EdgesSubPix(ho_Rim, &ho_Edges, "canny", 4, 20, 40);
//  SelectShapeXld(ho_Edges, &ho_Holes, "circularity", "and", 0.7, 1.0);
//  SortContoursXld(ho_Holes, &ho_Holes, "upper_left", "true", "row");
//  //Determine the midpoints
//  SmallestCircleXld(ho_Holes, &hv_Row, &hv_Column, &hv_Radius);
//  CountObj(ho_Holes, &hv_Number);
//  if (HDevWindowStack::IsOpen())
//    SetColor(HDevWindowStack::GetActive(),"red");
//  //stop ()
//  //For all boles ...
//  for (hv_i=1; hv_i<=1; hv_i+=1)
//  {
//    SelectObj(ho_Holes, &ho_Hole, hv_i);
//    if (HDevWindowStack::IsOpen())
//      DispObj(ho_Rim, HDevWindowStack::GetActive());
//    if (HDevWindowStack::IsOpen())
//      DispObj(ho_Hole, HDevWindowStack::GetActive());
//    //Compute the minimal and maximal radius of the holes by computing the
//    //distance of the midpoint to the contour
//    DistancePc(ho_Hole, HTuple(hv_Row[hv_i-1]), HTuple(hv_Column[hv_i-1]), &hv_DistanceMin, 
//        &hv_DistanceMax);
//    //And visualize the results (minimal and maximal radius)
//    DispCross(hv_WindowID, HTuple(hv_Row[hv_i-1]), HTuple(hv_Column[hv_i-1]), 20, 
//        0);
//    TupleString(hv_DistanceMin, ".3f", &hv_SVal);
//    GetStringExtents(hv_WindowID, "min: "+hv_SVal, &hv_Ascent, &hv_Descent, &hv_Width1, 
//        &hv_Height1);
//    //disp_message (WindowID, 'min: ' + SVal, 'image', Row[i - 1] - 2 * Ascent, Column[i - 1] - Radius[i - 1] - Width1 - 10, 'black', 'true')
//    //disp_message (WindowID, 'max: ' + DistanceMax$'.3f', 'image', Row[i - 1], Column[i - 1] - Radius[i - 1] - Width1 - 10, 'black', 'true')
//    //disp_message (WindowID, 'X:' + Row$'.3f', 'image', 300, 300, 'black', 'true')
//
//    //disp_continue_message(hv_WindowID, "black", "true");
//
//    //stop ()
//  }
//  //disp_message(hv_WindowID, (("X:"+(HTuple(hv_Row[0]).TupleString(".3f")))+" Y:")+(HTuple(hv_Column[0]).TupleString(".3f")), 
//      //"image", 300, 300, "black", "true");
//  DispCross(hv_WindowID, hv_Height/2, hv_Width/2, 20, 0);
//  //disp_message(hv_WindowID, (("X: "+((hv_Width/2).TupleString(".3f")))+" Y:")+((hv_Height/2).TupleString(".3f")), 
//      //"image", hv_Height/2, hv_Width/2, "black", "true");
//
//  //stop ()
//  //dev_close_window ()
//  //****************************************
//  //XLD fit
//}

void action()
{

  // Local iconic variables
  HObject  ho_Rim, ho_Edges, ho_Holes, ho_Hole;

  // Local control variables
  HTuple  hv_Width, hv_Height, hv_WindowID, hv_Row;
  HTuple  hv_Column, hv_Radius, hv_Number, hv_i, hv_DistanceMin;
  HTuple  hv_DistanceMax, hv_SVal, hv_Ascent, hv_Descent;
  HTuple  hv_Width1, hv_Height1;

  //Example program for distance_pc
  //Determine the minimal and maximal radius of drill holes
  //
  //if (HDevWindowStack::IsOpen())
  //  CloseWindow(HDevWindowStack::Pop());
  ReadImage(&ho_Rim, "rim");
  GetImageSize(ho_Rim, &hv_Width, &hv_Height);
  SetWindowAttr("background_color","black");
  OpenWindow(0,0,hv_Width,hv_Height,0,"","",&hv_WindowID);
  HDevWindowStack::Push(hv_WindowID);
  if (HDevWindowStack::IsOpen())
    DispObj(ho_Rim, HDevWindowStack::GetActive());
  //set_display_font(hv_WindowID, 14, "mono", "false", "false");
  //Segment the circular holes
  //The alpha parameter was choosen different than the default value
  //to ensure stronger smoothing and thusmore connected edge components
  EdgesSubPix(ho_Rim, &ho_Edges, "canny", 4, 20, 40);
  SelectShapeXld(ho_Edges, &ho_Holes, "circularity", "and", 0.7, 1.0);
  SortContoursXld(ho_Holes, &ho_Holes, "upper_left", "true", "row");
  //Determine the midpoints
  SmallestCircleXld(ho_Holes, &hv_Row, &hv_Column, &hv_Radius);
  CountObj(ho_Holes, &hv_Number);
  if (HDevWindowStack::IsOpen())
    SetColor(HDevWindowStack::GetActive(),"yellow");
  //For all boles ...
  {
  HTuple end_val20 = hv_Number;
  HTuple step_val20 = 1;
  for (hv_i=1; hv_i.Continue(end_val20, step_val20); hv_i += step_val20)
  {
		SelectObj(ho_Holes, &ho_Hole, hv_i);
		if (HDevWindowStack::IsOpen())
			DispObj(ho_Rim, HDevWindowStack::GetActive());
		if (HDevWindowStack::IsOpen())
			DispObj(ho_Hole, HDevWindowStack::GetActive());
		//Compute the minimal and maximal radius of the holes by computing the
		//distance of the midpoint to the contour
		DistancePc(ho_Hole, HTuple(hv_Row[hv_i-1]), HTuple(hv_Column[hv_i-1]), &hv_DistanceMin, 
			&hv_DistanceMax);
		//And visualize the results (minimal and maximal radius)
		DispCross(hv_WindowID, HTuple(hv_Row[hv_i-1]), HTuple(hv_Column[hv_i-1]), 6, 
			0);
		TupleString(hv_DistanceMin, ".3f", &hv_SVal);
		GetStringExtents(hv_WindowID, "min: "+hv_SVal, &hv_Ascent, &hv_Descent, &hv_Width1, 
			&hv_Height1);
		//disp_message(hv_WindowID, "min: "+hv_SVal, "image", HTuple(hv_Row[hv_i-1])-(2*hv_Ascent), 
		//    ((HTuple(hv_Column[hv_i-1])-HTuple(hv_Radius[hv_i-1]))-hv_Width1)-10, "black", 
		//    "true");
		//disp_message(hv_WindowID, "max: "+(hv_DistanceMax.TupleString(".3f")), "image", 
		//    HTuple(hv_Row[hv_i-1]), ((HTuple(hv_Column[hv_i-1])-HTuple(hv_Radius[hv_i-1]))-hv_Width1)-10, 
		//    "black", "true");
		//disp_continue_message(hv_WindowID, "black", "true");
		// stop(); only in hdevelop
	 }
  }
  //if (HDevWindowStack::IsOpen())
  //  CloseWindow(HDevWindowStack::Pop());
}