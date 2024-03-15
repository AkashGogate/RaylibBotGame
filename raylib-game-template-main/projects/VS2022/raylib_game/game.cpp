#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <list>

using namespace std;

#include <map>





class Vehicle {
private:
    bool evolved;
    int evolutionTimer;
    char name [10];

public:
    Vector2 location;
    Vector2 velocity;
    Vector2 acceleration;
    float r;
    float maxforce;
    float maxspeed;
    int foodCollected;





    Vehicle(double x, double y, double speed, double force, double range, const char * name) : location((Vector2){x,y}), acceleration((Vector2){0, 0}), velocity((Vector2){0, 0}), r(range), maxspeed(speed), maxforce(force), evolved(false), evolutionTimer(0) {
        strcpy(this->name,name);
    }


    void update() {
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
        location.x += velocity.x;
        location.y += velocity.y;
        acceleration.x *= 0;
        acceleration.y *= 0;
    }

    void applyForce(Vector2 force) {
        acceleration.x += force.x;
        acceleration.y += force.y;
    }


    void seek(Vector2 target) {

        Vector2 desired = Vector2Subtract(target,location);
        desired = Vector2Normalize(desired);
        desired = Vector2Scale(desired, maxspeed);
        Vector2 steer = Vector2Subtract(desired,velocity);
        steer = Vector2ClampValue(steer, -1 * maxforce, maxforce);
        applyForce(steer);

        applyForce(steer);

    }
    void evolve() {
        // Implement agent evolution here (e.g., change characteristics)
        // Provide visible cues for the evolution (e.g., change color or display a message)
        // Reset the evolution timer
        evolutionTimer = 0;
    }
    void incFood(){
        foodCollected++;
    }
    bool hasEvolved() {
        return evolved;
    }

};

void removeIndElement(int* arr, unsigned long size, int indexToRemove) {
    // Check if the array is empty
    if (size == 0) {
        std::cout << "Array is empty, cannot remove element." << std::endl;
        return;
    }

    int index = -1; // Initialize the index where the element is found

    // Find the index of the element to remove
    for (int i = 0; i < size-1; i++) {
        if (i == indexToRemove) {
            index = i;
            break;
        }
    }

    // If the element was found
    if (index != -1) {
        // Shift all elements after the index one position to the left
        for (int i = index; i < size; i++) {
            arr[i] = arr[i + 1];
        }

        // Decrease the size of the array
        size--;


    } else {

    }

}

int findBestShip(Vehicle * vehicles[10]){
    int indBest = -1;
    int bestFood = 0;
    for(int i = 1; i<10; i++){
        if(vehicles[i]== NULL)continue;
        if(vehicles[i]->foodCollected>bestFood){
            bestFood = vehicles[i]->foodCollected;
            indBest = i;
        }
    }
    return indBest;
}
int findWorstShip(Vehicle * vehicles[10]){
    int indWorst = -1;
    int worstFood = INT32_MAX;
    for(int i = 9; i>=0; i--){
        if(vehicles[i]== NULL)continue;
        if(vehicles[i]->foodCollected<worstFood){
            worstFood = vehicles[i]->foodCollected;
            indWorst = i;
        }
    }
    return indWorst;
}

void combineVehicles(Vehicle * vehicles[10], Texture2D * text, Rectangle * shipRect[10]){
    for(int i = 0; i<10; i++){
        if(vehicles[i] == NULL) continue;
        for(int j = i+1; j<10; j++){
         if(vehicles[j] == NULL) continue;
            if(Vector2Distance(vehicles[i]->location, vehicles[j]->location)<40){

                delete shipRect[i];
                delete shipRect[j];



                Vehicle * newVehicle = new Vehicle(vehicles[i]->location.x, vehicles[i]->location.y, 3, 0.2, vehicles[i]->r+vehicles[j]->r, "nfl");
                text[i] = LoadTexture("../.././raylib-game-template-main/projects/VS2022/raylib_game/resources/chicken.png");
                shipRect[i] = new Rectangle((Rectangle){0, 0, text[i].width, text[i].height});

                delete vehicles[i];
                delete vehicles[j];

                vehicles[i] = NULL;
                vehicles[j] = NULL;

                vehicles[i] = newVehicle;
            }
        }
    }
}


