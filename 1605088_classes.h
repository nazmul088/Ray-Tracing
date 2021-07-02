#include<stdio.h>
#include<iostream>
#include<bits/stdc++.h>
#include "bitmap_image.hpp"

#define pi (2*acos(0.0))
using namespace std;



int stacks = 40;
int	slices = 60;
int windowWidth = 500;
int windowHeight = 500;




 class Vector3D{
 private:
        double x,y,z;
 public:

     Vector3D() {};

     Vector3D(double a, double b, double c)
     {
         x = a;
         y = b;
         z = c;
     }
     void setX(double a)
     {
         x = a;
     }

     void setY(double b)
     {
         y = b;
     }

     void setZ(double c)
     {
         z = c;
     }

     double getX()
     {
         return x;
     }
     double getY()
     {
         return y;
     }
     double getZ()
     {
         return z;
     }
 };




struct point
{
	double x, y, z;
};




extern struct point position, u, r, l;

class Ray{
    Vector3D start;
    Vector3D dir;

public:

    Ray(){};
    Ray(Vector3D a,Vector3D b)
    {
        start.setX(a.getX());
        start.setY(a.getY());
        start.setZ(a.getZ());

        dir.setX(b.getX());
        dir.setY(b.getY());
        dir.setZ(b.getZ());
        }

    Vector3D getStart(){
        return start;
    }

    Vector3D getDir()
    {
        return dir;
    }

    void setStart(Vector3D v)
    {
        start.setX(v.getX());
        start.setY(v.getY());
        start.setZ(v.getZ());
    }

    void setDir(Vector3D v)
    {
        dir.setX(v.getX());
        dir.setY(v.getY());
        dir.setZ(v.getZ());
    }
    };




class Light{
    Vector3D light_pos;
    double color[3];
    public:
        Light(){};
        Light(Vector3D pos,double c[3])
        {
            light_pos.setX(pos.getX());
            light_pos.setY(pos.getY());
            light_pos.setZ(pos.getZ());
            color[0] = c[0];
            color[1] = c[1];
            color[2] = c[2];
        }

        double* getColor()
        {
            return color;
        }

        void draw()
        {
            glColor3f(color[0],color[1],color[2]);
            glBegin(GL_QUADS); {
                    glVertex3f(light_pos.getX()+2, light_pos.getY()+2, light_pos.getZ());
                    glVertex3f(light_pos.getX()+2, light_pos.getY()-2, light_pos.getZ());
                    glVertex3f(light_pos.getY()-2, light_pos.getY()-2, light_pos.getZ());
                    glVertex3f(light_pos.getX()-2, light_pos.getY()+2, light_pos.getZ());
                }glEnd();

        }

        Vector3D getLightPosition()
        {
            return light_pos;
        }
};


 class Object{

        Vector3D reference_point;
        double height,width,length;
        double color[3];
        double coEfficients[4];
        int shine;
 public:
        Object(){};
        virtual void draw(){};
        virtual double intersect(Ray* r,double* color,int level){};
        void setReference_point(double x,double y,double z)
        {
            reference_point.setX(x);
            reference_point.setY(y);
            reference_point.setZ(z);

        }

        Vector3D getReference_point()
        {
            return reference_point;
        }

        void setColor(double c[3]){
            color[0] = c[0];
            color[1] = c[1];
            color[2] = c[2];
        };
        void setShine(int s){
            shine = s;
        };

        int getShine()
        {
            return shine;
        }
        void setCoEfficients(double co[4]){
            coEfficients[0] = co[0];
            coEfficients[1] = co[1];
            coEfficients[2] = co[2];
            coEfficients[3] = co[3];
        };

        void setLength(double l)
        {
            length = l;
        }

        double getLength()
        {
            return length;
        }

        double* getColor()
        {
            return color;
        }

        double* getCoefficients()
        {
            return coEfficients;
        }
};


extern vector<Object*> objects;
extern vector<Light*> lights;



class Sphere : public Object{
public:
    Sphere(Vector3D center,double radius){
        setReference_point(center.getX(), center.getY(), center.getZ());
        setLength(radius);
    }

