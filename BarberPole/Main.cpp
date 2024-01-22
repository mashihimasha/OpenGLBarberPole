#include <glut.h>  
#include <math.h>

// For animating the rotation of the objects
float rotation = 0.0;
GLfloat PI = 3.14;
// variables to move outermost Object Frame (to move all the rendered environment)
float moveX = 0.0f;
float moveY = 0.0f;
float moveZ = 0.0f;

GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;

//variables to move the camera
float camX = 0.0f;
float camY = 0.0f;
float camZ = 0.0f;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}

//draw co-ordinate axes
void drawAxes() {

    glBegin(GL_LINES);

    glLineWidth(1.5);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-20, 0, 0);
    glVertex3f(20, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -20, 0);
    glVertex3f(0, 20, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, -20);
    glVertex3f(0, 0, 20);

    glEnd();
}

//draw white grid
void drawGrid() {
    GLfloat ext = 20.0f;
    GLfloat step = 1.0f;
    GLfloat yGrid = -1.0f;
    GLint line;

    glBegin(GL_LINES);
    for (line = -ext; line <= ext; line += step) {
        glVertex3f(line, yGrid, ext);
        glVertex3f(line, yGrid, -ext);

        glVertex3f(ext, yGrid, line);
        glVertex3f(-ext, yGrid, line);
    }
    glEnd();
}

//draw one helix strip
void drawHelixStrip(GLfloat radius, GLfloat y_start, GLfloat width,GLfloat height) {
    GLfloat angle = 0;
    GLint n = 64;
    GLfloat slope = 0.03;

    while (y_start <= height) {

        glBegin(GL_QUAD_STRIP);

        for (int i = 0; i <= n; i++) {
            angle = 2 * PI * (static_cast<float>(i) / n);
            double x = radius * cos(angle);
            double z = radius * sin(angle);

            // Calculate normal vector
            GLfloat normalX = static_cast<float>(cos(angle));
            GLfloat normalY = 0.0f; // Y-axis is the axis of rotation
            GLfloat normalZ = static_cast<float>(sin(angle));

            // Set normal vector
            glNormal3f(normalX, normalY, normalZ);

            glVertex3f(x, y_start, z);
            glVertex3f(x, y_start + width, z);

            y_start += slope;
        }

        glVertex3f(radius, y_start, 0);
        glVertex3f(radius, y_start+width, 0);

        glEnd();
    }
}

//draw cylinder
void drawCylinder(GLfloat radius, GLfloat height) {
    GLfloat angle = 0.0;
    GLint n = 64;
    GLfloat y = 0.0;

    glBegin(GL_QUAD_STRIP);
    glColor3f(1.0, 1.0, 1.0);

    for (int i = 0; i <= n;i++) {
        angle = 2 * PI * (static_cast<float>(i) / n);
        double x = radius * cos(angle);
        double z = radius * sin(angle);
        glVertex3f(x, y, z); 
        glVertex3f(x, y + height, z);
    }

    glVertex3f(radius, 0, 0);
    glVertex3f(radius, height, 0);

    glEnd();

}

////draw helix
//void drawHelix() {
//
//    glColor3f(1.0, 0.0, 0.0);
//    drawHelixStrip(1.55, 0.0, 0.5, 7.5);
//
//    glColor3f(0.0, 0.0, 1.0);
//    drawHelixStrip(1.55, 0.95, 0.5, 7.9);
//
//}

//set lighting
void setLighting() {
    // Lighting set up
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Set lighting intensity and color
    GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat qaDiffuseLight[] = { 1.0, 1.0, 1.0, 1.0 };  
    GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };  


    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

    // Set the light position
    GLfloat qaLightPosition[] = { 0.0, 2.0, -400.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

    //set light 1
    glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, qaSpecularLight);

}

