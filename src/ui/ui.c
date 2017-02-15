#include <stdlib.h>

#include "ui/ui.h"

Window*
CreateWindow(uint32_t bg_col) {
    Window *win = (Window*)CreateControl(sizeof(Window));
    if(win == NULL)
        return NULL;

    win->BGColor = bg_col;

    return win;
}

Control*
CreateControl(int size) {
    Control *ctrl = malloc(size);
    if(ctrl == NULL)
        return NULL;

    return ctrl;
}

Label*
CreateLabel(const char *str, uint32_t fg_col, uint32_t bg_col) {

    Label *lbl = (Label*)CreateControl(sizeof(Label));
    if(lbl == NULL)
        return NULL;

    lbl->String = malloc(strlen(str) + 1);
    if(lbl->String == NULL){
        DestroyControl(lbl);
        return NULL;
    }
    strcpy(lbl->String, str);
    lbl->FGColor = fg_col;
    lbl->BGColor = bg_col;

    return lbl;
}

ImageBox*
CreateImageBox(Texture *img) {

    ImageBox *imgBox = (ImageBox*)CreateControl(sizeof(ImageBox));
    if(imgBox == NULL)
        return NULL;

    imgBox->Image = img;
    
    return imgBox;
}

FreeLayoutPanel*
CreateFreeLayoutPanel(uint32_t bg_col) {

    FreeLayoutPanel *freeLayout = (FreeLayoutPanel*)CreateControl(sizeof(FreeLayoutPanel));
    if(freeLayout == NULL)
        return NULL;

    freeLayout->BGColor = bg_col;

    return freeLayout;
}

SplitLayoutPanel*
CreateSplitLayoutPanel(int split_loc, Orientation orient) {

    SplitLayoutPanel *splitLayout = (SplitLayoutPanel*)CreateControl(sizeof(SplitLayoutPanel));
    if(splitLayout == NULL)
        return NULL;
    
    splitLayout->SplitLocation = split_loc;
    splitLayout->Orientation = orient;
    
    return splitLayout;
}

FlowLayoutPanel*
CreateFlowLayoutPanel(int item_dim, Orientation orient, uint32_t bg_col) {

    FlowLayoutPanel *flowLayout = (FlowLayoutPanel*)CreateControl(sizeof(FlowLayoutPanel));
    if(flowLayout == NULL)
        return NULL;

    flowLayout->ElementDimension = item_dim;
    flowLayout->Orientation = orient;
    flowLayout->BGColor = bg_col;

    return flowLayout;
}

void
DestroyControl(Control *ctrl) {
    
    for(int i = 0; i < ctrl->ChildrenCount; i++){
        Control *c = ctrl->Children[i];
        ctrl->Children[i] = NULL;

        if(c->Destroy != NULL)
            c->Destroy(c);
    }

    free(ctrl);
}