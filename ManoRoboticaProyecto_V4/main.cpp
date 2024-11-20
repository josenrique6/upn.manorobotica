#include <GLFW/glfw3.h>// Librería para crear una ventana y controlar gráficos 3D.
#include <cmath> // Librería para funciones matemáticas esenciales.
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"  // Biblioteca para cargar imágenes como texturas

/*
  ============================ LEYENDA DE TECLAS - MANO ROBÓTICA ============================

  CONTROLES DE LOS DEDOS:
  - I + S: Flexionar dedo medio hacia abajo
  - I + W: Flexionar dedo medio hacia arriba
  - U + S: Flexionar dedo índice hacia abajo
  - U + W: Flexionar dedo índice hacia arriba
  - O + S: Flexionar dedo anular hacia abajo
  - O + W: Flexionar dedo anular hacia arriba
  - P + S: Flexionar dedo meñique hacia abajo
  - P + W: Flexionar dedo meñique hacia arriba
  - Y + S: Flexionar dedo pulgar hacia abajo
  - Y + W: Flexionar dedo pulgar hacia arriba

  CONTROLES DE LA CÁMARA:
  - Flecha Izquierda (←): Rotar la cámara hacia la izquierda
  - Flecha Derecha (→): Rotar la cámara hacia la derecha
  - Flecha Arriba (↑): Rotar la cámara hacia arriba
  - Flecha Abajo (↓): Rotar la cámara hacia abajo
  - Scroll del Ratón: Acercar/Alejar (Zoom In/Out)

  ============================================================================================
*/

GLuint texturaManoRobotica;  // Textura para la mano robótica
float zoom = -1.9f;  // Control de zoom, inicializado con un valor predeterminado

// Función para cargar una textura desde archivo
bool cargarTextura(const char* ruta, GLuint& texturaID) {
    int ancho, alto, canales;
    // Cargar la imagen desde el archivo y obtener datos como ancho, alto y número de canales.
    unsigned char* data = stbi_load(ruta, &ancho, &alto, &canales, 0);
    if (!data) return false;

    // Generar y enlazar una textura en OpenGL.
    glGenTextures(1, &texturaID);
    glBindTexture(GL_TEXTURE_2D, texturaID);

    // Configura los parámetros de la textura.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Especifica el formato de la textura según el número de canales.
    if (canales == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (canales == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ancho, alto, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);// Libera la memoria de la imagen cargada.
    return true;// Indica que la textura se cargó correctamente.
}

// Configuración de OpenGL para 3D
bool inicializarOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Configuración de la luz difusa
    GLfloat luzDifusa[] = { 0.6f, 0.6f, 0.6f, 1.0f }; // Menor intensidad de luz difusa
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);

    // Configuración de la luz ambiental
    GLfloat luzAmbiental[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // Luz ambiental más tenue
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiental);

    // Posición de la luz
    GLfloat posicionLuz[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz);

    // Configuración del color de fondo beige
    glClearColor(0.96f, 0.96f, 0.86f, 1.0f);

    return true;
}

// Función para capturar el desplazamiento de la rueda del mouse y ajustar el zoom
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    zoom += yoffset * 0.1f;  // Ajusta la velocidad del zoom según el desplazamiento
    if (zoom > -1.0f) zoom = -1.0f;  // Límite máximo de zoom
    if (zoom < -10.0f) zoom = -10.0f;  // Límite mínimo de zoom
}

// Función para dibujar un cubo con o sin textura
void dibujarCubo(float r, float g, float b, float scaleX, float scaleY, float scaleZ, bool textura = false, GLuint texturaID = 0) {
    glPushMatrix();
    glScalef(scaleX, scaleY, scaleZ);

    if (textura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaID);
    }
    else {
        glColor3f(r, g, b);
    }

    glBegin(GL_QUADS);

    // Definir las caras del cubo
    // Cara frontal
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Cara trasera
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Cara izquierda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Cara derecha
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Cara superior
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Cara inferior
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();

    if (textura) glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Función para dibujar un cubo sin iluminación (mantener color constante)