int main(void) {


    // cout << "The working directory is " << GetWorkingDirectory() << endl;

    srand(time(NULL));
    InitWindow(1600, 800, "raylib [core] example - basic window");

    SetTargetFPS(40);
    int fps = 40;

    int x1;
    int y1;


    int actualSize = 200;

    int nums[actualSize];
    int nums2[actualSize];

    int count = 0;
    Texture2D textures[10];
    Rectangle * shipRects[10];
    Vector2 * shipPositions[10];
    Vehicle * allVehicles[10];
    Vector2 * targets[10];

    for(int i = 0; i<5; i++){
        textures[i] = LoadTexture("../.././raylib-game-template-main/projects/VS2022/raylib_game/resources/saw.png");


        int x3 = GetRandomValue(20, 1580);
        int y3 = GetRandomValue(20, 780);

        allVehicles[i] = new Vehicle(x3, y3, 3, 0.2, 80, "ships");

        shipRects[i] = new Rectangle((Rectangle){0, 0, textures[i].width, textures[i].height});
        shipPositions[i] = new Vector2((Vector2){textures[i].width / 2, textures[i].height / 2});

        targets[i] = new Vector2(((Vector2){0,0}));
    }

    for(int i = 5; i<10; i++){
        textures[i] = LoadTexture("../.././raylib-game-template-main/projects/VS2022/raylib_game/resources/ball.png");

        int x3 = GetRandomValue(20, 1580);
        int y3 = GetRandomValue(20, 780);

        textures[i].width/= 4;
        textures[i].height/=4;

        allVehicles[i] = new Vehicle(x3, y3, 3, 0.2, 0, "chicken");

        shipRects[i] = new Rectangle((Rectangle){0, 0, textures[i].width, textures[i].height});
        shipPositions[i] = new Vector2((Vector2){textures[i].width/2, textures[i].height/2});

        targets[i] = new Vector2(((Vector2){0,0}));
    }



    while (!WindowShouldClose()) {


        BeginDrawing();
        //DrawTexture(ship, 400, 200, WHITE);

        ClearBackground(WHITE);

        // Draw the texture centered




        if (count % (fps * 10) == 0) {

            actualSize = 200;

            for (int i = 0; i < actualSize; i++) {
                int x = GetRandomValue(20, 1580);

                int y = GetRandomValue(20, 780);

                nums[i] = x;
                nums2[i] = y;


            }

            for(int i = 5; i<10; i++){
                x1 = GetRandomValue(20, 1400);
                y1 = GetRandomValue(20, 780);
                targets[i]->x = x1;
                targets[i]->y = y1;
            }
            if(count != 0){

                int bestShip = findBestShip(allVehicles);
                int worstShip = findWorstShip(allVehicles);

                delete allVehicles[worstShip];

                if(bestShip == worstShip){
                    CloseWindow();
                    exit(0);
                }


                allVehicles[worstShip] = NULL;

                if(bestShip<5){
                    allVehicles[bestShip]->r*=1.25;
                }
                else{
                    allVehicles[bestShip]->maxspeed*=1.5;
                }


                for(int i = 0; i<10; i++){
                    if(allVehicles[i]==NULL) continue;
                    allVehicles[i]->foodCollected = 0;
                }

                combineVehicles(allVehicles, textures, shipRects);
            }



        }



        for(int shi = 0; shi<5; shi++){
            int closest = 10000;
            int xClose;
            int yClose;

            if(allVehicles[shi]!= NULL){
                for (int i = 0; i < actualSize; i++) {
                    int distance = Vector2Distance(allVehicles[shi]->location, (Vector2) {nums[i], nums2[i]});

                    if (distance < closest && distance<allVehicles[shi]->r) {
                        xClose = nums[i];
                        yClose = nums2[i];
                        closest = Vector2Distance(allVehicles[shi]->location, (Vector2) {nums[i], nums2[i]});
                    }

                }
                if(closest == 10000){
                    targets[shi]->x = GetRandomValue(20,1580);
                    targets[shi]->y = GetRandomValue(20,780);
                }
                else{
                    targets[shi]->x = xClose;
                    targets[shi]->y = yClose;
                }
            }

        }

        for(int i = 0; i<actualSize; i++){
            DrawCircle(nums[i], nums2[i], 5.0f, RED);
        }


        int minDistance = 0;
        int index = 0;

        for(int i = 0; i<10; i++){
            if(allVehicles[i]==NULL) continue;


            allVehicles[i]->seek(*targets[i]);
            allVehicles[i]->update();
            DrawCircle(allVehicles[i]->location.x, allVehicles[i]->location.y, allVehicles[i]->r, Fade(RED, 0.3f));
            DrawTexturePro(textures[i],
                           *shipRects[i],
                           (Rectangle) {allVehicles[i]->location.x, allVehicles[i]->location.y,  shipRects[i]->width,  shipRects[i]->height},
                           *shipPositions[i],
                           atan2(allVehicles[i]->velocity.y, allVehicles[i]->velocity.x) * RAD2DEG +
                           90, // Rotate ship to face direction
                           WHITE);
        }


        EndDrawing();
        count++;

        for(int veh = 0; veh<5; veh++){
            int closest = 10000;
            int xClose;
            int yClose;
            if(allVehicles[veh] == NULL) continue;

            for (int ind = 0; ind < actualSize; ind++) {

                if ((Vector2Distance(allVehicles[veh]->location, (Vector2){nums[ind], nums2[ind]})<shipRects[veh]->width/5)) {

                    removeIndElement(nums, actualSize, ind);
                    removeIndElement(nums2, actualSize, ind);
                    actualSize--;

                    for (int dots = 0; dots < actualSize; dots++) {
                        int distance = Vector2Distance(allVehicles[veh]->location, (Vector2) {nums[dots], nums2[dots]});

                        if (distance < closest && distance<allVehicles[veh]->r) {
                            xClose = nums[dots];
                            yClose = nums2[dots];
                            closest = Vector2Distance(allVehicles[veh]->location, (Vector2) {nums[dots], nums2[dots]});
                        }
                    }

                    if(closest == 10000){
                        targets[veh]->x = GetRandomValue(20,1580);
                        targets[veh]->y = GetRandomValue(20,780);
                    }
                    else{
                        targets[veh]->x = xClose;
                        targets[veh]->y = yClose;
                    }


                    allVehicles[veh]->incFood();

                }

            }
        }
        for(int chi = 5; chi<10; chi++){
            int closest = 10000;
            int xClose;
            int yClose;
            if(allVehicles[chi] == NULL) continue;

            for (int ind = 0; ind < actualSize; ind++) {

                if ((Vector2Distance(allVehicles[chi]->location, (Vector2) {nums[ind], nums2[ind]}) < shipRects[chi]->width/2)) {

                    removeIndElement(nums,  actualSize, ind);
                    removeIndElement(nums2, actualSize, ind);
                    actualSize--;
                    allVehicles[chi]->incFood();

                }

            }
            if ((Vector2Distance(allVehicles[chi]->location, *targets[chi]) < shipRects[chi]->width/2)) {
                cout<< "Resetting Chicken Heading" << endl;
                x1 = GetRandomValue(20, 1580);
                y1 = GetRandomValue(20, 780);
                cout <<"Targets of Chi " << targets[chi] << " , x1: " << x1 << " , y1: " <<  y1 << endl;
                targets[chi]->x = x1;
                targets[chi]->y = y1;
            }

        }
    }

    CloseWindow();

}