#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cctype>
#include <cmath>

using namespace std;

int depth;

bool isInteger(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isValidDepth(int d) {
    return (d >= 0 && d <= 7);
}

int inputDepth() {
    string input;
    int d;
    
    while (true) {
        cout << "Введите глубину рекурсии (0-7): ";
        cin >> input;
        
        if (input.empty()) {
            cout << "Ошибка: строка пуста\n";
            continue;
        }
        
        if (!isInteger(input)) {
            cout << "Ошибка: введите целое число (0-7)\n";
            continue;
        }
        
        d = stoi(input);
        
        if (!isValidDepth(d)) {
            cout << "Ошибка: глубина должна быть от 0 до 7\n";
            continue;
        }
        
        break;
    }
    
    return d;
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void sierpinski(float x1, float y1, float x2, float y2, float x3, float y3, int d) {
    if (d == 0) {
        drawTriangle(x1, y1, x2, y2, x3, y3);
        return;
    }
    
    float x12 = (x1 + x2) / 2;
    float y12 = (y1 + y2) / 2;
    float x23 = (x2 + x3) / 2;
    float y23 = (y2 + y3) / 2;
    float x31 = (x3 + x1) / 2;
    float y31 = (y3 + y1) / 2;
    
    sierpinski(x1, y1, x12, y12, x31, y31, d - 1);
    sierpinski(x12, y12, x2, y2, x23, y23, d - 1);
    sierpinski(x31, y31, x23, y23, x3, y3, d - 1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    
    float w = glutGet(GLUT_WINDOW_WIDTH);
    float h = glutGet(GLUT_WINDOW_HEIGHT);
    
    sierpinski(w/2, 50, 50, h-50, w-50, h-50, depth);
    
    glFlush();
}

int main(int argc, char** argv) {
    depth = inputDepth();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Треугольник Серпинского");
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 800, 600, 0);
    glutDisplayFunc(display);
    
    cout << "\nГлубина: " << depth << endl;
    cout << "Количество треугольников: " << (int)pow(3, depth) << endl;
    
    glutMainLoop();
    
    return 0;
}