void dibujarCuboSinIluminacion(float r, float g, float b, float scaleX, float scaleY, float scaleZ) {
    glPushMatrix();
    glDisable(GL_LIGHTING);  // Desactivar iluminación para mantener el color constante
    glColor3f(r, g, b);
    glScalef(scaleX, scaleY, scaleZ);

    glBegin(GL_QUADS);

    // Definir las caras del cubo (sin normales ni iluminación)
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();
    glEnable(GL_LIGHTING);  // Reactivar iluminación para otras partes
    glPopMatrix();
}

// Función para dibujar un dedo humano con el exoesqueleto texturizado detrás
void dibujarDedoManoRobotica(float anguloBase, float posX, int segmentos, float* angulosSegmentos, float largoDedo, int dedoIndex) {
    glPushMatrix();
    glTranslatef(posX, 0.6f, 0.0f);
    glRotatef(anguloBase, 1.0f, 0.0f, 0.0f);

    for (int i = 0; i < segmentos; ++i) {
        dibujarCuboSinIluminacion(0.9f, 0.7f, 0.5f, 0.1f, largoDedo, 0.1f);

        // Exoesqueleto texturizado en la parte posterior con grosor reducido
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -0.1f);  // Ajuste de separación mínima
        dibujarCubo(1.0f, 1.0f, 1.0f, 0.12f, largoDedo + 0.05f, 0.1f, true, texturaManoRobotica); // Grosor reducido a 0.1f
        glPopMatrix();

        glTranslatef(0.0f, largoDedo, 0.0f);
        glRotatef(angulosSegmentos[dedoIndex], 1.0f, 0.0f, 0.0f);
    }
    glPopMatrix();
}

// Función para dibujar la mano con exoesqueleto
void dibujarManoRobotica(float* angulosDedo, float* angulosPulgar) {
    glPushMatrix();
    glTranslatef(0.0f, -0.2f, 0.0f);

    dibujarCuboSinIluminacion(0.9f, 0.7f, 0.5f, 0.9f, 0.8f, 0.2f);

    // Base posterior de la palma con textura
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.1f);
    dibujarCubo(1.0f, 1.0f, 1.0f, 0.95f, 0.8f, 0.1f, true, texturaManoRobotica);
    glPopMatrix();

    // Dibujar los dedos principales
    float largosDedos[4] = { 0.3f, 0.35f, 0.32f, 0.28f };
    for (int i = 0; i < 4; i++) {
        dibujarDedoManoRobotica(angulosDedo[i], (i - 1.5f) * 0.3f, 3, angulosDedo, largosDedos[i], i);
    }

    // Dibujar el pulgar
    glPushMatrix();
    glTranslatef(-0.4f, 0.0f, 0.1f);
    glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
    dibujarDedoManoRobotica(angulosPulgar[0], 0.0f, 2, angulosPulgar, 0.25f, 0);
    glPopMatrix();

    glPopMatrix();
}

