#include <xgfx/window.h>
#include <xgfx/drawing.h>
#include <stdlib.h>

typedef struct point {
    int x;
    int y;
    struct point* next;
} Point;

typedef struct {
    Point* head;
    int color;
} Path;

Path paths[100];
int numPaths = 0;

int drawingPath = -1; // -1 means not drawing a path, 0 or greater is the index of the path being drawn
int mouseX = 0;
int mouseY = 0;

int distanceSquared(Point* p, int x, int y) {
    return (p->x-x)*(p->x-x) + (p->y-y)*(p->y-y);
}

int main() {
    initWindow(640, 480, "CAD");

    while (1) {
        Event event;
        while (checkWindowEvent(&event)) {
            if (event.type == WINDOW_CLOSE) {
                return 0;
            }
            if (event.type == MOUSE_MOVE) {
                mouseX = event.mousemove.x;
                mouseY = event.mousemove.y;
            }
            if (event.type == MOUSE_BUTTON && event.mousebutton.button == BUTTON_LEFT && event.mousebutton.state == 1) {
                Point* newPoint = malloc(sizeof(Point));
                if (drawingPath >= 0) {
                    newPoint->next = paths[drawingPath].head;
                    paths[drawingPath].head = newPoint;
                    Point* firstPoint = paths[drawingPath].head;
                    while (firstPoint->next != 0) {
                        firstPoint = firstPoint->next;
                    }
                    if (distanceSquared(firstPoint, mouseX, mouseY) < 100) {
                        newPoint->x = firstPoint->x;
                        newPoint->y = firstPoint->y;
                        paths[drawingPath].color = 0xff0000ff;
                        drawingPath = -1; // finished path
                    }
                    else {
                        newPoint->x = mouseX;
                        newPoint->y = mouseY;
                    }
                }
                else {
                    drawingPath = numPaths; // create a new path
                    numPaths++;
                    newPoint->x = mouseX;
                    newPoint->y = mouseY;
                    paths[drawingPath].head = newPoint;
                    paths[drawingPath].color = 0xff00ff00;
                }
            }
        }

        clear();
        for (int i = 0; i < numPaths; i++) {
            Point* current = paths[i].head;
            while (current->next != 0) {
                line(current->x, current->y, current->next->x, current->next->y, paths[i].color);
                current = current->next;
            }
            if (distanceSquared(current, mouseX, mouseY) < 100) {
                circle(current->x, current->y, 5, 0xffffff00);
            }
        }

        if (drawingPath >= 0) {
            line(paths[drawingPath].head->x, paths[drawingPath].head->y, mouseX, mouseY, 0xffffffff);
        }
        circle(mouseX, mouseY, 5, 0xffffffff);
        updateWindow();
    }
}
