#include <time.h>
#include <limits>
#include <algorithm>
#include <chrono>

struct VoronoiPoint{
    int id;
    glm::vec3 color;
    int x, y;
};
using namespace std;
using namespace std::chrono;
using std::vector;
class Voronoi
{
    public:
        int startingPoint;
        int width,height;
        vector<vector<glm::vec3>> image;
        vector<VoronoiPoint> points;
        Voronoi()
        {
            startingPoint = 20;
            width=height = 320;
            image.resize(height);
            for(int i=0 ; i<height ; i++)
                image[i].resize(width);
            auto start = high_resolution_clock::now();
            generate();
            computeVoronoi();
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
 
            cout << "Time taken by function: "
                << duration.count() << " microseconds" << endl;
            drawPPM();
        }
        void generate()
        {
            int diffColor =0;
            for(int i=0 ; i<20; i++)
            {
                int x = rand()%width;
                int y = rand()%height;
                int color = diffColor;
                diffColor +=10000;
                int r = (color & 0x000000FF) >>  0;
                int g = (color & 0x0000FF00) >>  8;
                int b = (color & 0x00FF0000) >> 16;
                VoronoiPoint p = {color,glm::vec3(r,g,b),x,y};
                points.push_back(p);

            }

            for(int i=0 ; i<20; i++)
            {
                cout<<points[i].x<<endl;
                cout<<points[i].y<<endl;
                cout<<points[i].color<<endl;
            }
        }
        float distance(float x1, float y1, float x2, float y2)
        {
            float x = x1 - x2; //calculating number to square in next step
            float y = y1 - y2;
            float dist;

            dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance                
            return sqrt(dist);
        }
        void computeVoronoi()
        {
            for(int i=0 ; i<height ; i++)
            {
                for(int j=0 ; j<width ; j++)
                {
                    float minD = FLT_MAX;
                    int minIndice = -1;
                    for(int indiceP = 0 ; indiceP<startingPoint ; indiceP++)
                    {
                        VoronoiPoint p = points[indiceP];
                        float newD = distance(j,i,p.x,p.y);
                        //cout<<newD<<endl;
                        if(newD<minD){
                            minD = newD;
                            minIndice = indiceP;
                        }
                    }
                    float k = 1.;
                    if( minD > 0)
                    {
                        k = 1. - minD/80.;
                    }

                    k = std::fmin(1.,k);
                    k = std::fmax(0.,k);
                    image[i][j] =  k * /*points[minIndice].color;*/glm::vec3(255,255,255);
                }
            }
        }
        void drawPPM()
        {

            ofstream myfile;


            std::string output = "../Voronoi/test.ppm";
            myfile.open (&output[0]);

            myfile << "P3\n";
            myfile << width<<" "<<height<<"\n255\n";
            

        
            for(int i=0 ; i<height ; i++)
            {
                for(int j=0 ; j<width ; j++)
                {
                    for(int k=0 ; k<3 ; k++)
                        myfile << image[i][j][k]<<" ";
                    myfile<<"\n";
                }
            }
            myfile.close();

        }

};