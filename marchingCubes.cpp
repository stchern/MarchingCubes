#include "marchingCubes.h"

MarchingCubes::MarchingCubes(int resolution){

    dataSetSize = resolution;
    stepSize = 1.0/dataSetSize;
    isNeedObjFile = false;
}

void MarchingCubes::createObjFile(bool value){
    isNeedObjFile = value;
}

void MarchingCubes::createObjFile(const QString& filename){

        QFile objFile(filename);
        objFile.open(QIODevice::WriteOnly);
        QTextStream fileStream(&objFile);
        int dotIndex = filename.lastIndexOf('.');
        fileStream<<"#Marching Cubes Algorithm\n#object "<<filename.left(dotIndex)<<"\n\n";

        if (!vertices.isEmpty()){
            int q = 0;
            fileStream<<"v ";
            for ( const auto &el : vertices){
                if(q == 3){
                    q = 0;
                    fileStream << "\nv ";
                }
                fileStream<< el << ' ';
                ++q;
            }
        }
        else 
            fileStream<< "Error:: vertices are undefined\n";

        fileStream<< "\n# "<< vertices.size() << " vertices\n\n";

        if (!normals.isEmpty()){
            int q = 0;
            fileStream<<"vn ";
            for ( const auto &el : vertices){
                if(q == 3){
                    q = 0;
                    fileStream << "\nvn ";
                }
                fileStream<< el << ' ';
                ++q;
            }
        }
        else 
            fileStream<< "Error:: vertices are undefined\n";
        
        fileStream<< "\n# "<< normals.size() << " vertex normals\n\n";
            
        fileStream<<"end of the file\n";

}
//поверхность сферы
float MarchingCubes::sample1(const float &x, const float &y, const float &z){

    float dx = 0.5-x, dy = 0.5-y, dz = 0.5-z;
    return pow(dx * dx + dy * dy + dz * dz ,2);
}

//поверхность эллипсоида
float MarchingCubes::sample2(const float &x, const float &y, const float &z){

    float a = 2.0, b = 3.0, c = 4.0, scale = 7.0;
    float dx = scale * (0.5-x),  dy = scale * (0.5-y),  dz = scale * (0.5-z);
    
    return pow( (dx * dx) / (a * a) + (dy * dy) / (b * b) + (dz * dz) / (c * c),2);
}

//поверхность эллиптического цилиндра
float MarchingCubes::sample3(const float &x, const float &y, const float &z){
    
    float a = 2.0, b = 2.0, scale = 8.0;
    float dx = scale * (0.5-x),  dy = scale * (0.5-y);
    
    return (dx * dx) / (a * a) + (dy * dy) / (b * b) - 1;
}

//поверхность конуса
float MarchingCubes::sample4(const float &x, const float &y, const float &z){
    
    float a = 2.0, b = 3.0, c = 4.0, scale = 7.0;
    float dx = scale * (0.5-x),  dy = scale * (0.5-y),  dz = scale * (0.5-z);
    
    return (dx * dx) / (a * a) + (dy * dy) / (b * b) - (dz * dz) / (c * c);
}

//поверхность тора
float MarchingCubes::sample5(const float &x, const float &y, const float &z){

    float a = 2.0, scale = 8.0;
    float dx = scale * (0.5-x),  dy = scale * (0.5-y),  dz = scale * (0.5-z);

    return(pow(sqrt(dx * dx + dy * dy) - a,2) + dz * dz);
}

//нахождение ближайшей точки пересечения поверхности с кубом
float MarchingCubes::getOffset(const float &value1, const float &value2, const float &isolevel){

    double delta = value2 - value1;

    if(delta == 0.0)
    {
        return 0.5;
    }
    return (isolevel - value1)/delta;
}

//нахождение вектора-градиента в точке
void MarchingCubes::getNormal(QVector3D &normal, const float &x, const float &y, const float &z){
    
    float eps = 0.01;
    normal.setX( (this->*sample)(x-eps, y, z) - (this->*sample)(x+eps, y, z));
    normal.setY( (this->*sample)(x, y-eps, z) - (this->*sample)(x, y+eps, z));
    normal.setZ( (this->*sample)(x, y, z-eps) - (this->*sample)(x, y, z+eps));
    normal.normalize();

}

