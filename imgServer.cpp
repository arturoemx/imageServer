/*!
\file imgServer.h
\brief Este es el programa principal del servidor de imágenes. El programa captura imágenes de una cámara y las ofrece a multiples cliente a traves de una conexion vía sockets. Las imagenes se les aplica una transformación proyectiva de tal manera que una región de un plano en la escena se ajuste perfectamente a la imagen capturada. Tambien a cada imagen se le puede aplicar una mascara, esta máscara se puede utilizar para añadir información predeterminada a cada imagen capturada.

Este programa es el componente principal del sistema, mismo que fue diseñado como una herramienta para la teleoperación de robots móviles, y su uso principal es en labores de investigación y docencia.


Uso:

 imgServer camId ipAddress port Maze

El programa puede recibir hasta 4 parámetros, mismos que se detallan a continuacion:

  camId     :  Identificador numérico de la cámara que se va a utilizar para
                 la captura.
  ipAddress :  Direccion IP (IPV4) en donde el servidor va a ofrecer las
                 imagenes.
  port      :  Puerto del servidor.
  Maze      :  Archivo de imagen que contiene la codificación del archivo de
                 máscara que se va a utilizar. El archivo debe ser una imagen
                 a color. La codificación del archivo es como sigue: las
                 regiones blancas son regiones no enmascaradas, y las regiones
                 no-blancas sustituyen a las imagenes capturadas. 
*/

#include <ImageServer.h>
#include <cstring>

/*!
\struct cornerData
\brief Esta estructura contiene información de esquinas que se utiliza durante la captura de posiciones en la pantalla usando el raton.
*/
struct cornerData
{
    Point2f crn[4]; ///< Arreglo de 4 objetos tipo Point2f en donde se almacenaran las esquinas.
    int cont; ///< Contador.

    /*!
    \fn cornerData()
    \brief Constructor del objeto.
    */
    cornerData()
    {
        cont = 0;
    }
};

/*!
\fn void onMouseEvent(int event, int x, int y, int flags, void *data)
\brief Funcion que atiende evantos generados por el ratón. Esta funcion es invocada automáticamnete por el sistema cuando se registra usando la función setMouseCallback de la biblioteca highgui.
*/
void onMouseEvent(int event, int x, int y, int flags, void *data)
{
    cornerData *cD;

    cD = (cornerData *)data;
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        cD->crn[cD->cont].x = x;
        cD->crn[cD->cont].y = y;
        cD->cont++;
    }
}

/*!
\fn void dibujaPuntos (Mat &frame, Point2f *p, int n,  Scalar color=Scalar(0,0,255))
\brief Esta función dibuja una lista de puntos en una imagen.
\param Mat &frame Imagen en donde se van a dibujar los puntos.
\param Point2f *p Apuntador al arreglo de objetos de tipo Point2f que contiene las coordenadas de los puntos a dibujar.
\param int n Cantidad de puntos a dibujar.
\param Scalar color Objeto de tipo scalar que codifica en fomrto BGR el color de los puntos a dibujar.
*/
void dibujaPuntos (Mat &frame, Point2f *p, int n,  Scalar color=Scalar(0,0,255))
{

    for (int i=0;i<n; ++i)
        circle(frame, Point((int)p[i].x, (int)p[i].y), 5, color);
}

/*!
\fn void findMapping(int source, Point2f *vP, Mat &H)
\brief Esta funcion se utiliza para obtener la información necesaria para rectificar las imagenes que el servidor va a capturar. La funcion captura imágenes de la fuente de video "source", permite que usuario capture 4 coordenadas en la imagen utilizando el mouse, y a partir de esas cuatro coordenadas, calcula la transformación proyectiva que describe la transformación de esas 4 coordenadas al plano de la imagen. Se presume que el usuario va a elegir cuatro esquinas que yacen en un plano en la escena, y que la tranformación producida, permitira dedicar la atención de esa región únicamente.
\param int source El identificador de la cámara que se va a utilizar.
\paramn Point2f *vp
*/
void findMapping(int source, Point2f *vP, Mat &H)
{
    Point2f rP[4];
    Mat Frame;
    VideoCapture cap(source);
    cornerData cD;
    int i = 0;

    if (!cap.isOpened())
    {
        cerr << "no se pudo abrir la cámara" << endl;
        return;
    }

    namedWindow("Introduce esquinas");
    setMouseCallback("Introduce esquinas", onMouseEvent, (void*) &cD);
    cD.cont = 0;
    while (cD.cont < 4)
    {
        cap >> Frame;
        if (Frame.empty())
            cerr << "capturaPuntos: Error capturando el Frame" << endl;
        imshow("Introduce esquinas", Frame);
        dibujaPuntos (Frame, cD.crn, cD.cont);   
        if (waitKey(30) >= 0)
            break;
    }
    cvSetMouseCallback("Introduce esquinas", 0, 0);
    if (cD.cont < 4)
      cerr << "La captura de puntos se aborto." << endl;
    for (int i=0;i<4;++i)
        rP[i] = cD.crn[i];
    cap.release();

    cout << "Se capturaron los siguientes puntos:" << endl;
    for (i=0;i<4;++i)
        cout << "(" << rP[i].x << ", " << rP[i].y << ")" << endl;
    H = getPerspectiveTransform(rP, vP);
    destroyWindow("Introduce esquinas");
}

int main (int argc, char **argv)
{
	 int port = 8888, camId = 0;
	 char ipAddress[64] = "127.0.0.1";
	 Point2f vP[4], mP[4];
     Mat Hrv, Hmv, Maze;

	 Maze = 255 * Mat::ones(Size(640,480), CV_8UC3);
	 if (argc < 2)
	 {
	    cerr << "Uso: imgServer camId ipAddress port Maze" << endl;
	    return -1;
     }
     else
        camId = atoi(argv[1]);
	 if (argc > 2)
	 {
			strncpy (ipAddress, argv[2], 63);
			if (argc > 3)
			{
			    port = atoi (argv[3]);
			    if (argc > 4)
			        Maze = imread(argv[4]);
	        }
	}
    
     vP[0].x = 0; vP[0].y = 0;
     vP[1].x = 639; vP[1].y = 0;
     vP[2].x = 639; vP[2].y = 479;
     vP[3].x = 0; vP[3].y = 479;
     findMapping(camId, vP, Hrv);
     cout << "Calculamos la Hrv como:"<< endl << Hrv << endl;

     mP[0].x = 0; mP[0].y = 0;
     mP[1].x = Maze.cols; mP[1].y = 0;
     mP[2].x = Maze.cols; mP[2].y = Maze.rows;
     mP[3].x = 0; mP[3].y = Maze.rows;
     Hmv = getPerspectiveTransform(mP, vP);
     cout << "[" << Maze.cols << ", " << Maze.rows << "]" << endl;
     cout << "Calculamos la Hmv como:"<< endl << Hmv << endl;

	 ImageServer *imS = new ImageServer (camId, port, ipAddress, &Hrv, &Hmv, &Maze);

	 imS->start ();//Esta funcion bloquea la ejecución del programa. ¿debiera ocurrir?

	 return 0;
}
