#include <iostream>
#include <memory>
#include "Application.hpp"
#include "Log.hpp"

class MyApp : public LabEngine::Application {
    virtual void on_update() override {
    }    
};
int main() {
    auto myApp = std::make_unique<MyApp>();
    int returnCode = myApp->start(800, 600, "first");
    // system("pause");
    return returnCode;
}
