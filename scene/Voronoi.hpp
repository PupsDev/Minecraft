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
        vector<vector<int>> idImage;
        vector<vector<double>> heightMap;
        vector<vector<glm::vec3>> image;
        vector<VoronoiPoint> points;
        Voronoi()
        {
            startingPoint = 20;
            width=height = 100;
            image.resize(height);
            idImage.resize(height);
            heightMap.resize(height);
            for(int i=0 ; i<height ; i++){
                image[i].resize(width);
                idImage[i].resize(width);
                heightMap[i].resize(width);
            }
            auto start = high_resolution_clock::now();
            generate();
            computeVoronoi();
            blurr();
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
 
            cout << "Time taken by function: "
                << duration.count() << " microseconds" << endl;
            drawPPM();
        }
        void generate()
        {
            int diffColor =0;
            for(int i=0 ; i<startingPoint; i++)
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
            
            /*
            for(int i=0 ; i<startingPoint; i++)
            {
                cout<<points[i].x<<endl;
                cout<<points[i].y<<endl;
                cout<<points[i].color<<endl;
            }
            */
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
                cout<<"Génération voronoi : "<<((float)i)/height * 100 <<" %"<<endl;
                for(int j=0 ; j<width ; j++)
                {
                    float minD = FLT_MAX;
                    float minD2 = FLT_MAX;
                    int minIndice = -1;
                    int minIndice2 = -1;
                    for(int indiceP = 0 ; indiceP<startingPoint ; indiceP++)
                    {
                        VoronoiPoint p = points[indiceP];
                        float newD = distance(j,i,p.x,p.y);
                        //cout<<newD<<endl;
                        if(newD<minD){
                            minD2 = minD;
                            minD = newD;

                            minIndice2 = minIndice;
                            minIndice = indiceP;
                            
                        }
                    }
                    float k = 1.;
                    float k2 = 1.;
                    if( minD > 0)
                    {
                        k = 1. - minD/80.;
                    }

                    if( minD2 > 0)
                    {
                        k2 = 1. - minD2/80.;
                    }

                    k = std::fmin(1.,k);
                    k = std::fmax(0.,k);

                    k2 = std::fmin(1.,k2);
                    k2 = std::fmax(0.,k2);

                    float h1 = 0.2 * (minIndice % 5);
                    float h2 = 0.2 * (minIndice2 % 5);
                    float val = h2*(minD2/255.0f)+h1*(minD/255.0f);
                    val = h1;

                    heightMap[i][j] = std::max(0.0f,std::min(1.0f,val));
                    idImage[i][j] = minIndice;
                    image[i][j]   =  ((float)heightMap[i][j]) * /*points[minIndice].color;*/glm::vec3(255,255,255);
                }
            }
        }
        void blurr(int rad = 2){
            // float conv[] = {
            //     0,2,3,2,0,
            //     2,4,5,4,2,
            //     3,5,7,5,3,
            //     2,4,5,4,2,
            //     0,2,3,2,0
            // };

            for(int x = 0 ; x<width ; x++){
                for(int y = 0 ; y < height ; y ++){
                    double val = 0.0;
                    float nbVal = 0;
                    for(int dx = -rad ; dx <= rad ; dx++){
                        for(int dy = -rad ; dy <= rad ; dy++){
                            if(x+dx>=0 && x+dx < width && y +dy >= 0 && y+dy < height){
                                val+=heightMap[x+dx][y+dy];
                                nbVal++;
                            }
                        }
                    }
                    heightMap[x][y] = val/nbVal;
                    image[x][y]   =  ((float)heightMap[x][y]) * glm::vec3(255,255,255);

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
                        myfile << (int)image[i][j][k]<<" ";
                    myfile<<"\n";
                }
            }
            myfile.close();

        }

};