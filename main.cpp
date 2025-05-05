//Alunos:
// Raimundo Ferreira do Nasscimento Junior
// Carlos Eduardo

#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265358979323846

// Estruturas de dados
typedef struct {
    float comprimento;
    float largura;
    float angulo;
    float cor[3];
} SegmentoBraco;

typedef struct {
    float abertura;
    float comprimento;
    float largura;
    float angulo;
    float cor[3];
} Garra;

// Variáveis globais
SegmentoBraco base, braco, antebraco;
Garra garra;
float posicaoBase[2] = {0.0, 0.0};
int segmentoAtual = 0; // 0-base, 1-braço, 2-antebraço, 3-garra

// Configurações iniciais
void inicializar() {
    // Base
    base.comprimento = 0.6;
    base.largura = 0.2;
    base.angulo = 0.0;
    base.cor[0] = 0.3; base.cor[1] = 0.3; base.cor[2] = 0.3;
    
    // Braço
    braco.comprimento = 1.2;
    braco.largura = 0.2;
    braco.angulo = 90.0;
    braco.cor[0] = 0.8; braco.cor[1] = 0.2; braco.cor[2] = 0.2;
    
    // Antebraço
    antebraco.comprimento = 1.0;
    antebraco.largura = 0.17;
    antebraco.angulo = 45.0;
    antebraco.cor[0] = 0.2; antebraco.cor[1] = 0.2; antebraco.cor[2] = 0.8;
    
    // Garra
    garra.abertura = 45.0;
    garra.comprimento = 0.6;
    garra.largura = 0.1;
    garra.angulo = -45.0;
    garra.cor[0] = 0.8; garra.cor[1] = 0.8; garra.cor[2] = 0.2;
}

// Desenha um segmento cilíndrico (retângulo com extremidades arredondadas)
void desenharSegmentoCilindrico(float comprimento, float largura, float cor[3]) {
    const int segmentos = 20;
    float raio = largura/2;
    float meioComprimento = comprimento/2;
    
    glColor3fv(cor);
    
    // Corpo principal (retângulo)
    glBegin(GL_QUADS);
        glVertex2f(-meioComprimento, -raio);
        glVertex2f(meioComprimento, -raio);
        glVertex2f(meioComprimento, raio);
        glVertex2f(-meioComprimento, raio);
    glEnd();
    
    // Extremidade esquerda (semicírculo)
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-meioComprimento, 0);
        for(int i = 0; i <= segmentos; i++) {
            float angulo = M_PI/2 + M_PI * i / segmentos;
            glVertex2f(-meioComprimento + cos(angulo) * raio, 
                       sin(angulo) * raio);
        }
    glEnd();
    
    // Extremidade direita (semicírculo)
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(meioComprimento, 0);
        for(int i = 0; i <= segmentos; i++) {
            float angulo = -M_PI/2 + M_PI * i / segmentos;
            glVertex2f(meioComprimento + cos(angulo) * raio, 
                       sin(angulo) * raio);
        }
    glEnd();
}

// Desenha a garra
void desenharGarra(Garra g) {
    float comprimento = g.comprimento;
    float largura = g.largura;
    float angulo = g.angulo;
    float aberturaRad = g.abertura * M_PI / 180.0;
    
    glPushMatrix();
        glColor3fv(g.cor);
        
        // Parte fixa da garra
        glBegin(GL_QUADS);
            glVertex2f(0, -largura/2);
            glVertex2f(comprimento/2, -largura/2);
            glVertex2f(comprimento/2, largura/2);
            glVertex2f(0, largura/2);
        glEnd();
        
        // Garra esquerda
        glPushMatrix();
            glTranslatef(comprimento/2, 0, 0);
            glRotatef(g.abertura, 0, 0, 1);
            glBegin(GL_QUADS);
                glVertex2f(0, -largura/4);
                glVertex2f(comprimento/2, -largura/4);
                glVertex2f(comprimento/2, largura/4);
                glVertex2f(0, largura/4);
            glEnd();
        glPopMatrix();
        
        // Garra direita
        glPushMatrix();
            glTranslatef(comprimento/2, 0, 0);
            glRotatef(-g.abertura, 0, 0, 1);
            glBegin(GL_QUADS);
                glVertex2f(0, -largura/4);
                glVertex2f(comprimento/2, -largura/4);
                glVertex2f(comprimento/2, largura/4);
                glVertex2f(0, largura/4);
            glEnd();
        glPopMatrix();
    glPopMatrix();
}