void MarchingCubes::marchingCubes(int nsample,float isolevel){
    
    QString objFilename;
    switch (nsample){
        case 1:{
            sample = &MarchingCubes::sample1;
            isolevel = 0.05;
            objFilename = "sphere.obj";
            break;
        }
        case 2:{
            sample = &MarchingCubes::sample2;
            isolevel = 0.13;
            objFilename = "ellipsoid.obj";
            break;
        }
        case 3:{
            sample = &MarchingCubes::sample3;
            isolevel = 0.3;
            objFilename = "ellipticalCylinder.obj";
            break;
        }
        case 4:{
            sample = &MarchingCubes::sample4;
            isolevel = 0.05;
            objFilename = "cone.obj";
            break;
        } 
        case 5:{
            sample = &MarchingCubes::sample5;
            isolevel = 2.1;
            objFilename = "toroid.obj"; 
            break;
        }   
        default:
            break;
    }
    
    
    for(int x = 0; x < dataSetSize; ++x)
        for(int y = 0; y < dataSetSize; ++y)
            for(int z = 0; z < dataSetSize; ++z)
                marchCube(x*stepSize, y*stepSize, z*stepSize, stepSize, isolevel);
    
    if (isNeedObjFile)
        createObjFile(objFilename);

        
}


void MarchingCubes::marchCube(const float &x, const float &y, const float &z,
    const float &scale, const float &isolevel){
    
    int vertex, edge, triangle;
    int edgesIndex = 0;
    float cubeValue[8];
    QVector3D edgePoint[12];
    QVector3D edgeNorm[12];

  //  fileStream<<"x: "<<x<<" y: "<<y<<" z: "<<z<<"\n";
   //вычисление значения функции в текущей точке
  //вычиление вершин, лежащих ниже точки пересечения поверхности с кубом 
    for(vertex = 0; vertex < 8; ++vertex) {

        cubeValue[vertex] = (this->*sample)(x + VertexOffset[vertex][0]*scale,y + VertexOffset[vertex][1]*scale,z + VertexOffset[vertex][2]*scale);
        
        if(cubeValue[vertex] <= isolevel)
            edgesIndex |= 1<<vertex;

  //      fileStream << "value toroid: "<< vertex<< ": "<< cubeValue[vertex]<<"\n";
    }
        
    //нахождение ребер, пересекающих поверхность по таблице ребер
    int edgeFlags = EdgeTable[edgesIndex];

 //   fileStream<<"iFlagIndex: "<<edgesIndex<<"\n";
   // fileStream<<"iEdgeFlags: "<<edgeFlags<<"\n";

    //если пересечений не найдено, куб лежит выше или ниже заданного изоуровня
    if(edgeFlags == 0)
        return;

    //линейная интерполяция точек пересечения заданной поверхности с поверхностью куба
    //нахождение нормали  к ребру, в точке поверхности, пересекающей ребро куба
    for(edge = 0; edge < 12; ++edge) {
        //если поверхность пресекает ребро
        if(edgeFlags & (1<<edge)) {

            float offset = getOffset(cubeValue[ EdgeConnection[edge][0] ],cubeValue[ EdgeConnection[edge][1] ], isolevel);

            edgePoint[edge].setX( x + (VertexOffset[ EdgeConnection[edge][0] ][0]  +  offset * EdgeDirection[edge][0]) * scale);
            edgePoint[edge].setY( y + (VertexOffset[ EdgeConnection[edge][0] ][1]  +  offset * EdgeDirection[edge][1]) * scale);
            edgePoint[edge].setZ( z + (VertexOffset[ EdgeConnection[edge][0] ][2]  +  offset * EdgeDirection[edge][2]) * scale);

            getNormal(edgeNorm[edge], edgePoint[edge].x(), edgePoint[edge].y(), edgePoint[edge].z());

  /*          fileStream<<"iEdge: "<<edge<<"\n";
            fileStream<<"asEdgeVertex["<<edge<<"].x: "<<edgePoint[edge].x();
            fileStream<<"asEdgeVertex["<<edge<<"].y: "<<edgePoint[edge].y();
            fileStream<<"asEdgeVertex["<<edge<<"].z: "<<edgePoint[edge].z()<<"\n";
            fileStream<<"asEdgeNorm["<<edge<<"]: "<<edgeNorm[edge].x()<<" "<<edgeNorm[edge].y()<<" "<<edgeNorm[edge].z()<<"\n";
        */}
    }
    
    //нахождение вершин треугольников, образующих плоскости в кубе
    for(triangle = 0; triangle < 5; ++triangle) {

        if(TriangleTable[edgesIndex][3*triangle] < 0)
            break;
      //  fileStream<<"Triangle["<<triangle<<"]\n";
        for(int corner = 0; corner < 3; ++corner) {

            edge = TriangleTable[edgesIndex][3*triangle+corner];
            vertices.append(edgePoint[edge].x());
            vertices.append(edgePoint[edge].y());
            vertices.append(edgePoint[edge].z());
            normals.append(edgeNorm[edge].x());
            normals.append(edgeNorm[edge].y());
            normals.append(edgeNorm[edge].z());
            
     //       fileStream<<"Tri: "<<edgePoint[vertex].x()<<" "<< edgePoint[vertex].y()<<" "<<edgePoint[vertex].z()<<"\n";
            
        }
    }
}

