#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <IL/il.h>

#define FPS 60

#define PI 3.141592654

#define GRAVITY -0.2f

#define TO_RAD(deg) (PI * (deg) / 180)

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    float x;
    float y;
    float z;

    float angleX;
    float angleY;
    float angleZ;

    int jumping;
    float jumpSpeed;
} Camera;

typedef struct {
    float x;
    float y;
    float z;
} Vector;

Camera camera;
char keys[256];

struct timeval prevTime;
struct timeval currTime;

int prevMouseX;
int prevMouseY;

int mouseInWindow;

GLfloat vertices[] = {
    -1.0f, -1.0f, -10.0f,
    -1.0f, 1.0f, -10.0f,
    1.0f, 1.0f, -10.0f,
    1.0f, -1.0f, -10.0f,

    -1.0f, -1.0f, -12.0f,
    -1.0f, 1.0f, -12.0f,
    -1.0f, 1.0f, -10.0f,
    -1.0f, -1.0f, -10.0f,

    -1.0f, -1.0f, -12.0f,
    1.0f, -1.0f, -12.0f,
    1.0f, -1.0f, -10.0f,
    -1.0f, -1.0f, -10.0f,

    -1.0f, 1.0f, -12.0f,
    1.0f, 1.0f, -12.0f,
    1.0f, 1.0f, -10.0f,
    -1.0f, 1.0f, -10.0f,

    1.0f, -1.0f, -12.0f,
    1.0f, 1.0f, -12.0f,
    1.0f, 1.0f, -10.0f,
    1.0f, -1.0f, -10.0f,

    -1.0f, -1.0f, -12.0f,
    -1.0f, 1.0f, -12.0f,
    1.0f, 1.0f, -12.0f,
    1.0f, -1.0f, -12.0f,

    -1.0f, -1.0f, -15.0f,
    -1.0f, 1.0f, -15.0f,
    1.0f, 1.0f, -15.0f,
    1.0f, -1.0f, -15.0f,

    -1.0f, -1.0f, -17.0f,
    -1.0f, 1.0f, -17.0f,
    -1.0f, 1.0f, -15.0f,
    -1.0f, -1.0f, -15.0f,

    -1.0f, -1.0f, -17.0f,
    1.0f, -1.0f, -17.0f,
    1.0f, -1.0f, -15.0f,
    -1.0f, -1.0f, -15.0f,

    -1.0f, 1.0f, -17.0f,
    1.0f, 1.0f, -17.0f,
    1.0f, 1.0f, -15.0f,
    -1.0f, 1.0f, -15.0f,

    1.0f, -1.0f, -17.0f,
    1.0f, 1.0f, -17.0f,
    1.0f, 1.0f, -15.0f,
    1.0f, -1.0f, -15.0f,

    -1.0f, -1.0f, -17.0f,
    -1.0f, 1.0f, -17.0f,
    1.0f, 1.0f, -17.0f,
    1.0f, -1.0f, -17.0f
};

GLfloat normals[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,

    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,

    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f
};

GLfloat floorVerts[] = {
    -10.0f, -3.0f, -1.0f,
    -10.0f, -3.0f, -30.0f,
    10.0f, -3.0f, -30.0f,
    10.0f, -3.0f, -1.0f,

    -10.0f, -3.0f, -1.0f,
    -10.0f, 5.0f, -1.0f,
    -10.0f, 5.0f, -30.0f,
    -10.0f, -3.0f, -30.0f,

    10.0f, -3.0f, -1.0f,
    10.0f, 5.0f, -1.0f,
    10.0f, 5.0f, -30.0f,
    10.0f, -3.0f, -30.0f,
};

GLfloat floorNormals[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f
};

GLfloat floorColors[] = {
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f
};

GLfloat *terrainVerts;
GLfloat *terrainColors;
GLfloat *terrainNormals;

int imgWidth = 0;
int imgHeight = 0;

float pixelHeight(char *pixels, int width, int x, int y) {
    float height = (float)pixels[y * width + x];

    if (height < 0) {
        height = -height;
    }

    return height;
}

