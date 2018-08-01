#ifndef GLOBALDEF_H
#define GLOBALDEF_H

/*************************************************************/
/*--------------------class SizeHandleRect-------------------*/
/*************************************************************/
//控制手柄大小
enum { SELECTION_HANDLE_SIZE = 6,
       SELECTION_MARGIN = 10
     };

//手柄选中状态
enum SelectionHandleState {
    SelectionHandleOff,
    SelectionHandleInactive,
    SelectionHandleActive };
//鼠标在矩形框的相对(象限坐标)位置
enum enumQuadrant {
    LeftTop ,
    Top,
    RightTop,
    Right,
    RightBottom,
    Bottom,
    LeftBottom,
    Left ,
    Center,
    None};
/*************************************************************/
#endif // GLOBALDEF_H
