#ifndef HBN_UI_WINDOW_H_
#define HBN_UI_WINDOW_H_

#include "ui.h"

typedef struct Window {
    Control ControlData;
    uint32_t BGColor;
} Window;

Window*
CreateWindow(uint32_t bg_col);

void Window_Activate(Control *this_ctrl);
void Window_Deactivate(Control *this_ctrl);
void Window_Click(Control *this_ctrl, ClickInfo *click_info);
void Window_Update(Control *this_ctrl, uint32_t time);
void Window_Render(Control *this_ctrl, uint32_t time);
void Window_AddChild(Control *this_ctrl, struct Control *child);
void Window_RemoveChild(Control *this_ctrl, int idx);
void Window_Destroy(Control *this_ctrl);

#endif