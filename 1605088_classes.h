#include<stdio.h>
#include<iostream>
#include<bits/stdc++.h>

#define pi (2*acos(0.0))
using namespace std;



int stacks = 40;
int	slices = 60;




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


 class Object{

        Vector3D reference_point;
        double height,width,length;
        double color[3];
        double coEfficients[4];
        int shine;
 public:
        Object(){};
        virtual void draw(){};
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
};

class Sphere : public Object{
public:
    Sphere(Vector3D center,double radius){
        setReference_point(center.getX(), center.getY(), center.getZ());
        setLength(radius);
    }

    void draw(){

       // glTranslatef(getReference_point().getX(),getReference_point().getY(),getReference_point().getZ());
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
			points[i][j].setX(r * cos(((double)j / (double)slices) * 2 * pi)) ;
			points[i][j].setY(r * sin(((double)j / (double)slices) * 2 * pi)) ;
			points[i][j].setZ(h);
		}
        }
        //draw quads using generated points
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

             int initialCoordinateX = -500;
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
                    glVertex3f(initialCoordinateX+10, initialCoordinateY+10, 0);
                    glVertex3f(initialCoordinateX+10, initialCoordinateY-10, 0);
                    glVertex3f(initialCoordinateX-10, initialCoordinateY-10, 0);
                    glVertex3f(initialCoordinateX-10, initialCoordinateY+10, 0);
                }glEnd();
            initialCoordinateX = initialCoordinateX+20;
            }
            initialCoordinateY= initialCoordinateY+20;

        }

};
};
