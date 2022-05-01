
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h>

class LoaderObj 
{
    public:

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;
    std::vector< unsigned short> indices;

    LoaderObj()
{

}
LoaderObj(std::string filename)
{
    
    std::ifstream input_stream(filename);

    if (!input_stream) std::cerr << "Can't open input file!";
    std::cout << "Loading "<<filename<<"...";
    std::vector<std::string> text;
    std::string delimiter = " ";
    std::vector<float> tokensFloat;
    std::string line;



    while (getline(input_stream, line)) {
        text.push_back(line);

    }
           
    
    std::string name;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> mtextures;
     unsigned short indice_v=0,indice_n=0,indice_vt=0;

    std::vector< unsigned short> indices;
    std::vector<std::string> pointToken;
    std::vector<std::string> vertTexNorm;

    std::vector< unsigned short> tempFaceindices;
    std::vector< unsigned short> tempFaceTextureindices;

    std::vector< unsigned short> tempFaceNormalindices;
    std::vector<glm::vec3> normalsByIndice;
    std::vector<glm::vec2> texturesByIndice;

    std::map<std::string, unsigned short> hashedTuplet;

    // like face indice
    unsigned short currentIndice =0;

     unsigned short faceCount = 0;
    

    for(  unsigned short i = 0 ; i < text.size(); i++)
    {
        switch(text[i][0])
        {
            
            case 'o':
                // o is [0] -> name is [1]
                // Name of the Mesh
                name = parseToken(text[i], delimiter)[1];
                break;
            case 'v':
                tokensFloat = parseTokenToFloat(text[i], delimiter);
               

                switch(text[i][1])
                {
                    case 'n': // normals
                        normals.push_back( glm::vec3(tokensFloat[0],tokensFloat[1],tokensFloat[2]));
                    break;
                    case 't': // textures
                        mtextures.push_back( glm::vec2(tokensFloat[0],tokensFloat[1]));
                    break;
                    default: // 'v' -> vertices
                        vertices.push_back( glm::vec3(tokensFloat[0],tokensFloat[1],tokensFloat[2]));
                    break;
                }

                break;
            case 's':
                break;
            case 'f':
                pointToken = parseToken(text[i], " ");
                /*
                Check for v or v/t or v//vn instead of v/t/vn
                */
                for( unsigned short j = 1 ; j < 4 ; j++)
                {
                    string tupletString = pointToken[j];
                    vertTexNorm = parseToken(tupletString, "/");
                    // j : 1 - 4 -> point 1 - 4  as vtn
                    tempFaceindices.push_back(( unsigned short)(std::stoi(vertTexNorm[0])-1));
                    int item = ( unsigned short)std::stoi(vertTexNorm[1])-1;
                    tempFaceTextureindices.push_back(item);
                    tempFaceNormalindices.push_back(( unsigned short)(std::stoi(vertTexNorm[2])-1));

                    // not found
                    if ( hashedTuplet.find(tupletString) == hashedTuplet.end() ) {

                        hashedTuplet.insert( std::pair<string,int>(tupletString,currentIndice));

                        this->vertices.push_back(vertices[tempFaceindices[faceCount +j-1]]);
                        this->textures.push_back(mtextures[tempFaceTextureindices[faceCount +j-1]]);
                        this->normals.push_back(normals[tempFaceNormalindices[faceCount +j-1]]*-1.f);

                        this->indices.push_back(currentIndice);
                        
                    }
                    else // found
                    {
                        this->vertices.push_back(this->vertices[hashedTuplet[tupletString]]);
                        this->textures.push_back(this->textures[hashedTuplet[tupletString]]);
                        this->normals.push_back(this->normals[hashedTuplet[tupletString]]); 
                        this->indices.push_back(hashedTuplet[tupletString]);
                    }
                    currentIndice++;
                    

                }
                // Making triangle
                
                            
                //indices.push_back(tempFaceindices[faceCount]);
                //indices.push_back(tempFaceindices[faceCount+1]);
                //indices.push_back(tempFaceindices[faceCount+2]);
                //normalsByIndice.push_back(normals[tempFaceNormalindices[faceCount]]);
                //texturesByIndice.push_back(mtextures[tempFaceTextureindices[faceCount]]);
                //texturesByIndice.push_back(mtextures[tempFaceTextureindices[faceCount+1]]);
                //texturesByIndice.push_back(mtextures[tempFaceTextureindices[faceCount+2]]);
                faceCount+=3;


                break;
            default:
                break;

        }
    }
    //this->vertices = vertices;
    //this->indices = indices;
    //this->normals = normalsByIndice;
    //this->textures = texturesByIndice;
    //for(auto& textIndice : tempFaceTextureindices)
    //{
    //    cout<<std::count(tempFaceTextureindices.begin(), tempFaceTextureindices.end(), textIndice)<<endl;
    //}

    /*for(auto& texture : textures)
    {
       cout<<texture[0]<<" "<<texture[1]<<endl;
    }
    */
    //std::cout<<"Done !\n";
    //std::cout<<"size : "<<vertices.size()<<"\n";
    //std::cout<<"size : "<<normals.size()<<"\n";
    //std::cout<<"size : "<<textures.size()<<"\n";
    //std::cout<<"size : "<<this->indices.size()<<"\n";
    input_stream.close();


}

std::vector<std::string> parseToken(std::string s, std::string delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
     size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token); 
         
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s); 
    return tokens;
}
std::vector<float> parseTokenToFloat(std::string s, std::string delimiter)
{
    std::vector<float> tokens;
    std::string token;
     size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
       // cout << "test : "<< token[0]<<endl;
        if(token[0]!='v')
        {
            
            tokens.push_back(std::stof(token)); 
        }
         
        s.erase(0, pos + delimiter.length());
    }
      /*cout << "test : "<< s<<endl;
          for(auto & token : tokens)
    {
        cout<<token<<endl;
    } */
    tokens.push_back(std::stof(s));


    return tokens;
}

};