float pixelColor(char *pixels, int width, int x, int y) {
    int height = pixelHeight(pixels, width, x, y);

    return (float)height / 256.0f - 0.1f;

    // return (float)((y * width + x) % 256) / 256;
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    camera.x = 0.0f;
    camera.y = 0.0f;
    camera.z = 0.0f;

    camera.angleX = 0.0f;
    camera.angleY = 0.0f;
    camera.angleZ = 0.0f;

    camera.jumping = FALSE;
    camera.jumpSpeed = 0.0f;

    gettimeofday(&prevTime, NULL);

    prevMouseX = 0;
    prevMouseY = 0;

    mouseInWindow = FALSE;

    ILuint imgID = 0;
    ilGenImages(1, &imgID);
    ilBindImage(imgID);

    ilLoadImage("heightmap_luke.bmp");

    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);

    char pixels[width * height * 3];

    ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, pixels);

    ilBindImage(0);
    ilDeleteImage(imgID);

    imgWidth = width;
    imgHeight = height;

    terrainVerts = (GLfloat *)malloc(sizeof(GLfloat) * (imgWidth) * (imgHeight) * 12);
    terrainColors = (GLfloat *)malloc(sizeof(GLfloat) * (imgWidth) * (imgHeight) * 12);
    terrainNormals = (GLfloat *)malloc(sizeof(GLfloat) * (imgWidth) * (imgHeight) * 12);

    if (!terrainVerts || !terrainColors) {
        fprintf(stderr, "Failed to allocate terrain memory\n");

        exit(1);
    }

    for (int i = 0; i < imgHeight; i++) {
        for (int j = 0; j < imgWidth; j++) {
            float height = 0.15f * pixelHeight(pixels, imgWidth, j, i) - 25.0f;
            terrainVerts[i * imgWidth * 12 + j * 12] = j;
            terrainVerts[i * imgWidth * 12 + j * 12 + 1] = height;
            terrainVerts[i * imgWidth * 12 + j * 12 + 2] = i;

            height = 0.15f * pixelHeight(pixels, imgWidth, j + 1, i) - 25.0f;
            terrainVerts[i * imgWidth * 12 + j * 12 + 3] = j + 1;
            terrainVerts[i * imgWidth * 12 + j * 12 + 4] = height;
            terrainVerts[i * imgWidth * 12 + j * 12 + 5] = i;

            height = 0.15f * pixelHeight(pixels, imgWidth, j + 1, i + 1) - 25.0f;
            terrainVerts[i * imgWidth * 12 + j * 12 + 6] = j + 1;
            terrainVerts[i * imgWidth * 12 + j * 12 + 7] = height;
            terrainVerts[i * imgWidth * 12 + j * 12 + 8] = i + 1;

            height = 0.15f * pixelHeight(pixels, imgWidth, j, i + 1) - 25.0f;
            terrainVerts[i * imgWidth * 12 + j * 12 + 9] = j;
            terrainVerts[i * imgWidth * 12 + j * 12 + 10] = height;
            terrainVerts[i * imgWidth * 12 + j * 12 + 11] = i + 1;

            // printf("(%.3f, %.3f, %.3f, %.3f), (%.3f, %.3f, %.3f, %.3f), (%.3f, %.3f, %.3f, %.3f), (%.3f, %.3f, %.3f, %.3f)\n",
            //     terrainVerts[i * imgWidth * 12 + j * 12], terrainVerts[i * imgWidth * 12 + j * 12 + 1],
            //     terrainVerts[i * imgWidth * 12 + j * 12 + 2], terrainVerts[i * imgWidth * 12 + j * 12 + 3],
            //     terrainVerts[i * imgWidth * 12 + j * 12 + 4], terrainVerts[i * imgWidth * 12 + j * 12 + 5],
            //     terrainVerts[i * imgWidth * 12 + j * 12 + 1],)

            Vector normal;
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;

            normal.x += (terrainVerts[i * imgWidth * 12 + j * 12 + 1] - terrainVerts[i * imgWidth * 12 + j * 12 + 4])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 2] + terrainVerts[i * imgWidth * 12 + j * 12 + 5]);
            normal.y += (terrainVerts[i * imgWidth * 12 + j * 12 + 2] - terrainVerts[i * imgWidth * 12 + j * 12 + 5])
                       *(terrainVerts[i * imgWidth * 12 + j * 12]     + terrainVerts[i * imgWidth * 12 + j * 12 + 3]);
            normal.z += (terrainVerts[i * imgWidth * 12 + j * 12]     - terrainVerts[i * imgWidth * 12 + j * 12 + 3])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 1] + terrainVerts[i * imgWidth * 12 + j * 12 + 4]);

            normal.x += (terrainVerts[i * imgWidth * 12 + j * 12 + 4] - terrainVerts[i * imgWidth * 12 + j * 12 + 7])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 5] + terrainVerts[i * imgWidth * 12 + j * 12 + 8]);
            normal.y += (terrainVerts[i * imgWidth * 12 + j * 12 + 5] - terrainVerts[i * imgWidth * 12 + j * 12 + 8])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 3] + terrainVerts[i * imgWidth * 12 + j * 12 + 6]);
            normal.z += (terrainVerts[i * imgWidth * 12 + j * 12 + 3] - terrainVerts[i * imgWidth * 12 + j * 12 + 6])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 4] + terrainVerts[i * imgWidth * 12 + j * 12 + 7]);

            normal.x += (terrainVerts[i * imgWidth * 12 + j * 12 + 7] - terrainVerts[i * imgWidth * 12 + j * 12 + 10])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 8] + terrainVerts[i * imgWidth * 12 + j * 12 + 11]);
            normal.y += (terrainVerts[i * imgWidth * 12 + j * 12 + 8] - terrainVerts[i * imgWidth * 12 + j * 12 + 11])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 6] + terrainVerts[i * imgWidth * 12 + j * 12 + 9]);
            normal.z += (terrainVerts[i * imgWidth * 12 + j * 12 + 6] - terrainVerts[i * imgWidth * 12 + j * 12 + 9])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 7] + terrainVerts[i * imgWidth * 12 + j * 12 + 10]);

            normal.x += (terrainVerts[i * imgWidth * 12 + j * 12 + 10] - terrainVerts[i * imgWidth * 12 + j * 12 + 1])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 11] + terrainVerts[i * imgWidth * 12 + j * 12 + 2]);
            normal.y += (terrainVerts[i * imgWidth * 12 + j * 12 + 11] - terrainVerts[i * imgWidth * 12 + j * 12 + 2])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 9]  + terrainVerts[i * imgWidth * 12 + j * 12]);
            normal.z += (terrainVerts[i * imgWidth * 12 + j * 12 + 9]  - terrainVerts[i * imgWidth * 12 + j * 12])
                       *(terrainVerts[i * imgWidth * 12 + j * 12 + 10] + terrainVerts[i * imgWidth * 12 + j * 12 + 1]);

            for (int k = 0; k < 4; k++) {
                terrainNormals[i * imgWidth * 12 + j * 12 + 3 * k] = -normal.x;
                terrainNormals[i * imgWidth * 12 + j * 12 + 3 * k + 1] = -normal.y;
                terrainNormals[i * imgWidth * 12 + j * 12 + 3 * k + 2] = -normal.z;
            }

            float color = pixelColor(pixels, imgWidth, j, i);
            terrainColors[i * imgWidth * 12 + j * 12] = 0.0f;
            terrainColors[i * imgWidth * 12 + j * 12 + 1] = color;
            terrainColors[i * imgWidth * 12 + j * 12 + 2] = 0.0f;

            color = pixelColor(pixels, imgWidth, j + 1, i);
            terrainColors[i * imgWidth * 12 + j * 12 + 3] = 0.0f;
            terrainColors[i * imgWidth * 12 + j * 12 + 4] = color;
            terrainColors[i * imgWidth * 12 + j * 12 + 5] = 0.0f;

            color = pixelColor(pixels, imgWidth, j + 1, i + 1);
            terrainColors[i * imgWidth * 12 + j * 12 + 6] = 0.0f;
            terrainColors[i * imgWidth * 12 + j * 12 + 7] = color;
            terrainColors[i * imgWidth * 12 + j * 12 + 8] = 0.0f;

            color = pixelColor(pixels, imgWidth, j, i + 1);
            terrainColors[i * imgWidth * 12 + j * 12 + 9] = 0.0f;
            terrainColors[i * imgWidth * 12 + j * 12 + 10] = color;
            terrainColors[i * imgWidth * 12 + j * 12 + 11] = 0.0f;
        }
    }
}

