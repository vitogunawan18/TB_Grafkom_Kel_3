#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float angle = 0.0f;   
float wheelAngle = 0.0f; // Rotation angle for the wheel
bool hidden = false;
float xf = 0.0f;
float yf = 0.0f;
float cameraDistance = 20.0f;
float cameraAngle=0.0f;
GLuint tekstur_jalan;
//vito Gunawan
// Fungsi untuk memuat tekstur dari file gambar
GLuint textureID = 0;
GLuint loadTexture(const char* path) {
    glGenTextures(1, &textureID);
    void* imgData;
    int imgWidth;
    int imgHeight;
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path);
    if (format == FIF_UNKNOWN) {
        printf("Unknown file type for texture image file %s\n", path);
        return -1;
    }
    FIBITMAP* bitmap = FreeImage_Load(format, path, 0);
    if (!bitmap) {
        printf("Failed to load image %s\n", path);
        return -1;
    }
    FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);
    FreeImage_Unload(bitmap);
    imgData = FreeImage_GetBits(bitmap2);
    imgWidth = FreeImage_GetWidth(bitmap2);
    imgHeight = FreeImage_GetHeight(bitmap2);
    if (imgData) {
        printf("Texture image loaded from file %s, size %dx%d\n", path, imgWidth, imgHeight);
        int format;
        if (FI_RGBA_RED == 0)
            format = GL_RGB;
        else
            format = GL_BGR;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, imgData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        textureID++;
        return textureID - 1;
    } else {
        printf("Failed to get texture data from %s\n", path);
    }
    return -1;
}
void drawLamp(float x, float y, float z) {
    // Menggambar bagian lampu utama
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(3.0, 50, 50); // Lampu utama
    glPopMatrix();
}
// Fungsi untuk menggambar sisi 
void sisi_samping()
{
    // Bagian kiri
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(0, -20, 20);
    glScalef(1.5, 0.3, 0.01);
    glutSolidCube(60);
    glColor3f(1, 1, 1);
    glutWireCube(60);
    glPopMatrix();

    // Menggambar cermin kiri
    glColor3f(0.447, 0.04, 0.01);
    glPushMatrix();
    glTranslatef(-24, -13, 25);
    glRotatef(15, 0, 1, 0);  // Rotasi sebesar 15 derajat
    glScalef(0.025, 0.08, 0.1);
    glutSolidCube(60);
    glPopMatrix();

    // Menggambar gagang cerrmin
    glColor3f(0.447, 0.04, 0.01);
    glPushMatrix();
    glTranslatef(-24.5, -13, 22);
    glScalef(0.02, 0.03, 0.05);
    glutSolidCube(60);
    glPopMatrix();

    // Bagian kanan
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(0, -20, -20);
    glScalef(1.5, 0.3, 0.01);
    glutSolidCube(60);
    glColor3f(1, 1, 1);
    glutWireCube(60);
    glPopMatrix();

    // Menggambar cermin kanan
    glColor3f(0.447, 0.04, 0.01);
    glPushMatrix();
    glTranslatef(-24, -13, -25);
    glRotatef(-15, 0, 1, 0);  // Rotasi sebesar -15 derajat
    glScalef(0.025, 0.08, 0.1);
    glutSolidCube(60);
    glPopMatrix();

    // Menggambar gagang cermin
    glColor3f(0.447, 0.04, 0.01);
    glPushMatrix();
    glTranslatef(-24.5, -13, -22);
    glScalef(0.02, 0.03, 0.05);
    glutSolidCube(60);
    glPopMatrix();
}

// Menggabungkan fungsi atap dan bawah
void atas_bawah()
{
    // Bagian atap
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(11, 6, 0);
    glScalef(0.7, 0.01, 0.666667);
    glutSolidCube(60);
    glColor3f(1, 1, 1);
    glutWireCube(60);
    glPopMatrix();

    

    // Menggambar pintu samping kiri
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(11, -20, 20);
    glScalef(0.01, 0.3, 0.01);
    glutWireCube(60);
    glPopMatrix();
    //pintu samping kanan
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(11, -20, -20);
    glScalef(0.01, 0.3, 0.01);
    glutWireCube(60);
    glPopMatrix();

    // Bagian bawah
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(0, -29, 0);
    glScalef(1.5, 0.01, 0.666667);
    glutSolidCube(60);
    glColor3f(1, 1, 1);
    glutWireCube(60);
    glPopMatrix();
}

