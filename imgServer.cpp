#include <ImageServer.h>
#include <cstring>

struct cornerData
{
    Point2f crn[4];
    int cont;
    cornerData()
    {
        cont = 0;
    }
};

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

void dibujaPuntos (Mat &frame, Point2f *p, int n,  Scalar color=Scalar(0,0,255))
{

    for (int i=0;i<n; ++i)
        circle(frame, Point((int)p[i].x, (int)p[i].y), 5, color);
}

void capturaPuntos (const char *Name, Mat & Frame, Point2f *P)
{
    cornerData cD;

    imshow(Name, Frame);
    setMouseCallback(Name, onMouseEvent, (void*) &cD);
    while (cD.cont < 4)
    {
        dibujaPuntos (Frame, cD.crn, cD.cont);   
        if (waitKey(30) >= 0)
            break;
    }
    cvSetMouseCallback(Name, 0, 0);
    if (cD.cont < 4)
      cerr << "La captura de puntos se aborto." << endl;
    for (int i=0;i<4;++i)
        P[i] = cD.crn[i];
}

void findMapping(int source, Point2f *vP, Mat &H)
{
    Point2f rP[4];
    Mat Frame;
    VideoCapture cap(source);
    int i = 0;

    if (!cap.isOpened())
    {
        cerr << "no se pudo abrir la cámara" << endl;
        return;
    }

    while (true)
    {
        cap >> Frame;
        if (Frame.empty())
            cerr << "Error capturando el Frame No. " << i << endl;
        else
            ++i;
        if (waitKey(33)  != 0)
            break;
    }

    namedWindow("Introduce esquinas");
    capturaPuntos("Introduce esquinas", Frame, rP);
    cout << "Se capturaron los siguientes puntos:" << endl;
    for (i=0;i<4;++i)
        cout << "(" << rP[i].x << ", " << rP[i].y << ")" << endl;
    H = getPerspectiveTransform(rP, vP);
    destroyWindow("Introduce esquinas");
    
    
}

int main (int argc, char **argv)
{
	 int port = 8888;
	 char ipAddress[64] = "127.0.0.1";
	 Point2f vP[4], mP[4];
     Mat Hrv, Hmv, Maze;

	 if (argc < 2)
	    return -1;
     else
        Maze = imread(argv[1]);
	 if (argc > 2)
	 {
			strncpy (ipAddress, argv[2], 63);
			if (argc > 3)
			    port = atoi (argv[2]);
	 }
    
     vP[0].x = 0; vP[0].y = 0;
     vP[1].x = 639; vP[1].y = 0;
     vP[2].x = 639; vP[2].y = 479;
     vP[3].x = 0; vP[3].y = 479;
     findMapping(0, vP, Hrv);
     cout << "Calculamos la Hrv como:"<< endl << Hrv << endl;

     mP[0].x = 0; mP[0].y = 0;
     mP[1].x = Maze.cols; mP[1].y = 0;
     mP[2].x = Maze.cols; mP[2].y = Maze.rows;
     mP[3].x = 0; mP[3].y = Maze.rows;
     Hmv = getPerspectiveTransform(mP, vP);
     cout << "[" << Maze.cols << ", " << Maze.rows << "]" << endl;
     cout << "Calculamos la Hmv como:"<< endl << Hmv << endl;

	 ImageServer *imS = new ImageServer (port, ipAddress, &Hrv, &Hmv, &Maze);

	 imS->start ();

	 sleep (3);										// wait for server to init

	 do
	 {

	 }
	 while (true);

	 return 0;
}