    void draw()
    {
        Vector3D points[100][100];
        int i, j;
        double h, r;
        //generate points
        for (i = 0;i <= stacks;i++)
        {
            h = getLength() * sin(((double)i / (double)stacks) * (pi / 2));
            r = getLength() * cos(((double)i / (double)stacks) * (pi / 2));
		for (j = 0;j <= slices;j++)
		{
			points[i][j].setX(  r * cos(((double)j / (double)slices) * 2 * pi)) ;
			points[i][j].setY(r * sin(((double)j / (double)slices) * 2 * pi)) ;
			points[i][j].setZ(h);
		}
        }
        //draw quads using generated points

        glPushMatrix();
        glTranslated(getReference_point().getX(),getReference_point().getY(),getReference_point().getZ());
        for (i = 0;i < stacks;i++)
        {
            double* col = getColor();
            glColor3f(col[0],col[1],col[2]);
            for (j = 0;j < slices;j++)
            {
                glBegin(GL_QUADS); {
                    //upper hemisphere
                    glVertex3f(points[i][j].getX(), points[i][j].getY(), points[i][j].getZ());
                    glVertex3f(points[i][j + 1].getX(), points[i][j + 1].getY(), points[i][j + 1].getZ());
                    glVertex3f(points[i + 1][j + 1].getX(), points[i + 1][j + 1].getY(), points[i + 1][j + 1].getZ());
                    glVertex3f(points[i + 1][j].getX(), points[i + 1][j].getY(), points[i + 1][j].getZ());
                    //lower hemisphere
                    glVertex3f(points[i][j].getX(), points[i][j].getY(), -points[i][j].getZ());
                    glVertex3f(points[i][j + 1].getX(), points[i][j + 1].getY(), -points[i][j + 1].getZ());
                    glVertex3f(points[i + 1][j + 1].getX(), points[i + 1][j + 1].getY(), -points[i + 1][j + 1].getZ());
                    glVertex3f(points[i + 1][j].getX(), points[i + 1][j].getY(), -points[i + 1][j].getZ());
			}glEnd();
		}
	}
	glPopMatrix();
	}


	double findtMin(Vector3D center,double radius,Ray* r)
	{
	    Vector3D oc;
	    oc.setX(r->getStart().getX()-center.getX());
        oc.setY(r->getStart().getY()-center.getY());
        oc.setZ(r->getStart().getZ()-center.getZ());
        double a = (r->getDir().getX()*r->getDir().getX())+(r->getDir().getY()*r->getDir().getY())+(r->getDir().getZ()*r->getDir().getZ());
        double b = 2.0 * ((oc.getX()*r->getDir().getX())+(oc.getY()*r->getDir().getY())+(oc.getZ()*r->getDir().getZ()));
        double c = (oc.getX()*oc.getX())+(oc.getY()*oc.getY())+(oc.getZ()*oc.getZ()) - radius*radius;
        double discriminant = (b*b) - (4*a*c);
        if(discriminant<0)
        {
            return -1.0;
        }
        else
        {
            return ((-b-sqrt(discriminant))/(2.0*a));
        }
	}

