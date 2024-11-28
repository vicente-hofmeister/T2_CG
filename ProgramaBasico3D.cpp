// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa b�sico para criar aplicacoes 3D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <cmath>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Temporizador.h"
#include "ListaDeCoresRGB.h"
#include "Ponto.h"
#include "Instancia.h"
#include "Tools.h"
#include "Poliedro.h"
#include "Modelo3D.h"

Temporizador T;
double AccumDeltaT=0;

GLfloat AspectRatio, angulo=0;

// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'
int ModoDeProjecao = 1;

// Controle do modo de projecao
// 0: Wireframe; 1: Faces preenchidas
// A funcao "Init" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'e'
int ModoDeExibicao = 1;
double nFrames=0;
double TempoTotal=0;
Ponto CantoEsquerdo = Ponto(-12.5,0,-25);
Ponto OBS;
Ponto ALVO;
Ponto VetorAlvo;
GLfloat CameraMatrix[4][4];
GLfloat InvCameraMatrix[4][4];
Ponto PosicaoJogador(0,-0.5,4);
float RotacaoJogador = 0.0f;
float movimento = 0.1f;
float rotacao = 1.75f;
float anguloCanhao = 0.0f;
Ponto CentroCanhao = Ponto();
Ponto PontaCanhao = Ponto();
Ponto DirecaoCanhao = Ponto();
bool PewPewNoAr = false;
Ponto PosicaoPewPew = Ponto();
Ponto DirecaoPewPew = Ponto();
float ForcaPewPew = 0.75f;
float Gravidade = 0.05f;
float DistanciaCamera = 4.0f;
float AlturaCamera = 2.5f;
float CorJogador[3] = {0.75f, 0.75f, 0.0f};
float BlocoTamMin = 0.1f;
Poliedro BoundingBoxJogador;
Poliedro ParedeInicial = Poliedro(Ponto(-12.5f,-1.0f,-8.0f),Ponto(12.5f,5.0f,-4.0f));
std::vector<Poliedro> ListaBlocos;
bool teclaW = false, teclaA = false, teclaS = false, teclaD = false;
Modelo3D vaquinha = Modelo3D();
Modelo3D dog = Modelo3D();
bool paredao[25][15];
// **********************************************************************
//  void init(void)
//        Inicializa os parametros globais de OpenGL
// **********************************************************************
void init(void)
{
    glClearColor(0.4f, 1.0f, 0.0f, 1.0f); // Fundo de tela preto

    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE );
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);

    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 15; j++) {
            paredao[i][j] = true;
        }
    }

    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    if (ModoDeExibicao) // Faces Preenchidas??
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    ALVO = Ponto(0, 0, 0);
    OBS = Ponto(0,3,10);
    VetorAlvo = ALVO - OBS;

    Poliedro poli1 = Poliedro(Ponto(12.5f,-1.0f,-8.0f ),Ponto(13.5f,5.0f,-4.0f));
    Poliedro poli2 = Poliedro(Ponto(-13.5f,-1.0f,-8.0f),Ponto(-12.5f,5.0f,-4.0f));

    vaquinha.LeObjetoSimples("Vaca.tri");
    dog.LeObjetoSimples("dog.tri");


    ListaBlocos.push_back(ParedeInicial);
    ListaBlocos.push_back(poli1);
    ListaBlocos.push_back(poli2);

}
// **********************************************************************
//  void DesenhaCubo()
// **********************************************************************
void DesenhaCubo(float tamAresta)
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    // Back Face
    glNormal3f(0,0,-1);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    // Top Face
    glNormal3f(0,1,0);
    
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    // Bottom Face
    glNormal3f(0,-1,0);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    // Right face
    glNormal3f(1,0,0);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    // Left Face
    glNormal3f(-1,0,0);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    glEnd();

}
// **********************************************************************
//
// **********************************************************************
void DesenhaParalelepipedo()
{
    glPushMatrix();
        glTranslatef(0,0,-1);
        glScalef(1,1,2);
        glutSolidCube(2);
        //DesenhaCubo(1);
    glPopMatrix();
}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    defineCor(corDentro);// desenha QUAD preenchido
    //glColor3f(1,1,1);
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

    defineCor(corBorda);
    //glColor3f(0,1,0);

    glBegin ( GL_LINE_STRIP );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();


}
// **********************************************************************
//
// **********************************************************************
void DesenhaPiso()
{
    srand(100); // usa uma semente fixa para gerar sempre as mesma cores no piso
    glPushMatrix();
    glTranslated(CantoEsquerdo.x, CantoEsquerdo.y, CantoEsquerdo.z);
    for(int x = -12.5; x < 12.5;x++)
    {
        glPushMatrix();
        for(int z = -25; z < 25;z++)
        {
            DesenhaLadrilho(MediumGoldenrod, rand()%40);
            glTranslated(0, 0, 1);
        }
        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
}
// **********************************************************************
//
// **********************************************************************
void DesenhaParedao()
{
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 15; j++) {
            if (paredao[i][j]) {
                glPushMatrix();
                    glTranslatef(i - 12.5, j, 0);
                    DesenhaCubo(1);
                glPopMatrix();
            }
        }
    }
    
}
// **********************************************************************
//
// **********************************************************************
void DesenhaChao()
{
    glPushMatrix();
        DesenhaPiso();
    glPopMatrix();
}
// **********************************************************************
//
// **********************************************************************
void AtualizaBoundingBoxJogador()
{
    float largura = 2.0f * 0.75f;
    float altura = 2.0f * 0.5f;
    float profundidade = 2.0f * 1.0f;

    BoundingBoxJogador.setMin(Ponto(
        PosicaoJogador.x - largura / 2,
        PosicaoJogador.y - altura / 2,
        PosicaoJogador.z - profundidade / 2
    ));
    BoundingBoxJogador.setMax(Ponto (
        PosicaoJogador.x + largura / 2,
        PosicaoJogador.y + altura / 2,
        PosicaoJogador.z + profundidade / 2
    ));

}
// **********************************************************************
//
// **********************************************************************
void CalculaNormalECentroCanhao() {
    CentroCanhao.x = PosicaoJogador.x;
    CentroCanhao.y = PosicaoJogador.y + 0.95f;
    CentroCanhao.z = PosicaoJogador.z;

    DirecaoCanhao = Ponto(0,0,-1);
    DirecaoCanhao.rotacionaX(anguloCanhao);
    DirecaoCanhao.rotacionaY(RotacaoJogador);

    PontaCanhao = CentroCanhao + DirecaoCanhao * 1.5;

}
// **********************************************************************
//
// **********************************************************************
void DesenhaJogador()
{
    glPushMatrix(); // Jogador
        glTranslatef ( PosicaoJogador.x, PosicaoJogador.y, PosicaoJogador.z );
        glRotatef(RotacaoJogador, 0.0f, 1.0f, 0.0f);
        glColor3f(CorJogador[0], CorJogador[1], CorJogador[2]);
        glScalef(0.75f, 0.5f, 1.0f);
        glutSolidCube(2);

        glPushMatrix(); // Cabine
            glTranslatef(0.0f, 1.5f, 0.0f);
            glScalef(1.0f, 1.5f, 0.75f);
            glColor3f(CorJogador[0], CorJogador[1], CorJogador[2]);
            glutSolidSphere(0.9f, 20, 20);

            GLUquadric *quad = gluNewQuadric();
            glPushMatrix(); // Canhao
                glTranslatef(0.0f, 0.25f, 0.0f);
                glRotatef(anguloCanhao, 1.0f, 0.0f, 0.0f);
                glTranslatef(0.0f, 0.0f, -2.5f);

                glColor3f(CorJogador[0], CorJogador[1], CorJogador[2]);
                gluCylinder(quad, 0.3f, 0.15f, 2.5f, 10, 10);

                glPushMatrix();
                    glColor3b(0.0f, 0.0f, 0.0f);
                    glRotatef(180, 1.0f, 0.0f, 0.0f); // Inverte para alinhar com a base
                    CalculaNormalECentroCanhao();
                    gluDisk(quad, 0.0, 0.3, 10, 1); // Desenha o disco da base
                glPopMatrix();
            glPopMatrix();
            gluDeleteQuadric(quad);

        glPopMatrix();

        glPushMatrix(); // Roda dir
            glTranslatef(1.25f, -0.5f, 0.0f);
            glScalef(0.5f, 1.0f, 1.5f);
            glColor3f(0.25f, 0.25f, 0.25f);
            glutSolidCube(1);
        glPopMatrix();

        glPushMatrix(); // Roda esq
            glTranslatef(-1.25f, -0.5f, 0.0f);
            glScalef(0.5f, 1.0f, 1.5f);
            glColor3f(0.25f, 0.25f, 0.25f);
            glutSolidCube(1);
        glPopMatrix();

        AtualizaBoundingBoxJogador();

        Ponto P;
        P = InstanciaPonto(Ponto(0,0,0), InvCameraMatrix);
        //P = InstanciaPonto(Ponto(0,0,0), OBS, ALVO);
        // PosicaoJogador.imprime("Posicao do Objeto:", "\n");
        // P.imprime("Ponto Instanciado: ", "\n");
    glPopMatrix();
}
// **********************************************************************
//
// **********************************************************************
void DesenhaPoliedro(Ponto p1, Ponto p2)
{
    float minX = std::min(p1.x, p2.x);
    float minY = std::min(p1.y, p2.y);
    float minZ = std::min(p1.z, p2.z);
    float maxX = std::max(p1.x, p2.x);
    float maxY = std::max(p1.y, p2.y);
    float maxZ = std::max(p1.z, p2.z);
    float lengthX = maxX - minX;
    float lengthY = maxY - minY;
    float lengthZ = maxZ - minZ;
    float centralX = minX + (lengthX / 2);
    float centralY = minY + (lengthY / 2);
    float centralZ = minZ + (lengthZ / 2);
    
    glPushMatrix();
        glTranslatef(centralX, centralY, centralZ);
        glScalef(lengthX, lengthY, lengthZ);
        glutSolidCube(1);
        glEnd();
    glPopMatrix();

}
// **********************************************************************
// 
// **********************************************************************
void DesenhaParede()
{
    // Ponto p1 = Ponto(-4.0f,-0.5f,1.0f);
    // Ponto p2 = Ponto(4.0f,3.5f,2.0f);

    // DesenhaPoliedro(p1, p2);
    glColor3f(0.5, 0.5, 0.5); // Azul claro

    for (int i = 0; i < ListaBlocos.size(); i++) {
        Ponto p1 = ListaBlocos[i].getMin();
        Ponto p2 = ListaBlocos[i].getMax();

        DesenhaPoliedro(p1, p2);
    }
}
// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  //GLfloat LuzAmbiente[]   = {0.0, 0.0, 0.0 } ;
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4} ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  //GLfloat LuzDifusa[]   = {0, 0, 0};
  GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posi��o da Luz
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  //GLfloat LuzEspecular[] = {0.0f, 0.0f, 0.0 };
 
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de ilumina��o
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz n�mero Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��oo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,128);

}
// **********************************************************************
// 
// **********************************************************************
bool ChecaColisao(const Poliedro& poliedro1, const Poliedro& poliedro2) {
    Ponto min1 = poliedro1.getMin();
    Ponto max1 = poliedro1.getMax();

    Ponto min2 = poliedro2.getMin();
    Ponto max2 = poliedro2.getMax();

    // Verifica sobreposição em cada eixo
    bool colisaoX = !(max1.x < min2.x || max2.x < min1.x);
    bool colisaoY = !(max1.y < min2.y || max2.y < min1.y);
    bool colisaoZ = !(max1.z < min2.z || max2.z < min1.z);

    cout << "Jogador: Min(" << min1.x << ", " << min1.y << ", " << min1.z << ") "
     << "Max(" << max1.x << ", " << max1.y << ", " << max1.z << ")" << endl;
    cout << "Bloco: Min(" << min2.x << ", " << min2.y << ", " << min2.z << ") "
     << "Max(" << max2.x << ", " << max2.y << ", " << max2.z << ")" << endl;

    cout << "colisaoX:" << colisaoX << ", " << "colisaoY:" << colisaoY << ", " << "colisaoZ:" << colisaoZ << ")" << endl;

    // Os cubos colidem se houver sobreposição em todos os eixos
    return colisaoX && colisaoY && colisaoZ;
}
// **********************************************************************
//
// **********************************************************************
void VerificaColosoesComBlocos()
{
    int i = 0;
    for (const Poliedro& bloco : ListaBlocos) {
        bool achouColisao = ChecaColisao(BoundingBoxJogador, bloco);
        cout << "achouColisao: " << achouColisao << endl;

        if (achouColisao) {
            CorJogador[0] = 1.0f;
            CorJogador[1] = 0.0f;
            CorJogador[2] = 0.0f;
            cout << "COLISAO" << endl;
        }
    }
}
// **********************************************************************
//
// **********************************************************************
void MygluPerspective(float fieldOfView, float aspect, float zNear, float zFar )
{
    //https://stackoverflow.com/questions/2417697/gluperspective-was-removed-in-opengl-3-1-any-replacements/2417756#2417756
    // The following code is a fancy bit of math that is equivilant to calling:
    // gluPerspective( fieldOfView/2.0f, width/height , 0.1f, 255.0f )
    // We do it this way simply to avoid requiring glu.h
    //GLfloat zNear = 0.1f;
    //GLfloat zFar = 255.0f;
    //GLfloat aspect = float(width)/float(height);
    GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}