void moveCamera(Camera *camera, Vector vector) {
    camera->x += vector.x;
    camera->y += vector.y;
    camera->z += vector.z;
}

void rotateCamera(Camera *camera, Vector vector) {
    camera->angleX += vector.x;
    camera->angleY += vector.y;
    camera->angleZ += vector.z;
}

void keyDown(unsigned char key, int x, int y) {
    keys[key] = TRUE;
}

void keyUp(unsigned char key, int x, int y) {
    keys[key] = FALSE;
}

int numQuads = 0;

void update(int ticks) {
    gettimeofday(&currTime, NULL);

    float delta = (currTime.tv_usec - prevTime.tv_usec) / 1000;

    if (delta < 0) {
        delta = 20;
    }

    Vector translateVector;
    Vector rotateVector;
    translateVector.x = 0.0f;
    translateVector.y = 0.0f;
    translateVector.z = 0.0f;
    rotateVector.x = 0.0f;
    rotateVector.y = 0.0f;
    rotateVector.z = 0.0f;

    float moveSpeed = 0.05f * delta;

    if (keys['j']) {
        rotateVector.y -= 3;
    } else if (keys['l']) {
        rotateVector.y += 3;
    }

    if (keys['i']) {
        rotateVector.x -= 3;
    } else if (keys['k']) {
        rotateVector.x += 3;
    }

    rotateCamera(&camera, rotateVector);

    if (keys['w']) {
        translateVector.x -= moveSpeed * sin(TO_RAD(camera.angleY));
        translateVector.z += moveSpeed * cos(TO_RAD(camera.angleY));
    } else if (keys['s']) {
        translateVector.x += moveSpeed * sin(TO_RAD(camera.angleY));
        translateVector.z -= moveSpeed * cos(TO_RAD(camera.angleY));
    }

    if (keys['c']) {
        translateVector.y += moveSpeed;
    } else if (keys['e']) {
        translateVector.y -= moveSpeed;
    }

    if (keys['a']) {
        translateVector.x += moveSpeed * cos(TO_RAD(camera.angleY));
        translateVector.z += moveSpeed * sin(TO_RAD(camera.angleY));
    } else if (keys['d']) {
        translateVector.x -= moveSpeed * cos(TO_RAD(camera.angleY));
        translateVector.z -= moveSpeed * sin(TO_RAD(camera.angleY));
    }

    if (keys['q'] && !camera.jumping) {
        camera.jumpSpeed = 2.8f;
        camera.jumping = TRUE;
    }

    if (camera.jumping) {
        translateVector.y -= camera.jumpSpeed;
        camera.jumpSpeed += GRAVITY;

        if (camera.jumpSpeed < -3.0f) {
            camera.jumpSpeed = 0.0f;
            camera.jumping = FALSE;
        }
    }

    moveCamera(&camera, translateVector);

    prevTime = currTime;

    glutTimerFunc(1000 / FPS, update, ticks + 1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glLoadIdentity();

    glRotatef(camera.angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(camera.angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(camera.angleZ, 0.0f, 0.0f, 1.0f);
    glTranslatef(camera.x, camera.y, camera.z);

    GLfloat position[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, position);

    // glColorPointer(3, GL_FLOAT, 0, floorColors);
    // glVertexPointer(3, GL_FLOAT, 0, floorVerts);
    // glNormalPointer(GL_FLOAT, 0, floorNormals);
    // glDrawArrays(GL_QUADS, 0, 12);

    // glColorPointer(3, GL_FLOAT, 0, colors);
    // glVertexPointer(3, GL_FLOAT, 0, vertices);
    // glNormalPointer(GL_FLOAT, 0, normals);
    // glDrawArrays(GL_QUADS, 0, 48);

    glColorPointer(3, GL_FLOAT, 0, terrainColors);
    glVertexPointer(3, GL_FLOAT, 0, terrainVerts);
    glNormalPointer(GL_FLOAT, 0, terrainNormals);
    glDrawArrays(GL_POINTS, 0, 4 * (imgWidth) * (imgHeight));

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -h / (10.0 * w), h / (10.0 * w), 0.2, 9999999.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    glutInitWindowPosition(0, 0);

    glutCreateWindow("Test");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(1000 / FPS, update, 0);
    glutIdleFunc(display);

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    ilInit();

    // glutEntryFunc(mouseEnter);

    // glutMotionFunc(mouseMove);
    // glutPassiveMotionFunc(mouseMove);

    init();

    glutMainLoop();

    return 0;
}