    double intersect(Ray* r,double* color,int level){
        double tMin = findtMin(getReference_point(),getLength(),r);
        if(level == 0)
            return tMin;
        Vector3D intersection_point;
        intersection_point.setX(r->getStart().getX() + r->getDir().getX()*tMin);
        intersection_point.setY(r->getStart().getY() + r->getDir().getY()*tMin);
        intersection_point.setZ(r->getStart().getZ() + r->getDir().getZ()*tMin);
        double *intersection_point_color;
        intersection_point_color = new double[3];
        intersection_point_color = getColor();
        color[0] = intersection_point_color[0]*getCoefficients()[0];
        color[1] = intersection_point_color[1]*getCoefficients()[0];
        color[2] = intersection_point_color[2]*getCoefficients()[0];
        Vector3D normal;
        normal.setX(intersection_point.getX()-getReference_point().getX());
        normal.setY(intersection_point.getY()-getReference_point().getY());
        normal.setZ(intersection_point.getZ()-getReference_point().getZ());
        vector<Light*>::iterator iter,end;
        for(iter = lights.begin(), end = lights.end() ; iter != end; ++iter)
        {
            //Vector3D v(intersection_point.getX()-(*iter)->getLightPosition().getX(),intersection_point.getY()-(*iter)->getLightPosition().getY(),intersection_point.getZ()-(*iter)->getLightPosition().getZ());
            double normalized = pow(intersection_point.getX(),2)+pow(intersection_point.getY(),2)+pow(intersection_point.getZ(),2);
            normalized = sqrt(normalized);
            Vector3D r1;
            r1.setX((intersection_point.getX()-(*iter)->getLightPosition().getX())/normalized);
            r1.setY((intersection_point.getY()-(*iter)->getLightPosition().getY())/normalized);
            r1.setZ((intersection_point.getZ()-(*iter)->getLightPosition().getZ())/normalized);
            Vector3D r2;
            r2.setX((*iter)->getLightPosition().getX()/normalized);
            r2.setY((*iter)->getLightPosition().getY()/normalized);
            r2.setZ((*iter)->getLightPosition().getZ()/normalized);

            Ray ray1(r2, r1);
            double t = findtMin(getReference_point(),getLength(),&ray1);
            if(t>0)
                break;
            double lambertValue = getCoefficients()[1] * (ray1.getDir().getX()*normal.getX()+ray1.getDir().getY()*normal.getY()+ray1.getDir().getZ()*normal.getZ());
            //find reflected ray Rayr for Ray1
            double dotResult = (ray1.getDir().getX()*normal.getX())+ (ray1.getDir().getY()*normal.getY()) + (ray1.getDir().getZ()*normal.getZ());
            Vector3D rayr1(intersection_point.getX(),intersection_point.getY(),intersection_point.getZ());
            //rayr.setStart(v1);
            Vector3D rayr2;
            rayr2.setX((2*dotResult*normal.getX())-ray1.getDir().getX());
            rayr2.setY((2*dotResult*normal.getY())-ray1.getDir().getY());
            rayr2.setZ((2*dotResult*normal.getZ())-ray1.getDir().getZ());
            normalized = pow((rayr1.getX()+rayr2.getX()),2) + pow((rayr1.getY()+rayr2.getY()),2) + pow((rayr1.getZ()+rayr2.getZ()),2);

            rayr1.setX(rayr1.getX()/normalized);
            rayr1.setY(rayr1.getY()/normalized);
            rayr1.setZ(rayr1.getZ()/normalized);

            rayr2.setX(rayr2.getX()/normalized);
            rayr2.setY(rayr2.getY()/normalized);
            rayr2.setZ(rayr2.getZ()/normalized);
            //Vector3D p1(intersection_point.getX()-p.getX(),intersection_point.getY()-p.getY(),intersection_point.getZ()-p.getZ()); Add kora lagte pare
            Ray rayr(rayr1,rayr2);

            Vector3D view1(r->getStart().getX(),r->getStart().getY(),r->getStart().getZ());
            //view.setStart(p2);
            Vector3D view2(r->getStart().getX()-intersection_point.getX(),r->getStart().getY()-intersection_point.getY(),r->getStart().getZ()-intersection_point.getZ());
            normalized = pow(r->getStart().getX()+r->getStart().getX()-intersection_point.getX(),2) + pow(r->getStart().getY()+r->getStart().getY()-intersection_point.getY(),2) + pow(r->getStart().getZ()+r->getStart().getZ()-intersection_point.getZ(),2);
            normalized = sqrt(normalized);
            view1.setX(view1.getX()/normalized);
            view1.setY(view1.getY()/normalized);
            view1.setZ(view1.getZ()/normalized);

            view2.setX(view2.getX()/normalized);
            view2.setY(view2.getY()/normalized);
            view2.setZ(view2.getZ()/normalized);

            Ray View(view1,view2);

            double phongValue = (View.getDir().getX()*rayr.getDir().getX() + View.getDir().getY()*rayr.getDir().getY() + View.getDir().getZ()*rayr.getDir().getZ());
            color[0] = color[0]+ (*iter)->getColor()[0]*getCoefficients()[1]*lambertValue*getColor()[0];
            color[1] = color[1]+ (*iter)->getColor()[1]*getCoefficients()[1]*lambertValue*getColor()[1];
            color[2] = color[2]+ (*iter)->getColor()[2]*getCoefficients()[1]*lambertValue*getColor()[2];

            color[0] = color[0]+ (*iter)->getColor()[0]*getCoefficients()[2]*  pow(phongValue,getShine())*getColor()[0];
            color[1] = color[1]+ (*iter)->getColor()[1]*getCoefficients()[2]*  pow(phongValue,getShine())*getColor()[1];
            color[2] = color[2]+ (*iter)->getColor()[2]*getCoefficients()[2]*  pow(phongValue,getShine())*getColor()[2];
        }
}



};


class Triangle : public Object {

    Vector3D a, b, c;
public:
    Triangle(Vector3D d, Vector3D e, Vector3D f)
    {
        a.setX(d.getX());
        a.setY(d.getY());
        a.setZ(d.getZ());

        b.setX(e.getX());
        b.setY(e.getY());
        b.setZ(e.getZ());

        c.setX(f.getX());
        c.setY(f.getY());
        c.setZ(f.getZ());
    }

    void draw()
    {

       // glTranslatef(getReference_point().getX(),getReference_point().getY(),getReference_point().getZ());
        glBegin(GL_TRIANGLES);
		{
			glVertex3f(a.getX(), a.getY(), a.getZ());
			glVertex3f(b.getX(), b.getY(), b.getZ());
			glVertex3f(c.getX(), c.getY(), c.getZ());
		}
		glEnd();
    }

};



class Floor: public Object{

public:
    Floor( double floor_width, double tile_width)
    {
            setReference_point(-floor_width/2,-floor_width/2,0);
            setLength(tile_width);
    }

