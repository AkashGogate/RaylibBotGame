//
// Created by Akash Gogate on 10/6/23.
//

#ifndef EXAMPLE_PLAYER_H
#define EXAMPLE_PLAYER_H


class Player {
    Player::Player(float x, float y) {
    acceleration = (Vector2){0,0};
    velocity = (Vector2) {0,0};
    location = (Vector2) {x,y};
    r = 3.0;
    //[full] Arbitrary values for maxspeed and
    // force; try varying these!
    maxspeed = 4;
    maxforce = 0.1;
    //[end]
}

void Player::update() {
    velocity = Vector2Add(velocity, acceleration);
    velocity = Vector2ClampValue(velocity, -1 * maxspeed, maxspeed);
    location = Vector2Add(location, velocity);
    acceleration = Vector2Scale(acceleration, 0);
}

void Player::applyForce(Vector2 force) {
    acceleration = Vector2Add(acceleration, force);
}

void Player::seek(Vector2 target) {
    Vector2 desired = Vector2Subtract(target,location);
    desired = Vector2Normalize(desired);
    desired = Vector2Scale(desired, maxspeed);
    Vector2 steer = Vector2Subtract(desired,velocity);
    steer = Vector2ClampValue(steer, -1 * maxforce, maxforce);
    applyForce(steer);
}

void Player::arrive(Vector2 target) {
    Vector2 desired = Vector2Subtract(target,location);

    // The distance is the magnitude of
    // the vector pointing from
    // location to target.
    float d = Vector2Length(desired);
    desired = Vector2Normalize(desired);
    // If we are closer than 100 pixels...
    if (d < 100) {
        //[full] ...set the magnitude
        // according to how close we are.
        float m = Remap(d,0,100,0,maxspeed);
        desired = Vector2Scale(desired, m);
        //[end]
    } else {
        // Otherwise, proceed at maximum speed.
        desired = Vector2Scale(desired, maxspeed);
    }

    Vector2 steer = Vector2Subtract(desired,velocity);
    steer = Vector2ClampValue(steer, -1 * maxforce, maxforce);
    applyForce(steer);
}
};


#endif //EXAMPLE_PLAYER_H
