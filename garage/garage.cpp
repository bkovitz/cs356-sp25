#include <iostream>
#include <string>

enum class State {
        TOP, CLOSING, OPENING, BOTTOM
};

enum class Input  {
        BUTTON, TOP_SENSOR, BOTTOM_SENSOR
};

// Takes the state and prints it
void printState(State state)
{
    std::cout << "Door is ";

    switch(state) {
        case State::TOP:
            std::cout << "at top. Motor is off." << std::endl;
            break;
        case State::CLOSING:
            std::cout << "closing. Motor is on going down." << std::endl;
            break;
        case State::OPENING:
            std::cout << "opening. Motor is on going up." << std::endl;
            break;
        case State::BOTTOM:
            std::cout << "at bottom. Motor is off." << std::endl;
            break;
    }
}

State getState(bool top_sensor, bool bottom_sensor, bool is_moving) {
    if (is_moving) {
        if (top_sensor) {
            return State::CLOSING;
        } else {
            return State::OPENING;
        }
    } else {
        if (top_sensor) {
            return State::TOP;
        } else {
            return State::BOTTOM;
        }
    }
}

Input takeInput(){
    std::string inputString;


    std::cout << "What is your input? b, t, or B?" << std::endl;
    std::getline(std::cin, inputString);

    if (inputString == "b")
    {
        return Input::BOTTOM_SENSOR;
    }
    else if (inputString == "t")
    {
        return Input::TOP_SENSOR;
    }
    else if (inputString == "B")
    {
        return Input::BUTTON;
    }
    std::cout << "Invalid Input, try again :(" << std::endl;
    return takeInput();

}

int main() {
    std::cout << "" << std::endl;

    bool top_sensor = false;
    bool bottom_sensor = true;
    //std::string input;

    Input input;
    State state = State::BOTTOM;

    while(true){
        printState(state);
        input = takeInput();

        switch (state){
            case State::TOP: 
                if (input == Input::BUTTON){
                    state = State::CLOSING;
                }
                break;
            case State::BOTTOM:
                if (input == Input::BUTTON){
                    state = State::OPENING;
                }
                break;
            case State::CLOSING:
                if(input == Input::BOTTOM_SENSOR){
                    state = State::BOTTOM;
                } else if (input == Input::BUTTON)
                {
                    state = State::OPENING;
                } 
                break;
            case State::OPENING:
                 if(input == Input::TOP_SENSOR){
                    state = State::TOP;
                } else if (input == Input::BUTTON)
                {
                    state = State::CLOSING;
                } 
                break;
        }
    }
}