void depan_belakang()
{
    // Bagian depan
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(-45, -20, 0);
    glScalef(0.01, 0.3, 0.666667);
    glutSolidCube(60);
    glColor3f(1, 1, 1);
    glutWireCube(60);
    glPopMatrix();

    // Bagian atas 
    glColor3f(0 , 0, 1);
    glPushMatrix();
    glTranslatef(-36, -11, 0);
    glScalef(0.3, 0.01, 0.666667);
    glutSolidCube(60);
    glColor3f(1, 1, 1);
    glutWireCube(60);
    glPopMatrix();

    // Bagian dekoratif depan
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(-18, -2.5, 0);
    glRotatef(-45, 0, 0, 1);  // Rotasi sebesar -45 derajat
    glScalef(0.002, 0.4, 0.666667);
    glutWireCube(60);
    glPopMatrix();

    // Bagian belakang
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(45, -20, 0);
    glScalef(0.01, 0.3, 0.666667);
    glutSolidCube(60);
    glColor3f(1, 1, 1);
    glutWireCube(60);
    glPopMatrix();

    // Bagian dekoratif belakang
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(38.5, -2, 0);
    glRotatef(36, 0, 0, 1);  // Rotasi sebesar 36 derajat
    glScalef(0.01, 0.35, 0.666667);
    glutWireCube(60); 
    glPopMatrix();

    // Menggambar lampu depan
    drawLamp(-45, -15, 12); // Lampu depan kiri
    drawLamp(-45, -15, -12); // Lampu depan kanan
    //lampu belakang
    drawLamp(45, -15, 12); // Lampu belakang kiri
    drawLamp(45, -15, -12); // Lampu belakang kanan
}