// Função de desenho principal
void desenhar() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    // Desenha a base
    glPushMatrix();
        glTranslatef(posicaoBase[0], posicaoBase[1], 0.0);
        glRotatef(base.angulo, 0, 0, 1);
        desenharSegmentoCilindrico(base.comprimento, base.largura, base.cor);
        
        // Desenha o braço (articulação no topo da base)
        glPushMatrix();
            glTranslatef(0, base.largura/2, 0);
            glRotatef(braco.angulo, 0, 0, 1);
            glTranslatef(braco.comprimento/2, 0, 0);
            desenharSegmentoCilindrico(braco.comprimento, braco.largura, braco.cor);
            
            // Desenha o antebraço (articulação na extremidade do braço)
            glPushMatrix();
                glTranslatef(braco.comprimento/2, 0, 0);
                glRotatef(antebraco.angulo, 0, 0, 1);
                glTranslatef(antebraco.comprimento/2, 0, 0);
                desenharSegmentoCilindrico(antebraco.comprimento, antebraco.largura, antebraco.cor);
                
                // Desenha a garra (na extremidade do antebraço)
                glPushMatrix();
                    glTranslatef(antebraco.comprimento/2, 0, 0);
                    glRotatef(garra.angulo, 0, 0, 1);
                    desenharGarra(garra);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    
    // Exibe instruções na tela
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glColor3f(0.0, 0.0, 0.0);
            glRasterPos2i(10, glutGet(GLUT_WINDOW_HEIGHT) - 20);
            
            const char* instrucoes = "Controles: 1-Base 2-Braco 3-Antebraco 4-Garra | Teclas: A/D-Rotaciona, A/D/W/S-Move Base, Mouse-Abertura Garra";
            for (const char* c = instrucoes; *c != '\0'; c++) {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
            }
            
            char segmento[50];
            const char* nomes[] = {"Base", "Braco", "Antebraco", "Garra"};
            sprintf(segmento, "Segmento atual: %s", nomes[segmentoAtual]);
            
            glRasterPos2i(10, glutGet(GLUT_WINDOW_HEIGHT) - 40);
            for (const char* c = segmento; *c != '\0'; c++) {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
            }
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glutSwapBuffers();
}

// Função de redimensionamento
void redimensionar(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / (float)h;
    
    if (w <= h) {
        gluOrtho2D(-3.0, 3.0, -3.0/aspect, 3.0/aspect);
    } else {
        gluOrtho2D(-3.0*aspect, 3.0*aspect, -3.0, 3.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
}
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            // Botão esquerdo abre a garra
            garra.abertura = fmax(garra.abertura - 5.0, 0.0);
        } else if (button == GLUT_RIGHT_BUTTON) {
            // Botão direito fecha a garra
            garra.abertura = fmin(garra.abertura + 5.0, 90.0);
        }
        glutPostRedisplay();
    }
}
// Função de teclado
void teclado(unsigned char key, int x, int y) {
    switch(key) {
        case '1': segmentoAtual = 0; break;
        case '2': segmentoAtual = 1; break;
        case '3': segmentoAtual = 2; break;
        case '4': segmentoAtual = 3; break;
        
        case 'a': case 'A':
            if (segmentoAtual == 0) posicaoBase[0] -= 0.1;
            else if (segmentoAtual == 1) braco.angulo += 5.0;
            else if (segmentoAtual == 2) antebraco.angulo += 5.0;
            else if (segmentoAtual == 3) garra.angulo += 5.0;
            break;
            
        case 'd': case 'D':
            if (segmentoAtual == 0) posicaoBase[0] += 0.1;
            else if (segmentoAtual == 1) braco.angulo -= 5.0;
            else if (segmentoAtual == 2) antebraco.angulo -= 5.0;
            else if (segmentoAtual == 3) garra.angulo -= 5.0;
            break;
            
        case 'w': case 'W':
            if (segmentoAtual == 0) posicaoBase[1] += 0.1;
            else if (segmentoAtual == 3) garra.abertura = fmax(garra.abertura - 5.0, 0.0);
            break;
            
        case 's': case 'S':
            if (segmentoAtual == 0) posicaoBase[1] -= 0.1;
            else if (segmentoAtual == 3) garra.abertura = fmin(garra.abertura + 5.0, 90.0);
            break;
            
        case 27: // ESC
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("Braco Robotico 2D");
    
    inicializar();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    glutDisplayFunc(desenhar);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(teclado);
    glutMouseFunc(mouse);
    
    glutMainLoop();
    return 0;
}