// Función principal del programa.
int main() {
    // Inicializa GLFW y verificar si se pudo iniciar correctamente.
    if (!glfwInit()) return -1;// Salir del programa si no se inicializó.

    // Crear una ventana con un contexto de OpenGL.
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Simulación de ManoRobótica para Rehabilitación", NULL, NULL);
    if (!window) {// Si no se pudo crear la ventana:
        glfwTerminate();// Finalizar GLFW y liberar recursos
        return -1;
    }
    glfwMakeContextCurrent(window);// Establece el contexto de OpenGL para la ventana.

    inicializarOpenGL();
    // Configura la función de callback para capturar eventos de scroll.
    glfwSetScrollCallback(window, scrollCallback);

    if (!cargarTextura("texturaCromo.jpg", texturaManoRobotica)) {
        printf("Error al cargar la textura de la ManoRobótica\n");
        return -1;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = 1200.0f / 800.0f;
    glFrustum(-aspect, aspect, -1.0f, 1.0f, 1.0f, 10.0f);

    float angulosDedo[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float angulosPulgar[2] = { 0.0f, 0.0f };
    float anguloRotacionX = 0.0f;
    float anguloRotacionY = 0.0f;
    float velocidadRotacion = 0.1f; // Ajusta este valor para cambiar la velocidad de rotación
    float incrementoFlexion = 0.1f; // Ajusta este valor para cambiar la velocidad de flexión de los dedos

    // Bucle principal para renderizar continuamente hasta que se cierre la ventana.
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, -0.4f, zoom);

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) anguloRotacionY -= velocidadRotacion;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) anguloRotacionY += velocidadRotacion;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) anguloRotacionX += velocidadRotacion;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) anguloRotacionX -= velocidadRotacion;

        // Detectar la combinación de teclas I + S para bajar el dedo medio
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            angulosDedo[1] += incrementoFlexion;
            if (angulosDedo[1] > 90.0f) angulosDedo[1] = 90.0f; // Limitar la flexión máxima a 90 grados
        }

        // Detectar la combinación de teclas I + W para subir el dedo medio
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            angulosDedo[1] -= incrementoFlexion;
            if (angulosDedo[1] < 0.0f) angulosDedo[1] = 0.0f; // Limitar la extensión mínima a 0 grados
        }

        // Detectar la combinación de teclas U + S para bajar el dedo índice
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            angulosDedo[0] += incrementoFlexion;
            if (angulosDedo[0] > 90.0f) angulosDedo[0] = 90.0f; // Limitar la flexión máxima a 90 grados
        }

        // Detectar la combinación de teclas U + W para subir el dedo índice
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            angulosDedo[0] -= incrementoFlexion;
            if (angulosDedo[0] < 0.0f) angulosDedo[0] = 0.0f; // Limitar la extensión mínima a 0 grados
        }

        // Detectar la combinación de teclas O + S para bajar el dedo anular
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            angulosDedo[2] += incrementoFlexion;
            if (angulosDedo[2] > 90.0f) angulosDedo[2] = 90.0f; // Limitar la flexión máxima a 90 grados
        }

        // Detectar la combinación de teclas O + W para subir el dedo anular
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            angulosDedo[2] -= incrementoFlexion;
            if (angulosDedo[2] < 0.0f) angulosDedo[2] = 0.0f; // Limitar la extensión mínima a 0 grados
        }

        // Detectar la combinación de teclas P + S para bajar el dedo meñique
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            angulosDedo[3] += incrementoFlexion;
            if (angulosDedo[3] > 90.0f) angulosDedo[3] = 90.0f; // Limitar la flexión máxima a 90 grados
        }

        // Detectar la combinación de teclas P + W para subir el dedo meñique
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            angulosDedo[3] -= incrementoFlexion;
            if (angulosDedo[3] < 0.0f) angulosDedo[3] = 0.0f; // Limitar la extensión mínima a 0 grados
        }

        // Detectar la combinación de teclas Y + S para bajar el dedo pulgar
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            angulosPulgar[0] += incrementoFlexion;
            if (angulosPulgar[0] > 90.0f) angulosPulgar[0] = 90.0f; // Limitar la flexión máxima a 90 grados
        }

        // Detectar la combinación de teclas Y + W para subir el dedo pulgar
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            angulosPulgar[0] -= incrementoFlexion;
            if (angulosPulgar[0] < 0.0f) angulosPulgar[0] = 0.0f; // Limitar la extensión mínima a 0 grados
        }


        glRotatef(anguloRotacionX, 1.0f, 0.0f, 0.0f);
        glRotatef(anguloRotacionY, 0.0f, 1.0f, 0.0f);

        dibujarManoRobotica(angulosDedo, angulosPulgar);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();// Finalizar GLFW y liberar todos los recursos.
    return 0;// Salir del programa con éxito.
}