void jalan() {
    glColor3f(0.3f, 0.3f, 0.3f); // Warna abu-abu untuk jalan
    glPushMatrix();
    glTranslatef(0.0f, -40.0f, 0.0f); // Posisi jalan di bawah mobil

    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tekstur_jalan); // Assuming textureID is the ID of the loaded texture

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, 0.0f, -50.0f); // Titik kiri-bawah
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1000.0f, 0.0f, -50.0f);  // Titik kanan-bawah
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1000.0f, 0.0f, 50.0f);   // Titik kanan-atas
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f, 0.0f, 50.0f);  // Titik kiri-atas
    glEnd();
    // Disable texturing
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
void tiangListrik(float x, float z) {
    // Tiang vertikal
    glColor3ub(114, 125, 115);
    glPushMatrix();
    glTranslatef(x, -40.0f, z);  // Posisi dasar tiang
    GLUquadric *quad = gluNewQuadric();
    glRotatef(-90, 1, 0, 0);     // Rotasi untuk orientasi vertikal
    gluCylinder(quad, 2.0, 2.0, 90.0, 30, 30); // Tiang utama
    gluDeleteQuadric(quad);
    glPopMatrix();

    /// Tiang horizontal (solid cube)
    glPushMatrix();
    glTranslatef(x, 7.0f, z);   // Posisi untuk tiang horizontal di tengah tiang vertikal
    glColor3ub(114, 125, 115);
    glRotatef(60, 0, 1, 0); 

    // Tiang horizontal ke kanan
    glPushMatrix();
    glTranslatef(10.0f, 35.0f, 0.0f); // Pindah ke kanan dari pusat tiang
    glScalef(15.0f, 1.0f, 1.0f);     // Skala untuk memperpanjang kubus menjadi seperti balok horizontal
    glutSolidCube(1.0f);             // Kubus solid dengan skala
    glPopMatrix();

    // Tiang horizontal ke kiri
    glPushMatrix();
    glTranslatef(-10.0f, 35.0f, 0.0f); // Pindah ke kiri dari pusat tiang
    glScalef(15.0f, 1.0f, 1.0f);      // Skala untuk memperpanjang kubus menjadi seperti balok horizontal
    glutSolidCube(1.0f);              // Kubus solid dengan skala
    glPopMatrix();

    glPopMatrix();
}
//fadhlian 
void myKeyboard(unsigned char key,int x,int y)
{
    float jarakMaksimum = 500.0f;
    float angleStep = 3.0f;   // Langkah rotasi kamera
    float zoomStep = 20.0f;   // Langkah zoom
    float minZoomIn = 200.0f;   // Jarak minimum dekat 
    float maxZoomOut = 350.0f;  // Jarak maksimum jauh

    switch (key)
    {
    case 27:
            exit(0);
            break;
    case 'q':
        hidden = !hidden; // Toggle garis bantu
        break;

    case 'w': // Gerakkan mobil maju
        xf -= cos(angle * M_PI / 180.0f) * 5.0f;
        yf -= sin(angle * M_PI / 180.0f) * 5.0f;

        if (xf > jarakMaksimum)
            xf = -jarakMaksimum;
        else if (xf < -jarakMaksimum)
            xf = jarakMaksimum;
        break;

    case 's': // Gerakkan mobil mundur
        xf += cos(angle * M_PI / 180.0f) * 5.0f;
        yf += sin(angle * M_PI / 180.0f) * 5.0f;

        if (xf > jarakMaksimum)
            xf = -jarakMaksimum;
        else if (xf < -jarakMaksimum)
            xf = jarakMaksimum;
        break;

    case 'a': // Rotasi kamera ke kiri
        cameraAngle -= angleStep;
        if (cameraAngle < 0.0f)
            cameraAngle += 360.0f; // Jaga sudut dalam rentang [0, 360]
        break;

    case 'd': // Rotasi kamera ke kanan
        cameraAngle += angleStep;
        if (cameraAngle >= 360.0f)
            cameraAngle -= 360.0f; // Jaga sudut dalam rentang [0, 360]
        break;

    case '+': // Zoom in (kamera mendekat)
        cameraDistance -= zoomStep;
        if (cameraDistance < minZoomIn)
            cameraDistance = minZoomIn; // Batasi jarak minimum
        break;

    case '-': // Zoom out (kamera menjauh)
        cameraDistance += zoomStep;
        if (cameraDistance > maxZoomOut)
            cameraDistance = maxZoomOut; // Batasi jarak maksimum
        break;
    }
	glutPostRedisplay();
}
void cartecius()
{
    glColor3f(1.0, 0, 0);
    glBegin(GL_LINES);
    // Garis x
    glVertex3f(-900.0, 0.0, 0.0);
    glVertex3f(900.0, 0.0, 0.0);
    // Garis y
    glVertex3f(0.0, -900.0, 0.0);
    glVertex3f(0.0, 900.0, 0.0);
    // Garis z
    glVertex3f(0.0, 0.0, -900.0);
    glVertex3f(0.0, 0.0, 900.0);
    glEnd();
}
void hiddencarte()
{
	if (hidden)
	{
		cartecius();
	}
}
void addGlass()
{
    //Depan
    glColor3f(0.0f, 0.0f, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-27.9f, -13.0f, -20.0f); // Titik kiri bawah
    glVertex3f(-27.9f, -13.0f, 20.0f);  // Titik kanan bawah
    glVertex3f(-8.5f, 5.8f, 20.0f);  // Titik kanan atas
    glVertex3f(-8.5f, 5.8f, -20.0f); // Titik kiri atas
    glEnd();
    //kiri
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(32.0f, -11.0f, 20.0f); // Titik kanan bawah
    glVertex3f(32.0f, 6.0f, 20.0f); // Titik kana atas
    glVertex3f(-8.5f, 6.0f, 20.0f); // Titik kiri atas
    glVertex3f(-8.5f, -11.0f, 20.0f); // Titik kiri bawah
    glEnd();

    //kanan
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(32.0f, -11.0f, -20.0f); // Titik kanan bawah
    glVertex3f(32.0f, 6.0f, -20.0f); // Titik kanan atas
    glVertex3f(-8.5f, 6.0f, -20.0f); // Titik kiri atas
    glVertex3f(-8.5f, -11.0f, -20.0f); // Titik kiri bawah
    glEnd();

    // Back Glass (Belakang)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(45.0f, -11.0f, -20.0f); // Titik kiri bawah
    glVertex3f(45.0f, -11.0f, 20.0f);  // Titik kanan bawah
    glVertex3f(32.0f, 6.2f, 20.0f);  // Titik kiri atas
    glVertex3f(32.0f, 6.2f, -20.0f); // Titik kanan atas
    glEnd();
    
    // kiri belakang
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(32, 6.5, 20);  // Titik atas (kiri atas, sudut siku-siku)
    glVertex3f(32, -11, 20); // Titik bawah (vertikal)
    glVertex3f(45, -11, 20); // Titik kanan (horizontal)
    glEnd();

    // kiri depan
    glBegin(GL_TRIANGLES);
    glVertex3f(-8.5, 6.5, 20);  // Titik atas 
    glVertex3f(-8.5, -11, 20); // Titik bawah 
    glVertex3f(-26, -11, 20); // Titik kanan 
    glEnd();

    //kanan depan
    glBegin(GL_TRIANGLES);
    glVertex3f(-26, -11, -20); // Titik kanan 
    glVertex3f(-8.5, -11, -20); // Titik bawah 
    glVertex3f(-8.5, 6.8, -20);   // Titik atas 
    glEnd();
    
    //kanan belakang
    glBegin(GL_TRIANGLES);
    glVertex3f(32, 6.8, -20);  // Titik atas
    glVertex3f(32, -11, -20); // Titik bawah
    glVertex3f(45, -11, -20); // Titik kanan 
    glEnd();
}
void wheel(int x, int y, int z)
{
    glColor3f(0.0f, 0.0f, 0.0f); // Set wheel color to black

    // Outer wheel
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(wheelAngle, 0, 0, 1); // Rotate the wheel
    glutSolidTorus(2, 6, 50, 50);
    glPopMatrix();

    // Inner spokes
    glColor3f(0.7f, 0.7f, 0.7f); // Set spoke color to light gray
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(wheelAngle, 0, 0, 1); // Rotate spokes with the wheel

    for (int i = 0; i < 360; i += 45) // Add spokes at intervals of 45 degrees
    {
        glPushMatrix();
        glRotatef(i, 0, 0, 1); // Rotate each spoke to the correct angle
        glScalef(1.0, 0.1, 1.0); // Scale the spoke
        glutSolidCube(5); // Create the spoke
        glPopMatrix();
    }
    glPopMatrix();
}
void mobil()
{
	glPushMatrix();
    glTranslatef(xf, yf, 0); // Apply translation
    glRotatef(angle, 0, 0, 1); 
    glScalef(1, 1, 1); // Apply scaling

	
    sisi_samping();
    atas_bawah();
    depan_belakang();
    addGlass();
	hiddencarte();
    wheel(-33, -33, 15);
    wheel(33, -33, 15);
    wheel(-33, -33, -15);
    wheel(33, -33, -15);
    
    glPopMatrix();
}