    void draw()
    {
        int initialCoordinateX = getReference_point().getX();
        int initialCoordinateY = getReference_point().getY();
        int totalTiles = (-getReference_point().getX()*2)/getLength();
       // cout<<"Total tiles"<<totalTiles<<endl;

        for(int i=0;i<totalTiles;i++)
        {

             int initialCoordinateX = getReference_point().getX();
            for(int j=0;j<totalTiles;j++)
            {
                if(i%2==0  && j%2==0)
                glColor3f(1,1,1);
                else if(i%2==0 && j%2!=0)
                    glColor3f(0,0,0);
                else if(i%2!=0 && j%2==0)
                    glColor3f(0,0,0);
                else if(i%2!=0 && j%2!=0)
                glColor3f(1,1,1);

                glBegin(GL_QUADS); {
                    glVertex3f(initialCoordinateX+getLength()/2, initialCoordinateY+getLength()/2, 0);
                    glVertex3f(initialCoordinateX+getLength()/2, initialCoordinateY-getLength()/2, 0);
                    glVertex3f(initialCoordinateX-getLength()/2, initialCoordinateY-getLength()/2, 0);
                    glVertex3f(initialCoordinateX-getLength()/2, initialCoordinateY+getLength()/2, 0);
                }glEnd();
            initialCoordinateX = initialCoordinateX+20;
            }
            initialCoordinateY= initialCoordinateY+20;

        }

};
};



//Capture method

void capture()
{
    cout<<"Capture Called"<<endl;
    double viewAngle = (90 * 180) / pi;
    double planeDistance =(windowHeight/2.0)/tan(viewAngle/2.0);
    struct point topLeft;
    topLeft.x = position.x + l.x*planeDistance - r.x*windowWidth/2 + u.x*windowHeight/2;
    topLeft.y = position.y + l.y*planeDistance - r.y*windowWidth/2 + u.y*windowHeight/2;
    topLeft.z = position.z + l.z*planeDistance - r.z*windowWidth/2 + u.z*windowHeight/2;
    double du = windowWidth/500;
    double dv = windowHeight/500;
    topLeft.x = topLeft.x + r.x*(0.5*du)-u.x*(0.5*dv);
    topLeft.y = topLeft.y + r.y*(0.5*du)-u.y*(0.5*dv);
    topLeft.z = topLeft.z + r.z*(0.5*du)-u.z*(0.5*dv);
    int nearest;
    double t=999999,tMin;
    bitmap_image image(windowHeight,windowWidth);
    for(int i=0;i<500;i++)
    {
        for(int j=0;j<500;j++)
        {
            //calculate current pixel
            Vector3D current_pixel;
            current_pixel.setX(topLeft.x + r.x*(i*du)-u.x*(j*dv));
            current_pixel.setY(topLeft.y + r.y*(i*du)-u.y*(j*dv));
            current_pixel.setZ(topLeft.z + r.z*(i*du)-u.z*(j*dv));
            Vector3D eye(position.x,position.y,position.z);
            Vector3D eyeDir(current_pixel.getX()-eye.getX(),current_pixel.getY()-eye.getY(),current_pixel.getZ()-eye.getZ());
            double valueNormalized = pow((eye.getX()+eyeDir.getX()),2) + pow((eye.getY()+eyeDir.getY()),2) + pow((eye.getZ()+eyeDir.getZ()),2);
            valueNormalized = sqrt(valueNormalized);
            eye.setX(position.x/valueNormalized);
            eye.setY(position.y/valueNormalized);
            eye.setZ(position.z/valueNormalized);

            eyeDir.setX((current_pixel.getX()-eye.getX())/valueNormalized);
            eyeDir.setY((current_pixel.getY()-eye.getY())/valueNormalized);
            eyeDir.setZ((current_pixel.getZ()-eye.getZ())/valueNormalized);

            Ray ray(eye,eyeDir);
            double* color;
            color = new double[3];
            vector<Object*>::iterator iter,end;
            Object* nearest_object=NULL;
            for(iter = objects.begin(), end = objects.end() ; iter != end; ++iter)
            {
                double* dummyColor;
                dummyColor = new double[3];
                double temp =(*iter)->intersect(&ray,dummyColor,0);
                if(temp>0 && temp<t)
                    {
                        t = temp;
                        nearest_object = *iter;

                    }
            }
            //cout<<"Length is:"<<nearest_object->getLength()<<endl;
            if(nearest_object!=NULL)
            {
                tMin = nearest_object->intersect(&ray,color,1);
            }
            //delete[] color;
            image.set_pixel(i,j,color[0],color[1],color[2]);

    }
    }
    image.save_image("F:\\Study\\L-4-T-1\\Computer Graphics Sessional\\Offline 3\\1605088\\1605088_main\\out.bmp");

}

