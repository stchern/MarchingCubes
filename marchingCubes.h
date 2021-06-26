#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include <QDebug>
#include <QVector>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QtMath>

#include "marchingCubesData.h"

class MarchingCubes {
    
public:
    MarchingCubes(int resolution = 50);
    ~MarchingCubes(){}

    void marchingCubes(int nsample = 1,float isolevel = 0.05);
    void createObjFile(bool value);

    QVector<float> vertices;
    QVector<float> normals;

private:
    float (MarchingCubes::*sample)(const float &x, const float &y, const float &z);

    float sample1(const float &x, const float &y, const float &z);
    float sample2(const float &x, const float &y, const float &z);
    float sample3(const float &x, const float &y, const float &z);
    float sample4(const float &x, const float &y, const float &z);
    float sample5(const float &x, const float &y, const float &z);
    
    void marchCube(const float &x, const float &y, const float &z, const float &scale, const float &Tv);
   
    float getOffset(const float &value1, const float &value2, const float &isolevel);
    void  getNormal(QVector3D& normal, const float &x, const float &y, const float &z);
    void createObjFile(const QString& filename);

    int dataSetSize;
    float stepSize;
    bool isNeedObjFile;

    MarchingCubes();
    MarchingCubes(const MarchingCubes&);
    MarchingCubes& operator=(const MarchingCubes&);
};

#endif 
