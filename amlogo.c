#ifdef __PLATFORM_OS_X__
#include <GLUT/glut.h>
#elif
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>

#define OUTER_CURVE_SEGMENTS 6
#define INNER_CURVE_SEGMENTS 5
#define SEGMENT_RESOLUTION 20
#define NUM_VERTICES (OUTER_CURVE_SEGMENTS * SEGMENT_RESOLUTION + 1) + (INNER_CURVE_SEGMENTS * SEGMENT_RESOLUTION + 1)

#ifndef CALLBACK
#define CALLBACK
#endif

GLfloat spin = 45.0;
GLfloat movex = 0.0;
GLfloat movey = 0.0;
GLfloat movez = 0.0;
GLuint startList;
GLdouble vertices[NUM_VERTICES][3];
GLdouble outer_curve[6][4][3] = {
  {{-1.795, -0.387, 0.0}, {-1.386, -0.295, 0.0}, {-0.893, 0.175, 0.0}, {-0.515, 0.466, 0.0}},
  {{-0.515, 0.466, 0.0}, {-0.006, 0.857, 0.0}, {0.576, 1.155, 0.0}, {0.906, 1.286, 0.0}},
  {{0.906, 1.286, 0.0}, {1.657, 1.583, 0.0}, {1.430, 1.414, 0.0}, {1.118, 1.129, 0.0}},
  {{1.118, 1.129, 0.0}, {0.974, 0.998, 0.0}, {0.845, 0.846, 0.0}, {0.738, 0.718, 0.0}},
  {{0.738, 0.718, 0.0}, {0.625, 0.584, 0.0}, {0.497, 0.353, 0.0}, {0.401, 0.165, 0.0}},
  {{0.401, 0.165, 0.0}, {0.308, -0.018, 0.0}, {0.198, -0.210, 0.0}, {-0.009, -0.387, 0.0}}};
GLdouble inner_curve[5][4][3] = {
  {{-0.411, -0.387, 0.0}, {-0.116, -0.138, 0.0}, {0.049, 0.025, 0.0}, {0.197, 0.263, 0.0}},
  {{0.197, 0.263, 0.0}, {0.328, 0.473, 0.0}, {0.474, 0.696, 0.0}, {0.690, 0.905, 0.0}},
  {{0.690, 0.905, 0.0}, {1.070, 1.254, 0.0}, {0.997, 1.261, 0.0}, {0.459, 0.969, 0.0}},
  {{0.459, 0.969, 0.0}, {0.287, 0.876, 0.0}, {-0.045, 0.641, 0.0}, {-0.343, 0.398, 0.0}}, 
  {{-0.343, 0.398, 0.0}, {-0.726, 0.084, 0.0}, {-0.982, -0.305, 0.0}, {-1.323, -0.387, 0.0}}};

static GLdouble calculate_bezier_point(GLdouble a, GLdouble b, GLdouble c, GLdouble d, float u) {
  float v = 1.0 - u;

  return (a * v * v * v) + (b * 3 * v * v * u) + (c * 3 * v * u * u) + (d * u * u * u);
}

static void create_vertex_array() {
  int index, segment, u;
  index = 0;
  for (segment = 0; segment < OUTER_CURVE_SEGMENTS; segment++) {
    for (u = 0; u < SEGMENT_RESOLUTION; u++, index++) {
      vertices[index][0] = calculate_bezier_point(outer_curve[segment][0][0], 
                                                  outer_curve[segment][1][0], 
                                                  outer_curve[segment][2][0], 
                                                  outer_curve[segment][3][0], 
                                                  u/(float)SEGMENT_RESOLUTION);
      vertices[index][1] = calculate_bezier_point(outer_curve[segment][0][1], 
                                                  outer_curve[segment][1][1], 
                                                  outer_curve[segment][2][1], 
                                                  outer_curve[segment][3][1], 
                                                  u/(float)SEGMENT_RESOLUTION);
      vertices[index][2] = calculate_bezier_point(outer_curve[segment][0][2], 
                                                  outer_curve[segment][1][2], 
                                                  outer_curve[segment][2][2], 
                                                  outer_curve[segment][3][2], 
                                                  u/(float)SEGMENT_RESOLUTION);
    }
  }

  vertices[index][0] = calculate_bezier_point(outer_curve[OUTER_CURVE_SEGMENTS-1][0][0], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][1][0], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][2][0], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][3][0], 
                                              1.0);
  vertices[index][1] = calculate_bezier_point(outer_curve[OUTER_CURVE_SEGMENTS-1][0][1], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][1][1], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][2][1], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][3][1], 
                                              1.0);
  vertices[index][2] = calculate_bezier_point(outer_curve[OUTER_CURVE_SEGMENTS-1][0][2], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][1][2], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][2][2], 
                                              outer_curve[OUTER_CURVE_SEGMENTS-1][3][2], 
                                              1.0);
  index++;

  for (segment = 0; segment < INNER_CURVE_SEGMENTS; segment++) {
    for (u = 0; u < SEGMENT_RESOLUTION; u++, index++) {
      vertices[index][0] = calculate_bezier_point(inner_curve[segment][0][0], 
                                                  inner_curve[segment][1][0], 
                                                  inner_curve[segment][2][0], 
                                                  inner_curve[segment][3][0], 
                                                  u/(float)SEGMENT_RESOLUTION);
      vertices[index][1] = calculate_bezier_point(inner_curve[segment][0][1], 
                                                  inner_curve[segment][1][1], 
                                                  inner_curve[segment][2][1], 
                                                  inner_curve[segment][3][1], 
                                                  u/(float)SEGMENT_RESOLUTION);
      vertices[index][2] = calculate_bezier_point(inner_curve[segment][0][2], 
                                                  inner_curve[segment][1][2], 
                                                  inner_curve[segment][2][2], 
                                                  inner_curve[segment][3][2], 
                                                  u/(float)SEGMENT_RESOLUTION);
    }
  }
  vertices[index][0] = calculate_bezier_point(inner_curve[INNER_CURVE_SEGMENTS-1][0][0], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][1][0], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][2][0], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][3][0], 
                                              1.0);
  vertices[index][1] = calculate_bezier_point(inner_curve[INNER_CURVE_SEGMENTS-1][0][1], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][1][1], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][2][1], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][3][1], 
                                              1.0);
  vertices[index][2] = calculate_bezier_point(inner_curve[INNER_CURVE_SEGMENTS-1][0][2], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][1][2], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][2][2], 
                                              inner_curve[INNER_CURVE_SEGMENTS-1][3][2], 
                                              1.0);
  index++;
}

