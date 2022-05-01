#include <map>
#include <time.h> 
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using std::vector;

class ForestGenerator
{
    public :
        int startTreeCount;
        float travelRadius;
        int amountSeeding;
        int lifeTime;
        int decayRate;
        float coverPercentage;

        vector<vector<int>> shape;
        // PPM image
        vector<vector<vector<int>>> image;
        int width = 320;
        int height = 320;
        // MAP WITH TREE POS
        vector<vector<int>> treeMap;
        vector<std::pair<int,int>> treeList;

        vector<vector<int>> seedMap;
        int countTree=0;
    ForestGenerator()
    {
        startTreeCount=25;
        travelRadius=10;
        amountSeeding=5;
        lifeTime = 2;
        decayRate=1;
        coverPercentage=0.0025;

        shape = { {0,1,0},{1,1,1},{0,1,0}};
        image.resize(height);
        for(int i=0 ; i<height ; i++)
            image[i].resize(width);
        

        for(int i=0 ; i<height ; i++)
        {
            for(int j=0 ; j<width ; j++)
            {
                
                for(int k=0 ; k<3 ; k++){
                    
                    image[i][j].push_back(0);
                }
               
            }
        }
        int s = 0;
        float count = 0.;
        init();
        cout<<"Initialize.."<<endl;
        while(count<coverPercentage)
        {
            step();
            cout<<"step: "<<s++<<endl;
            count = countTrees();
            cout<<"coverage: "<<100.*count<<"%"<<endl;
        }
            
        //drawSeed();
        cout<<"Generate.."<<endl;
        coverPercentage = (int) (100.*count);
        generateImage();
        drawPPM();
    }
    void init()
    {
        treeMap.resize(height);
        seedMap.resize(height);

        for(int i=0 ; i<height ; i++)
        {
            treeMap[i].resize(width);
            seedMap[i].resize(width);
        }


        for(int i = 0 ; i < startTreeCount;i++)
        {
            int posx = rand()%(width-1)+1;
            int posy = rand()%(height-1)+1;
            
            addTree(posy,posx);

        }
        seedTrees();

    }
    void step()
    {
        // decay seed
        for(int i = 0; i <height ; i++)
        {
            for(int j = 0; j < width ; j++)
            {
                seedMap[i][j] = std::max(0,seedMap[i][j]-decayRate);
            }
        }
        // generate tree
        for(int i = 1; i <height-1 ; i++)
        {
            for(int j = 1; j < width-1 ; j++)
            {
                int seedValue = seedMap[i][j] ;
                if (seedValue> 0)
                {
                    if( rand()%20 < seedValue )
                    {
                        seedMap[i][j]=0;
                        addTree(i,j);

                    }
                }
            }
        }
        // seed trees
        
        seedTrees();
    }
    void seedTrees()
    {
        for(auto& tree : treeList)
        {
            
            seeding(tree.first, tree.second);
    
        }

    }
    void seeding(int i , int j)
    {
        int seeded = 0;

       while (seeded < amountSeeding)
       {
           int nj = rand()%(int)(2*travelRadius) - travelRadius + j;
           int ni = rand()%(int)(2*travelRadius) - travelRadius + i;
           if( checkBounds(ni,nj) && !treeMap[ni][nj] && distance(i,j,ni,nj) < travelRadius )
           {
               seedMap[ni][nj]=lifeTime;
               seeded+=1;
           }

       }

    }
    float distance(float x1, float y1, float x2, float y2)
    {
        float x = x1 - x2; //calculating number to square in next step
        float y = y1 - y2;
        float dist;

        dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
        dist = sqrt(dist);                  

        return dist;
    }
    bool checkBounds(int i , int j)
    {
        return (i>=0 && i<height) && (j>=0 && j<width);
    }
    void drawSeed()
    {
        for(int i = 0; i <height ; i++)
        {
            for(int j = 0; j < width ; j++)
            {
                if(seedMap[i][j]>0){
                    image[i][j][0] = 255;
                    image[i][j][1] = 255;
                }
                
            }
        }
    }
    void addTree(int i,int j)
    {
        treeMap[i][j] = 1;
        treeList.push_back(std::make_pair(i,j));

    }
    void generateImage()
    {
        for(int i=1 ; i<height-1 ; i++)
        {
            for(int j=1 ; j<width-1 ; j++)
            {
                if(treeMap[i][j])
                {
                    for(int s=-1 ; s<2 ; s++)
                    {
                        for(int t=-1 ; t<2 ; t++)
                        {   
                            //image[i+s][j+t][2]=0;
                            //image[i+s][j+t][0]=0;
                            image[i+s][j+t][1]+=shape[s+1][t+1]*255;
                            
                        }

                    }
                }
            }

        }

    }
    float countTrees()
    {
        int c =0;

        for(int i = 0; i <height ; i++)
        {
            for(int j = 0; j < width ; j++)
            {
                if(treeMap[i][j]){
                    c++;
                    countTree++;
                }
                
            }
        }
        return (float)c/ ((float)width*(float)height);        
    }
    void drawPPM()
    {

        ofstream myfile;


        std::string output = "Forest/Forest"+std::to_string(startTreeCount)+"_"
                                    +std::to_string((int)travelRadius)+"_"
                                    +std::to_string(amountSeeding)+"_"
                                    +std::to_string(lifeTime)+"_"
                                    +std::to_string(decayRate)+"_"
                                    +std::to_string(coverPercentage)+".ppm";
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