// **********************************************************************
//  void PosicUser()
// **********************************************************************
void PosicUser()
{

    // Define os par�metros da proje��o Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o volume de visualiza��o sempre a partir da posicao do
    // observador
    if (ModoDeProjecao == 0)
        glOrtho(-12, 12, -12, 12, 1, 18); // Projecao paralela Orthografica
    else MygluPerspective(90,AspectRatio,0.1,50); // Projecao perspectiva

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (ModoDeProjecao <= 1) {
        OBS = Ponto(0,0,10);
        ALVO = Ponto(0, 0, 0);

        gluLookAt(OBS.x, OBS.y, OBS.z,   // Posi��o do Observador
                  ALVO.x, ALVO.y, ALVO.z,     // Posi��o do Alvo
                  0.0,1.0,0.0);
    }
    else if (ModoDeProjecao == 2) {


        OBS.x = PosicaoJogador.x + DistanciaCamera * sin(RotacaoJogador * M_PI / 180.0);
        OBS.y = PosicaoJogador.y + AlturaCamera;
        OBS.z = PosicaoJogador.z + DistanciaCamera * cos(RotacaoJogador * M_PI / 180.0);
        ALVO = PosicaoJogador;

        gluLookAt(OBS.x, OBS.y, OBS.z,   // Posi��o do Observador
                  ALVO.x, ALVO.y, ALVO.z,     // Posi��o do Alvo
                  0.0,1.0,0.0);
    }
    
    
    glGetFloatv(GL_MODELVIEW_MATRIX,&CameraMatrix[0][0]);
    InverteMatriz(CameraMatrix, InvCameraMatrix);
    SalvaMatrizDaCamera(InvCameraMatrix);
    
    //ImprimeMatriz(CameraMatrix);
    //cout << "Inversa:\n";
    //ImprimeMatriz(InvCameraMatrix);

}
// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
// **********************************************************************
void reshape( int w, int h )
{

	// Evita divis�o por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a rela��o entre largura e altura para evitar distor��o na imagem.
    // Veja fun��o "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();

}
// **********************************************************************
//
// **********************************************************************
void PewPew() {
    PewPewNoAr = true;
    CalculaNormalECentroCanhao();
    PosicaoPewPew = PontaCanhao;
    DirecaoPewPew = DirecaoCanhao * ForcaPewPew;
}
// **********************************************************************
//
// **********************************************************************
void MovePewPew() {
    if (PewPewNoAr) {
        DirecaoPewPew.y -= Gravidade;
        PosicaoPewPew = PosicaoPewPew + DirecaoPewPew;
    }
}
// **********************************************************************
//
// **********************************************************************
void DesenhaPewPew() {
    MovePewPew();
    glPushMatrix();
    glTranslatef(PosicaoPewPew.x, PosicaoPewPew.y, PosicaoPewPew.z);
    glColor3f(0.7f, 0.7f, 0.7f);
    glutSolidSphere(0.15f, 10, 10);
    glPopMatrix();
}
// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();

	glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glTranslatef(0, -1, 0);
    DesenhaChao();
    DesenhaParedao();
    glPopMatrix();
    
    // glPushMatrix();
    // glTranslatef(0.0f, 2.5f, 0.0f);
    // glScalef(0.1f, 0.10f, 0.10f);
    // glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    // glColor3f(1.0, 0.0, 0.0);
    // vaquinha.DesenharSimples();
    // glPopMatrix();

    // glPushMatrix();
    // glTranslatef(3.0f, 0.5f, 0.0f);
    // glScalef(0.2f, 0.2f, 0.2f);
    // dog.DesenharSimples();
    // glPopMatrix();
    DesenhaJogador();
    DesenhaPewPew();

	glPushMatrix(); 
		glTranslatef ( -4.0f, 0.0f, 2.0f );
		glRotatef(angulo,0,1,0);
		glColor3f(0.6156862745, 0.8980392157, 0.9803921569); // Azul claro
        glutSolidCube(2);
	glPopMatrix();

    //glColor3f(0.8,0.8,0);
    //glutSolidTeapot(2);
    //DesenhaParedao();

	glutSwapBuffers();
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
            PosicaoJogador.x -= movimento * sin(RotacaoJogador * M_PI / 180.0f);
            PosicaoJogador.z -= movimento * cos(RotacaoJogador * M_PI / 180.0f);
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
            PosicaoJogador.x += movimento * sin(RotacaoJogador * M_PI / 180.0f);
            PosicaoJogador.z += movimento * cos(RotacaoJogador * M_PI / 180.0f);
			break;
        case GLUT_KEY_RIGHT:
            RotacaoJogador -= rotacao;
            break;
        case GLUT_KEY_LEFT:
            RotacaoJogador += rotacao;
            break;

		default:
			break;
	}
}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
    
    switch ( key )
    {
        case 27:        // Termina o programa qdo
            exit ( 0 );   // a tecla ESC for pressionada
            break;
        case 'w':
            teclaW = true;
            break;
        case 's':
            teclaS = true;
            break;
        case 'a':
            teclaA = true;
            break;
        case 'd':
            teclaD = true;
            break;
        case 'r':
            if (anguloCanhao < 50.0f)
                anguloCanhao += 2.5f; 
            break;
        case 'f':
            if (anguloCanhao > -10.0f)
                anguloCanhao -= 2.5f;
            break;
        case 'e':
            if (ForcaPewPew == 0.75f)
                ForcaPewPew = 1.0f;
            else if (ForcaPewPew == 1.0f)
                ForcaPewPew = 1.25f;
            else if (ForcaPewPew == 1.25f)
                ForcaPewPew = 1.5f;
            else if (ForcaPewPew == 1.5f)
                ForcaPewPew = 0.5f;
            else 
                ForcaPewPew = 0.75f;
            break;
        case ' ':
            PewPew();
            break;
        case 'p':
            if (ModoDeProjecao == 0)
                ModoDeProjecao = 1;
            else if (ModoDeProjecao == 1)
                ModoDeProjecao = 2;
            else 
                ModoDeProjecao = 0;
                
            break;
        case 'o':
            ModoDeExibicao = !ModoDeExibicao;
            init();
            break;
        default:
                cout << key;
        break;
    }
    glutPostRedisplay();
}
// **********************************************************************
//  void keyboardUp ( unsigned char key, int x, int y )
// **********************************************************************
void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
            teclaW = false;
            break;
        case 's':
            teclaS = false;
            break;
        case 'a':
            teclaA = false;
            break;
        case 'd':
            teclaD = false;
            break;
    }
}
// **********************************************************************
//  void atualizaMovimento ( )
// **********************************************************************
void atualizaMovimento()
{
    if (teclaW) {
        PosicaoJogador.x -= movimento * sin(RotacaoJogador * M_PI / 180.0f);
        PosicaoJogador.z -= movimento * cos(RotacaoJogador * M_PI / 180.0f);
    }
    if (teclaS) {
        PosicaoJogador.x += movimento * sin(RotacaoJogador * M_PI / 180.0f);
        PosicaoJogador.z += movimento * cos(RotacaoJogador * M_PI / 180.0f);
    }
    if (teclaA) {
        RotacaoJogador += rotacao;
    }
    if (teclaD) {
        RotacaoJogador -= rotacao;
    }
    AtualizaBoundingBoxJogador();
    VerificaColosoesComBlocos();
    glutPostRedisplay();
}
// **********************************************************************
//
// **********************************************************************
void animate()
{
    atualizaMovimento();

    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+= 1;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}
// **********************************************************************
//  void main ( int argc, char** argv )
// **********************************************************************
int main ( int argc, char** argv )
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 700);
	glutCreateWindow( "Computacao Grafica - Exemplo Basico 3D" );

	init ();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(arrow_keys);
	glutIdleFunc(animate);

	glutMainLoop ( );
	return 0;
}