//kayla
void kabelMelintang(float x1, float z1, float x2, float z2) {
    glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam untuk kabel
    glBegin(GL_LINES);
    glVertex3f(x1, 42.0f, z1); // Ujung kanan tiang pertama
    glVertex3f(x2 , 42.0f, z2); // Ujung kiri tiang kedua
    glEnd();
}
void init()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(19.0f / 255.0f, 75.0f / 255.0f, 112.0f / 255.0f, 1.0f);
    // Set up the projection matrix (perspective)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
    //mengembalikan ke modelview matrix
    glMatrixMode(GL_MODELVIEW);
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    // Set light properties
    GLfloat light_pos[] = {300.0f, 500.0f, 500.0f, 1.0f};
    GLfloat light_amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light_diff[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light_spec[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
}

void display()
{
    wheelAngle += 2.0; // Adjust the wheel rotation speed
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    float cameraX = cameraDistance * cos(cameraAngle * M_PI / 180.0f);
    float cameraZ = cameraDistance * sin(cameraAngle * M_PI / 180.0f);

    // Atur posisi kamera menggunakan gluLookAt
    gluLookAt(cameraX, 120.0f, cameraZ, // Posisi kamera (X, Y, Z)
              0.0f, 0.0f, 0.0f,         // Titik fokus kamera
              0.0f, 1.0f, 0.0f);         // Up vector (Y-axis)
    jalan();
     // Gambar tiang listrik  vito
    tiangListrik(-500.0f, -40.0f);
    tiangListrik(-200.0f, -40.0f);
    tiangListrik(0.0f, -40.0f);
    tiangListrik(200.0f, -40.0f);
    tiangListrik(400.0f, -40.0f);
    tiangListrik(700.0f, -40.0f);

    // Gambar kabel melintang kanan
    kabelMelintang(-520.0f, -27.0f, 700.0f, -27.0f);
    // Gambar kabel melintang kiri
    kabelMelintang(-500.0f, -52.0f, 720.0f, -52.0f);
    // Mobil digambar tanpa dipengaruhi oleh rotasi kamera
    glPushMatrix();
    glTranslatef(xf, yf, 0);  // Apply translation
    glScalef(1.0, 1.0, 1.0); // Apply scaling
    mobil(); // Gambar mobil
    glPopMatrix();
    
    glutSwapBuffers();
    glutPostRedisplay();

}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Tugas Besar_Mobil");
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glewInit();

    // Load texture Vito
    tekstur_jalan = loadTexture("jalan.png");

    glutFullScreen(); 
    init();
    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST);
    glutKeyboardFunc(myKeyboard);
    
    glutMainLoop();

    return 0;
}