void CALLBACK beginCallback(GLenum which) {
  glBegin(which);
}

void CALLBACK vertexCallback(void *vertex_data) {
  GLdouble* vertex = (GLdouble *) vertex_data;

  glVertex3dv(vertex);
  glNormal3dv(vertex);
}

void CALLBACK errorCallback(GLenum errorCode) {
  const GLubyte* estring;

  estring = gluErrorString(errorCode);
  fprintf(stderr, "Tessallation Error: %s\n", estring);
  exit(1);
}

void CALLBACK endCallback(void) {
  glEnd();
}

static void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);

  create_vertex_array();

  startList = glGenLists(1);

  GLUtesselator* tobj = gluNewTess();
  gluTessCallback(tobj, GLU_TESS_VERTEX, vertexCallback);
  gluTessCallback(tobj, GLU_TESS_EDGE_FLAG, glEdgeFlag);
  gluTessCallback(tobj, GLU_TESS_BEGIN, beginCallback);
  gluTessCallback(tobj, GLU_TESS_END, endCallback);
  gluTessCallback(tobj, GLU_TESS_ERROR, errorCallback);

  glNewList(startList, GL_COMPILE);
  gluTessBeginPolygon(tobj, NULL);
    gluTessBeginContour(tobj);
      int index;
      for (index = 0; index < NUM_VERTICES; index++)
        gluTessVertex(tobj, vertices[index], vertices[index]);
    gluTessEndContour(tobj);
  gluTessEndPolygon(tobj);
  glEndList();

  gluDeleteTess(tobj);

  glEnable(GL_DEPTH_TEST);

  GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0};
  GLfloat mat_shininess[] = {50.0};
  GLfloat mat_diffuse[] = {0.674, 0.0, 0.0, 1.0};
  GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
  GLfloat light_position[] = {0.0, 5.0, 5.0, 0.0};
  GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lmodel_ambient[] = {0.5, 0.5, 0.5, 1.0};

  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_POLYGON_SMOOTH);
}

static void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glPushMatrix();
  glRotatef(spin, -1.0, 1.0, 0.0);
  glTranslatef(movex, movey, movez);

  glColor3f(0.674, 0.0, 0.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glCallList(startList);

  int next, index;
  for (index = 0; index < NUM_VERTICES; index++) {
    next = index == NUM_VERTICES - 1 ? 0 : index + 1;
    glBegin(GL_POLYGON);
      glNormal3dv(vertices[index]);
      glNormal3d(vertices[index][0], vertices[index][1], -0.2);
      glNormal3d(vertices[next][0], vertices[next][1], -0.2);
      glNormal3dv(vertices[next]);

      glVertex3dv(vertices[index]);
      glVertex3d(vertices[index][0], vertices[index][1], -0.2);
      glVertex3d(vertices[next][0], vertices[next][1], -0.2);
      glVertex3dv(vertices[next]);
    glEnd();
  }
  
  glPushMatrix();
  glTranslatef(0.0, 0.0, -0.2);
  glCallList(startList);
  glPopMatrix();

  glPopMatrix();

  glutSwapBuffers();
}

static void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-3.0, 3.0, -3.0, 3.0, 3, -3.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void bounce_and_spin() {

  spin = spin + 0.05;
  if (spin > 360.0)
    spin = spin - 360.0;
  glutPostRedisplay();
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Adaptive Methods");

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  //glutIdleFunc(bounce_and_spin);

  glutMainLoop();

  return 0;
}

