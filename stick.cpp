#include <QHHeadersGLUT.h>//Include all necessary headers
#include <HL/hl.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define RAD2DEG(x) (180.0*(x)/M_PI)

QHGLUT* DisplayObject;
Text* instructionMsg;//title
Text* instructionMsg2;//show the cursor position
Text* instructionMsg3;//show the velocity
Text* instructionMsg4;//show the jointangle
Text* instructionMsg5;//show the roll, pitch, yaw
Text* instructionMsg6;//show the force

hduVector3Dd position;
HDdouble transform[16];
HDfloat velocity[3];
hduVector3Dd jointangle;
HDdouble force[3];
double cp, roll, pitch, yaw;

void touchCallback(unsigned int ShapeID);
void button1DownCallback(unsigned int ShapeID);
void button1UpCallback(unsigned int ShapeID);
void motionCallback(unsigned int ShapeID);
void button2DownCallback(unsigned int ShapeID);

int main(int argc, char *argv[])
{
    DisplayObject = new QHGLUT(argc,argv);//create a display window
    DeviceSpace* OmniSpace = new DeviceSpace;//Find the default Phantom Device
    DisplayObject->tell(OmniSpace);//Tell QuickHaptics about it
    DisplayObject->setBackgroundColor(212, 244, 250);   

#pragma region Texts

    instructionMsg = new Text(20.0, "The position of pen", 0.25, 0.9);
    instructionMsg->setShapeColor(0.9, 0.9, 0.0);
    DisplayObject->tell(instructionMsg);

    instructionMsg2 = new Text(18.0, "points", 0.1, 0.85);
    instructionMsg2->setShapeColor(0.0, 0.0, 0.0);

    instructionMsg3 = new Text(18.0, "points", 0.1, 0.8);
    instructionMsg3->setShapeColor(0.0, 0.0, 0.0);

    instructionMsg4 = new Text(18.0, "points", 0.1, 0.75);
    instructionMsg4->setShapeColor(0.0, 0.0, 0.0);

    instructionMsg5 = new Text(18.0, "points", 0.1, 0.7);
    instructionMsg5->setShapeColor(0.0, 0.0, 0.0);

    instructionMsg6 = new Text(18.0, "points", 0.1, 0.65);
    instructionMsg6->setShapeColor(0.0, 0.0, 0.0);

#pragma endregion

#pragma region Meshs

    //make basket
    TriMesh* Basket = new TriMesh("models/appleBasket/bowl.3DS");
    Basket->setRotation(hduVector3Dd(1.0, 0.0, 0.0), 45.0);
    Basket->setTexture("models/appleBasket/wood.jpg");
    Basket->setStiffness(0.9);
    Basket->setFriction(0.7, 0.5);
    Basket->setUnDraggable();
    DisplayObject->tell(Basket);

    //make cursor
    Cursor* OmniCursor = new Cursor("models/appleBasket/pencil.3DS");//Declare a new cursor
    OmniCursor->scaleCursor(0.004);
    TriMesh* ModelTriMeshPointer = OmniCursor->getTriMeshPointer();
    ModelTriMeshPointer->setTexture("models/appleBasket/gray.jpg");
    DisplayObject->tell(OmniCursor);//tell QuickHaptics about the cursor

#pragma endregion

    Line* line = new Line(100,100,100,2000,2000,2000);
    line->setWidth(500);
    DisplayObject->tell(line);

    OmniSpace->button1DownCallback(button1DownCallback);
    OmniSpace->button1UpCallback(button1UpCallback);
    OmniSpace->motionCallback(motionCallback);
    OmniSpace->button2DownCallback(button2DownCallback);

    qhStart();//Set everything in motion
     
    return 0;
}

void motionCallback(unsigned int ShapeID)
{
#pragma region Get&Set

    hdGetDoublev(HD_CURRENT_POSITION, position);
    hdGetDoublev(HD_CURRENT_TRANSFORM, transform);
    hdGetFloatv(HD_CURRENT_VELOCITY, velocity);
    hdGetDoublev(HD_CURRENT_JOINT_ANGLES, jointangle);
    hdGetDoublev(HD_CURRENT_FORCE, force);

#pragma endregion
    
    char p_buff[256], v_buff[256], j_buff[256], s_buff[256], f_buff[256];
    sprintf(p_buff, "%lf %lf %lf", position[0], position[1], position[2]);
    sprintf(v_buff, "%f %f %f", velocity[0], velocity[1], velocity[2]);
    sprintf(j_buff, "%lf %lf %lf", RAD2DEG(jointangle[0]), RAD2DEG(jointangle[1]), RAD2DEG(jointangle[2]));
    sprintf(f_buff, "%lf %lf %lf", force[0], force[1], force[2]);

    cp = sqrt(transform[1] * transform[1] + transform[5] * transform[5]); //sqrt(제곱근 구하는 함수)
    roll = atan2(-transform[1], transform[5]);
    pitch = atan2(transform[9], cp);
    yaw = atan2(-transform[8], transform[10]);
    
    sprintf(s_buff, "%lf %lf %lf", RAD2DEG(roll), RAD2DEG(pitch), RAD2DEG(yaw));

#pragma region TestMsgs

    instructionMsg2->setText(p_buff);
    DisplayObject->tell(instructionMsg2);

    instructionMsg3->setText(v_buff);
    DisplayObject->tell(instructionMsg3);

    instructionMsg4->setText(j_buff);
    DisplayObject->tell(instructionMsg4);

    instructionMsg5->setText(s_buff);
    DisplayObject->tell(instructionMsg5);

    instructionMsg6->setText(f_buff);
    DisplayObject->tell(instructionMsg6);

#pragma endregion

}

void touchCallback(unsigned int ShapeID)
{
    TriMesh* touchedApple = TriMesh::searchTriMesh(ShapeID);//Find a pointer to the object touched
    if(touchedApple==NULL)//id the pointer is NULl then return
        return;   
}

void button1DownCallback(unsigned int ShapeID)
{
    instructionMsg->setText("button1 pressed");
    DisplayObject->tell(instructionMsg);
}

void button1UpCallback(unsigned int ShapeID)
{   
    instructionMsg->setText("Position of pen");
    DisplayObject->tell(instructionMsg);
}

void button2DownCallback(unsigned int ShapeID) 
{   
    exit(EXIT_SUCCESS);
}