//draw object with lighting
void drawObjectWithLighting() {

    // Set material properties
    GLfloat qaRed[] = { 1.0, 0.0, 0.0, 1.0};
    GLfloat qaBlue[] = { 0.0, 0.0, 1.0, 1.0};
    GLfloat qaSilver[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat qaSilverDark[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat qaWhite[] = { 1.0,1.0,1.0,1.0 };
    GLfloat qaLowAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat qaFullAmbient[] = { 1.0, 1.0, 1.0, 1.0 };

    //Set, ambient, diffuse and specular lighting. Set ambient to 20%.
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaSilver);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaSilver);
    glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 40.0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaLowAmbient);
    glLightfv(GL_LIGHT1, GL_AMBIENT, qaLowAmbient);

    glEnable(GL_LIGHTING);

    glShadeModel(GL_SMOOTH);

    //top sphere
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslated(0.0, 8.0, 0.0);
    glutSolidSphere(1.8f, 60, 60);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaSilverDark);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaSilverDark);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

    //bottom sphere
    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6);
    glutSolidSphere(1.8f, 60, 60);
    glPopMatrix();

    //bottom torus 1
    glPushMatrix();
    glRotated(-90, 1.0, 0.0, 0.0);
    glTranslated(0.0, 0.0, 0.85);
    glColor3f(0.6, 0.6, 0.6);
    glutSolidTorus(0.2, 1.8, 10, 50);
    glPopMatrix();

    //bottom torus 2
    glPushMatrix();
    glRotated(-90, 1.0, 0.0, 0.0);
    glTranslated(0.0, 0.0, 0.8);
    glColor3f(0.5, 0.5, 0.5);
    glutSolidTorus(0.2, 1.8, 10, 50);
    glPopMatrix();

    //top torus 1
    glPushMatrix();
    glTranslated(0.0, 7.1, 0.0);
    glRotated(-90, 1.0, 0.0, 0.0);
    glColor3f(0.6, 0.6, 0.6);
    glutSolidTorus(0.15, 1.73, 10, 50);
    glPopMatrix();

    //top torus middle
    glPushMatrix();
    glTranslated(0.0, 7.0, 0.0);
    glRotated(-90, 1.0, 0.0, 0.0);
    glColor3f(0.7, 0.7, 0.7);
    glutSolidTorus(0.1, 1.72, 10, 50);
    glPopMatrix();

    //top torus 2
    glPushMatrix();
    glTranslated(0.0, 6.9, 0.0);
    glRotated(-90, 1.0, 0.0, 0.0);
    glColor3f(0.5, 0.5, 0.5);
    glutSolidTorus(0.15, 1.7, 10, 50);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
    glPushMatrix();
    drawCylinder(1.5, 8.0);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaRed);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
    glMaterialf(GL_FRONT, GL_SHININESS, 200.0);

    glPushMatrix();
    glRotated(rotation, 0.0, 1.0, 0.0);
    glTranslated(0.0, -0.5, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    drawHelixStrip(1.55, 0.0, 0.5, 7.5);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaBlue);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
    glMaterialf(GL_FRONT, GL_SHININESS, 200.0);

    glPushMatrix();
    glRotated(rotation, 0.0, 1.0, 0.0);
    glTranslated(0.0, -0.5, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    drawHelixStrip(1.55, 0.95, 0.5, 7.9);    
    glPopMatrix();

    glDisable(GL_LIGHTING);
}

//draw object without lighting
//void drawObject() {
//
//    //top sphere
//    glPushMatrix();
//    glColor3f(1.0, 1.0, 1.0);
//    glTranslated(0.0, 8.0, 0.0);
//    glutSolidSphere(1.8f, 60, 60);
//    glPopMatrix();
//
//    //bottom sphere
//    glPushMatrix();
//    glColor3f(0.6, 0.6, 0.6);
//    glutSolidSphere(1.8f, 60, 60);
//    glPopMatrix();
//
//    //bottom torus 1
//    glPushMatrix();
//    glRotated(-90, 1.0, 0.0, 0.0);
//    glTranslated(0.0, 0.0, 0.85);
//    glColor3f(0.6, 0.6, 0.6);
//    glutSolidTorus(0.2, 1.8, 10, 50);
//    glPopMatrix();
//
//    //bottom torus 2
//    glPushMatrix();
//    glRotated(-90, 1.0, 0.0, 0.0);
//    glTranslated(0.0, 0.0, 0.8);
//    glColor3f(0.5, 0.5, 0.5);
//    glutSolidTorus(0.2, 1.8, 10, 50);
//    glPopMatrix();
//
//    //top torus 1
//    glPushMatrix();
//    glTranslated(0.0, 7.1, 0.0);
//    glRotated(-90, 1.0, 0.0, 0.0);
//    glColor3f(0.6, 0.6, 0.6);
//    glutSolidTorus(0.15, 1.73, 10, 50);
//    glPopMatrix();
//
//    //top torus middle
//    glPushMatrix();
//    glTranslated(0.0, 7.0, 0.0);
//    glRotated(-90, 1.0, 0.0, 0.0);
//    glColor3f(0.7, 0.7, 0.7);
//    glutSolidTorus(0.1, 1.72, 10, 50);
//    glPopMatrix();
//
//    //top torus 2
//    glPushMatrix();
//    glTranslated(0.0, 6.9, 0.0);
//    glRotated(-90, 1.0, 0.0, 0.0);
//    glColor3f(0.5, 0.5, 0.5);
//    glutSolidTorus(0.15, 1.7, 10, 50);
//    glPopMatrix();
//
//    glPushMatrix();
//    drawCylinder(1.5, 8.0);
//    glPopMatrix();
//
//    glPushMatrix();
//    glRotated(rotation, 0.0, 1.0, 0.0);
//    glTranslated(0.0, -0.5, 0.0);
//    drawHelix();
//    glPopMatrix();
//
//}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Setting the modelview matrix to the identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    gluLookAt(0.0 + camX, 2.5 + camY, 10.0 + camZ, 0, 0, 0, 0, 1.0, 0);
    glColor3f(1.0, 1.0, 1.0);

    glTranslatef(moveX, moveY, moveZ);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

    drawGrid();

    drawAxes();

    setLighting();

    glPushMatrix();
    drawObjectWithLighting();
    glPopMatrix();

    glutSwapBuffers();

}

void keyboardSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        camY += 0.5;
    if (key == GLUT_KEY_DOWN)
        camY -= 0.5;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

    if (key == 'z')
        moveZ += 1;

    if (key == 'Z')
        moveZ -= 1;

    if (key == 'l')
        rotY -= 5.0;

    if (key == 'r')
        rotY += 1.0;

    glutPostRedisplay();

}

void Timer(int x) {
    rotation += rotation >= 360.0 ? -rotation : 2;
    glutPostRedisplay();

    glutTimerFunc(60, Timer, 1);
}

void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define the Perspective projection frustum
    // (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
    gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(1080, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Barber Pole");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // keyboard function activation
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);

    glutTimerFunc(60.0, Timer, 1);
    init();
    glutMainLoop();


    return 0;
}