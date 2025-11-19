#include "_enemy.h"

_enemy::_enemy()
{
    //ctor


}

_enemy::~_enemy()
{
    //dtor
}
void _enemy::init(const char* filename){
    initModel(filename);
}

// Spawns the enemy at a random location around the player
void _enemy::spawn(vec3 center){
    speed = 0.005f + rand()%50 * 0.0001f;
    float radius = rand()%500/10.0f + 30;
    angle  = rand()%360 / 180.0f * PI;
    // Calculate the random point
    //vec3 spawnPoint;
    pos.x = center.x + radius * cos(angle);
    pos.y = 0.0f;
    pos.z = center.z + radius * sin(angle);
    std::cout << radius << " " << angle << " " << pos.x << pos.z << std::endl;
    isSpawned = isAlive = true;
}
void _enemy::draw() {
    glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(angle, 0, 1, 0);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glScalef(0.1,0.1,0.1);
        actionTrigger = action;
        Actions();
        Draw();
    glPopMatrix();
}

void _enemy::moveTowardPoint(vec3 point) {
    // create a vector pointing at the player
    vec3 path;
    path.x = point.x - pos.x;
    path.y = 0; // ignore vertical rotation for 2D sprites
    path.z = point.z - pos.z;
    this-> angle = atan2f(path.x, path.z) * 180.0f / PI;
    // Normalize the vector, to control speed better
    float length = sqrtf(path.x*path.x + path.z*path.z);
    if(length > 0.01f){
        path.x /= length;
        path.z /= length;

        pos.x += path.x * speed;
        pos.z += path.z * speed;

        //action = RUN;
    }
    // Now that the vector has been normalized, move the enemy
    // glTranslatef(pos.x + path.x, pos.y, pos.z + path.z);
    // glRotatef(angle,0,1,0);
}
