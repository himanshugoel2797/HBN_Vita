#ifndef HBN_UI_H_
#define HBN_UI_H_

#include "helpers/display.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct ClickInfo{
    int Buttons;
    uint8_t FrontTouchForce;
    uint8_t BackTouchForce;
};

typedef enum {
    Orientation_Vertical,
    Orientation_Horizontal
} Orientation;

typedef struct Control{
    int X;
    int Y;
    int Width;
    int Height;
    bool Enabled;
    void *ObjectState;

    void (*Activate)(Control *this_ctrl);
    void (*Deactivate)(Control *this_ctrl);
    void (*Click)(Control *this_ctrl, ClickInfo *click_info);
    void (*Update)(Control *this_ctrl, uint32_t time);
    void (*Render)(Control *this_ctrl, uint32_t time);
    void (*AddChild)(Control *this_ctrl, struct Control *child);
    void (*RemoveChild)(Control *this_ctrl, int idx);
    void (*Destroy)(Control *this_ctrl);

    struct Control *Children;
    int ChildrenCount;
    struct Control *Parent;
} Control;

typedef struct FlowLayoutPanel {
    Control ControlData;
    int ElementDimension;
    int CurrentItem;
    Orientation Orientation;
    uint32_t BGColor;

    void (*SetActiveItem)(Control *this_ctrl, int item);
} FlowLayoutPanel;

typedef struct SplitLayoutPanel {
    Control ControlData;
    int SplitLocation;
    Orientation Orientation;
} SplitLayoutPanel;

typedef struct FreeLayoutPanel {
    Control ControlData;
    int ElementWidth;
    int CurrentItem;
    uint32_t BGColor;

    void (*SetActiveItem)(Control *this_ctrl, int item);
} FreeLayoutPanel;

typedef struct ImageBox {
    Control ControlData;
    Texture *Image;
} ImageBox;

typedef struct Label {
    Control ControlData;
    char *String;
    uint32_t FGColor;
    uint32_t BGColor;
} Label;

Control*
CreateControl(int size);

Label*
CreateLabel(const char *str, uint32_t fg_col, uint32_t bg_col);

ImageBox*
CreateImageBox(Texture *img);

FreeLayoutPanel*
CreateFreeLayoutPanel(uint32_t bg_col);

SplitLayoutPanel*
CreateSplitLayoutPanel(int split_loc, Orientation orient);

FlowLayoutPanel*
CreateFlowLayoutPanel(int item_dim, Orientation orient, uint32_t bg_col);

void
DestroyControl(Control *ctrl);

#endif